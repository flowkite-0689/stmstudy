# 串口发送环形缓冲区升级说明

## 升级概述
将原有的**阻塞式串口发送**机制升级为**基于环形缓冲区的非阻塞发送**机制，提升系统实时性能。

## 核心改动

### 1. 新增环形缓冲区数据结构（uart_dma.c）

```c
// 环形缓冲区（应用层）
static uint8_t tx_ring_buf[UART_TX_BUF_SIZE];       // 512字节环形缓冲
static volatile uint16_t tx_write_idx = 0;          // 写指针（生产者：printf）
static volatile uint16_t tx_read_idx  = 0;          // 读指针（消费者：DMA）
static volatile uint8_t  tx_busy = 0;               // DMA发送状态

// DMA硬件缓冲区
static uint8_t uart_dma_tx_buf[UART_TX_BUF_SIZE];   // DMA搬运用的临时缓冲
```

### 2. 新增辅助函数

#### ringbuf_available()
计算环形缓冲区中待发送的数据量。

#### ringbuf_free_space()
计算环形缓冲区剩余可用空间。

#### uart_start_dma_transfer()
核心函数：将环形缓冲区数据搬运到DMA硬件缓冲区并启动发送。
- 处理环形缓冲区跨界情况（读指针到缓冲区末尾 + 缓冲区开头到写指针）
- 更新读指针
- 启动DMA传输
- 设置tx_busy标志

### 3. 修改fputc函数（非阻塞实现）

**原来的实现（阻塞）：**
```c
int fputc(int ch, FILE *f)
{
    static uint8_t printf_buf[1] = {0};
    printf_buf[0] = (uint8_t)ch;
    Usart1_Send_DMA(printf_buf, 1);  // 每次发送1字节
    while (DMA_GetCmdStatus(DMA2_Stream7) == ENABLE); // 阻塞等待
    return (ch);
}
```

**新的实现（非阻塞）：**
```c
int fputc(int ch, FILE *f)
{
    uint8_t temp_char = (uint8_t)ch;
    
    // 缓冲区满时等待（可选策略）
    while (ringbuf_free_space() < 1) {
        uart_start_dma_transfer();
    }
    
    // 写入环形缓冲区（临界区保护）
    __disable_irq();
    tx_ring_buf[tx_write_idx] = temp_char;
    tx_write_idx = (tx_write_idx + 1) % UART_TX_BUF_SIZE;
    __enable_irq();
    
    // 尝试启动DMA（如果空闲）
    if (!tx_busy) {
        uart_start_dma_transfer();
    }
    
    return ch;
}
```

### 4. 增强DMA中断处理

**原来的DMA中断（只清除标志）：**
```c
void DMA2_Stream7_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
        DMA_Cmd(DMA2_Stream7, DISABLE);
        USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
    }
}
```

**新的DMA中断（链式发送）：**
```c
void DMA2_Stream7_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
    {
        DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
        DMA_Cmd(DMA2_Stream7, DISABLE);
        USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
        tx_busy = 0;

        // 关键：如果缓冲区还有数据，继续发送
        if (tx_write_idx != tx_read_idx) {
            uart_start_dma_transfer();
        }
    }
}
```

### 5. 新增API函数（uart_dma.h）

```c
// 主循环任务：确保数据及时发送
void uart_tx_task(void);

// 监控接口：查看缓冲区使用情况
uint16_t uart_get_tx_buf_usage(void);
```

### 6. 修改main.c主循环

```c
while (1)
{
    key = KEY_Get();
    
    // 新增：处理环形缓冲区发送任务
    uart_tx_task();
    
    // 处理串口命令
    Process_Usart_Command();
    
    // ... 其他代码
}
```

## 优势对比

| 特性 | 原阻塞式发送 | 新环形缓冲区发送 |
|------|------------|----------------|
| **性能** | 每个字符阻塞等待DMA完成 | 非阻塞，快速返回 |
| **实时性** | printf会暂停主任务 | 不影响主任务执行 |
| **吞吐量** | 低（逐字节发送） | 高（批量发送） |
| **缓冲能力** | 无缓冲 | 512字节缓冲 |
| **适用场景** | 简单调试 | 高频率日志输出 |

## 工作原理

