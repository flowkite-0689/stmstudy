#include "2048_oled.h"

#define SIZE 4

int board[SIZE][SIZE];
int score = 0;

// 新增数字的逻辑
void addNum()
{
  int empty[SIZE * SIZE][2];
  int count = 0;
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 0)
      {
        empty[count][0] = i;
        empty[count][1] = j;
        count++;
      }
    }
  }
  if (count)
  {
    int index = rand() % count;
    int num = (rand() % 10 < 9) ? 2 : 4;
    board[empty[index][0]][empty[index][1]] = num;
  }
}

// 初始化逻辑
void init()
{

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      board[i][j] = 0;
    }
  }
  addNum();
  addNum();
}

// 数字移动逻辑
int move(int direction)
{
  /*
  0 左
  1 右
  2 上
  3 下
  */

  int he[SIZE * SIZE / 2][2]; // 已合并的位置
  int count = 0;
  int moved = 0;      // 判断有无移动
  if (direction == 0) // 左移
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = 1; j < SIZE; j++)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          while (k > 0 && board[i][k - 1] == 0)
          {
            board[i][k - 1] = board[i][k];
            board[i][k] = 0;
            k--;
            moved = 1;
          }
          if (k > 0 && board[i][k - 1] == board[i][k])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k - 1))
              {
                flag = 1; // 判断已合并过
              }
            }
            if (!flag)
            {
              board[i][k - 1] *= 2;
              he[count][0] = i;
              he[count][1] = (k - 1);
              count++;
              score += board[i][k - 1];
              board[i][k] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  else if (direction == 1) // 右移
  {
    for (int i = 0; i < SIZE; i++)
    {
      for (int j = SIZE - 2; j >= 0; j--)
      {
        if (board[i][j] != 0)
        {
          int k = j;
          while (k < SIZE - 1 && board[i][k + 1] == 0)
          {
            board[i][k + 1] = board[i][k];
            board[i][k] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE - 1 && board[i][k + 1] == board[i][k])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == i && he[z][1] == (k + 1))
              {
                flag = 1; // 判断已合并过
              }
            }
            if (!flag)
            {
              board[i][k + 1] *= 2;
              score += board[i][k + 1];
              he[count][0] = i;
              he[count][1] = (k + 1);
              count++;
              board[i][k] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  else if (direction == 2) // 上移
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = 1; i < SIZE; i++)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          while (k > 0 && board[k - 1][j] == 0)
          {
            board[k - 1][j] = board[k][j];
            board[k][j] = 0;
            k--;
            moved = 1;
          }
          if (k > 0 && board[k - 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k - 1) && he[z][1] == j)
              {
                flag = 1; // 判断已合并过
              }
            }
            if (!flag)
            {
              board[k - 1][j] *= 2;
              score += board[k - 1][j];
              he[count][0] = (k - 1);
              he[count][1] = j;
              count++;
              board[k][j] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  else if (direction == 3) // 下移
  {
    for (int j = 0; j < SIZE; j++)
    {
      for (int i = SIZE - 2; i >= 0; i--)
      {
        if (board[i][j] != 0)
        {
          int k = i;
          while (k < SIZE - 1 && board[k + 1][j] == 0)
          {
            board[k + 1][j] = board[k][j];
            board[k][j] = 0;
            k++;
            moved = 1;
          }
          if (k < SIZE - 1 && board[k + 1][j] == board[k][j])
          {
            int flag = 0;
            for (int z = 0; z < count; z++)
            {
              if (he[z][0] == (k + 1) && he[z][1] == j)
              {
                flag = 1; // 判断已合并过
              }
            }
            if (!flag)
            {
              board[k + 1][j] *= 2;
              score += board[k + 1][j];
              he[count][0] = (k + 1);
              he[count][1] = j;
              count++;
              board[k][j] = 0;
              moved = 1;
            }
          }
        }
      }
    }
  }
  return moved;
}

// 游戏结束的逻辑
int isGameover()
{

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i][j] == 2048)
      {
        printf("合成了2048\nWe are the champion!!!\n");
        return 1;
      }
    }
  }
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {

      if (!board[i][j])
      {
        return 0;
      }
    }
  }
  for (int i = 0; i < SIZE - 1; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (board[i + 1][j] == board[i][j])
      {
        return 0;
      }
    }
  }

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE - 1; j++)
    {
      if (board[i][j + 1] == board[i][j])
      {
        return 0;
      }
    }
  }

  return 1;
}

