# OLED模拟器

一个用于STM32 OLED显示屏开发的跨平台模拟器，基于SDL2实现。

## 项目结构

```
├── build/                  # 构建输出目录
├── docs/                   # 文档目录
├── examples/               # 示例代码
├── include/                # 头文件
│   ├── logo.h             # Logo图像数据
│   └── oledfont.h         # OLED字体数据
├── lib/                    # 库文件
├── src/                    # 源代码
│   ├── oled_simulator.c   # 基础模拟器
│   ├── oled_simulator_enhanced.c  # 增强模拟器
│   └── simple_test_image.c # 简单测试程序
├── assets/                 # 资源文件
├── CMakeLists.txt          # CMake构建配置
└── README.md               # 项目说明
```

## 功能特性

- **基础模拟器**: 提供OLED显示的基本功能
- **增强模拟器**: 支持所有OLED功能，包括：
  - 图像显示
  - 中文字符显示
  - 多种字体大小
  - 绘图功能
  - 滚动显示
  - 局部刷新

## 依赖项

- SDL2 (Simple DirectMedia Layer)
- CMake 3.10+
- GCC 或其他C99兼容编译器

## 编译和运行

### Ubuntu/Debian 安装依赖

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev cmake build-essential
```

### macOS 安装依赖

```bash
brew install sdl2 cmake
```

### 编译

```bash
mkdir build
cd build
cmake ..
make
```

### 运行

```bash
# 运行增强模拟器（推荐）
make run_enhanced

# 或运行基础模拟器
make run_basic

# 或运行简单测试
make run_test

# 或直接运行可执行文件
./bin/enhanced_simulator
./bin/basic_simulator
./bin/simple_test
```

## 使用说明

1. **启动程序**: 运行任意模拟器可执行文件
2. **查看显示**: 程序将显示一个128x64像素的OLED模拟窗口
3. **退出程序**: 点击窗口关闭按钮或按X键

## 开发说明

### 添加新功能

1. 在`src/`目录中创建新的源文件
2. 在`CMakeLists.txt`中添加新的可执行文件目标
3. 更新文档

### 调试技巧

- 使用`gdb`调试程序
- 检查构建输出中的错误信息
- 使用SDL环境变量控制显示行为

## 许可证

本项目遵循MIT许可证。

## 贡献

欢迎提交Issue和Pull Request来改进项目。

## 更新日志

- v1.0: 基础模拟器功能
- v2.0: 增强模拟器，支持完整OLED功能集
- v2.1: 重构项目结构，改进构建系统