### 生产者-消费者模式
```
应用层(printf) --> 环形缓冲区 --> DMA硬件缓冲 --> USART1
   (生产者)      tx_ring_buf    uart_dma_tx_buf    (硬件)
                   ↑                                    ↓
                   └────────── DMA完成中断 ──────────────┘
```

### 发送流程
1. **printf调用** → fputc将字符写入tx_ring_buf，更新tx_write_idx
2. **触发发送** → 如果DMA空闲，调用uart_start_dma_transfer()
3. **数据搬运** → 将环形缓冲区数据复制到uart_dma_tx_buf
4. **DMA传输** → 启动DMA，将uart_dma_tx_buf发送到USART1
5. **中断回调** → DMA完成后触发中断，检查是否还有数据需要发送
6. **循环发送** → 如果有数据，重复步骤3-5

### 环形缓冲区处理跨界情况

```
情况1：未跨界（write_idx > read_idx）
[____RRRRRRRRRW_____]
     ↑        ↑
   read      write
   直接复制一段

情况2：跨界（write_idx < read_idx）
[WWWW_________RRRRRR]
    ↑         ↑
  write      read
  复制两段：[read→end] + [0→write]
```

## 保留功能
- ✅ LED命令解析（Process_Usart_Command）
- ✅ 命令就绪检测（is_command_ready）
- ✅ 接收字符串处理（Usart1_Receive_String）
- ✅ DMA接收（IDLE中断）
- ✅ 所有原有API接口

## 注意事项

1. **主循环必须调用uart_tx_task()**
   - 确保缓冲区数据及时发送
   - 建议放在主循环开始位置

2. **缓冲区满处理策略**
   - 当前策略：等待直到有空间
   - 可修改为：丢弃数据（改while为if return）

3. **临界区保护**
   - 写入环形缓冲区时使用__disable_irq()/__enable_irq()
   - 防止中断导致的数据竞争

4. **监控缓冲区**
   - 使用uart_get_tx_buf_usage()检查缓冲区使用情况
   - 避免缓冲区溢出

## 测试建议

```c
// 测试1：连续快速打印
for(int i = 0; i < 100; i++) {
    printf("Line %d: Test message\r\n", i);
}

// 测试2：监控缓冲区
printf("Buffer usage: %d/%d\r\n", 
       uart_get_tx_buf_usage(), 
       UART_TX_BUF_SIZE);

// 测试3：混合场景
while(1) {
    uart_tx_task();
    Process_Usart_Command();
    
    if(some_event) {
        printf("Event timestamp: %u\r\n", get_systick());
    }
    
    delay_ms(10);
}
```

## 重要修复：Usart1_Send_String 冲突问题

### 问题说明
升级后发现 `Usart1_Send_String` 有时失灵，原因：
1. **DMA配置不一致**：DMA初始化使用 `uart_dma_tx_buf`，但旧代码往 `tx_buffer` 写数据
2. **双系统冲突**：环形缓冲区和直接发送两套系统同时操作DMA会互相干扰
3. **数据错乱**：DMA从错误地址读取数据，导致发送失败或乱码

### 解决方案
已将 `Usart1_Send_String()` 和 `Usart1_Send_DMA()` 改为使用环形缓冲区：

```c
// 现在统一走环形缓冲区
void Usart1_Send_String(char *string)
{
    printf("%s", string);  // 简单高效
}

void Usart1_Send_DMA(uint8_t *data, uint16_t len)
{
    // 写入环形缓冲区，避免冲突
    __disable_irq();
    for (uint16_t i = 0; i < len; i++) {
        while (ringbuf_free_space() < 1) {
            __enable_irq();
            uart_start_dma_transfer();
            __disable_irq();
        }
        tx_ring_buf[tx_write_idx] = data[i];
        tx_write_idx = (tx_write_idx + 1) % UART_TX_BUF_SIZE;
    }
    __enable_irq();
    
    if (!tx_busy) {
        uart_start_dma_transfer();
    }
}
```

### 优势
- ✅ 所有发送函数统一使用环形缓冲区，无冲突
- ✅ 保持API兼容性，旧代码无需修改
- ✅ 解决发送失灵和数据错乱问题

## 升级完成
✅ 发送机制已成功升级为环形缓冲区实现
✅ 修复旧函数冲突问题
✅ 保留所有原有功能
✅ 提升系统实时性和吞吐量