// 靠mpu6050的体感来操控方向的方法


//游戏运行界面

//打印棋盘
void printBoard()
{
  for (int i = 0; i < SIZE; i++)
  {
    if (i==0)
    {
        OLED_Printf_Line(i,"%c  %c  %c  %cscore:%d",
      board[i][0]?(board[i][0]+'0'):'.',
    board[i][1]?(board[i][1]+'0'):'.',
  board[i][2]?(board[i][2]+'0'):'.',
board[i][3]?(board[i][3]+'0'):'.',score);
    }
    else
    {
    OLED_Printf_Line(i,"%c  %c  %c  %c",
      board[i][0]?(board[i][0]+'0'):'.',
    board[i][1]?(board[i][1]+'0'):'.',
  board[i][2]?(board[i][2]+'0'):'.',
board[i][3]?(board[i][3]+'0'):'.');
  }}
  OLED_Refresh_Dirty();
}




//
void game_running_2048()
{
 init();
 u8 key ;
 while (1)
 {
  
  printBoard();
  key =KEY_Get();
  if (key)
  {
    switch (key)
    {
    case KEY2_PRES:
      return;
    
    default:
      break;
    }
  }
  
 }
 
}


// 游戏菜单界面
// 选项
char *menu_2048_opt[] = {
    "start"};
u8 last_page_2048 = 99;
#define SHOWING_NUM 4
#define m_TOTAL_ITEMS (sizeof(menu_2048_opt) / sizeof(menu_2048_opt[0]))
void menu_2048_oled_RE(u8 selected)
{
  u8 page = selected / SHOWING_NUM;
  if (last_page_2048 != page)
  {
    OLED_Clear();
    last_page_2048 = page;
  }
  // 当前在第几页（从0开始）
  u8 start_idx = page * SHOWING_NUM;              // 本页第一个选项的索引
  u8 items_this_page = m_TOTAL_ITEMS - start_idx; // 本页实际有多少项
  if (items_this_page > SHOWING_NUM)
    items_this_page = SHOWING_NUM;

  for (u8 i = 0; i < items_this_page; i++)
  {
    u8 current_idx = start_idx + i; // 实际在总列表中的索引

    // 判断是否是当前选中项
    char arrow = (current_idx == selected) ? '>' : ' ';

    // 显示这一行（假设第0行开始显示菜单）
    OLED_Printf_Line(i, "%c %s", arrow, menu_2048_opt[current_idx]);
  }

  // 如果本页不足4行，下面几行可以清空或留空
  for (u8 i = items_this_page; i < SHOWING_NUM; i++)
  {
    OLED_Clear_Line(i); // 8个空格清行
  }

  OLED_Refresh_Dirty();
}
// 进入到所选
void menu_2048_enter_select(u8 selected)
{
  switch (selected)
  {
  case 0:
    game_running_2048();
    break;

  default:
    break;
  }
}

//游戏菜单主函数
void menu_2048_oled()
{
  u8 flag_Re = 1;
  u8 key;
  u8 selected = 0;
  while (1)
  {
    if (flag_Re)
    {
      menu_2048_oled_RE(selected);
      flag_Re = 0;
    }

    delay_ms(10);
    key = KEY_Get();
    if (key)
    {
      switch (key)
      {
      case KEY3_PRES:
        flag_Re = 1;
        menu_2048_enter_select(selected);
        break;
      case KEY2_PRES:
        OLED_Clear();
        return;
      default:
        break;
      }
    }
  }
}

