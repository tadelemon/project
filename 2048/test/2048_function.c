/* 
*Copyright (c) 2018,likeSmile Group.
*All rights reserved. 
*
*文件名：2048_function.c
*摘要：2048游戏所需要的函数定义
*
*当前版本：1.0.0
*作者：tadelemon
*完成日期：2018.9.8
*
*取代版本号：0.0.0
*原作者: shiyanlou
*完成日期：
*/
#ifndef _2048_FUNCTION_C
#define _2048_FUNCTION_C

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include "2048.h" //引入2048的函数原型

//游戏界面为一个 4x4 的 16 宫格，使用二维数组进行表示，用 0 表示空格
int a[4][4] = {0}; //将16宫格全部初始化为空格
int empty;         //空格数
int old_x, old_y;  //原坐标

void init()
{
    int x, y;
    initscr();   //初始化curses库和tty
    cbreak();    //读取除delete和ctrl等控制字外的字符
    noecho();    //用户输入不回显
    curs_set(0); // 0 将光标设为不可见, 1 为可见

    srand(time(0)); //目标数随机生成位置
    x = rand() % 4;
    y = rand() % 4;
    a[x][y] = 2;
    empty = 15; //当前空格子数目
    draw();     //绘制图形
}

void draw()
{
    int n, m, x, y; //n,x表示行；m,y表示列
    int flag = 0;

    clear(); //清屏
    //绘制边框
    if (flag == 0)
    {

        for (n = 0; n < 9; n += 2)
        {
            for (m = 0; m < 21; m++)
            {
                move(n, m);
                addch('-');
            }
        }
        for (m = 0; m < 21; m += 5)
        {
            for (n = 1; n < 8; n++)
            {
                move(n, m);
                addch('|');
            }
        }
        flag = 1;
    }
    refresh(); //刷新输出；

    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            draw_one(x, y);
        }
    }
    refresh();
}

void draw_one(int x, int y)
{
    int i, m, k, j;
    char c[5] = {0};
    i = a[x][y];
    m = 0;
    while (i > 0)
    {
        j = i % 10;
        c[m++] = j + '0'; //进行字符转换
        i /= 10;
    }
    m = 0;
    //矩阵位置与视图位置的映射
    k = (y + 1) * 5 - 1; //第一个字符的列输出位置
    while (c[m] != 0)
    {
        mvaddch(2 * x + 1, k, c[m++]);
        k--;
    }
}

int left_move()
{
    int x, y, i;
    int move = 0;
    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4;)
        {
            if (a[x][y] == 0) //防止更新的y被加一
            {
                y++;
            }
            else
            {
                for (i = y + 1; i < 4; i++)
                {
                    if (a[x][i] != 0)
                    {
                        if (a[x][i] == a[x][y])
                        {
                            a[x][y] += a[x][i];
                            a[x][i] = 0;
                            empty++;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }

                y = i; //这一步检查后面的值是否可加
            }
        }
    }
    for (x = 0; x < 4; x++) //检查数据前（左）是否有空格，若有则左移
    {
        for (y = 0; y < 4; y++)
        {
            if (a[x][y] != 0)
            {
                for (i = y - 1; (i > -1) && (a[x][i] == 0); i--)
                {
                    a[x][i] = a[x][i + 1];
                    a[x][i + 1] = 0;
                    move = 1; //标志位置 1 ，表明有移动。
                }
            }
        }
    }

    return move;
}

int right_move()
{
    int x, y, i;
    int move = 0;
    for (x = 0; x < 4; x++)
    {
        for (y = 3; y > -1;)
        {
            if (a[x][y] == 0)
            {
                y--;
            }
            else
            {
                for (i = y - 1; i > -1; i--)
                {
                    if (a[x][i] == 0)
                    {
                        continue;
                    }
                    else if (a[x][y] == a[x][i])
                    {
                        a[x][y] += a[x][i];
                        a[x][i] = 0;
                        empty++;
                        break;
                    }
                    else
                    {
                        break;
                    }
                }

                y = i;
            }
        }
    }
    for (x = 0; x < 4; x++)
    {
        for (y = 3; y > -1; y--)
        {
            if (a[x][y] != 0)
            {
                for (i = y + 1; (i < 4) && (a[x][i] == 0); i++)
                {
                    a[x][i] = a[x][i - 1];
                    a[x][i - 1] = 0;
                    move = 1;
                }
            }
        }
    }
    return move;
}

int up_move()
{
    int x, y, i;
    int move = 0;
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4;)
        {
            if (a[x][y] == 0)
            {
                x++;
            }
            else
            {
                for (i = x + 1; i < 4; i++)
                {
                    if (a[i][y] != 0)
                    {
                        if (a[i][y] == a[x][y])
                        {
                            a[x][y] += a[i][y];
                            a[i][y] = 0;
                            empty++;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                x = i;
            }
        }
    }
    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            if (a[x][y] != 0)
            {
                for (i = x - 1; (i > -1) && (a[i][y] == 0); i--)
                {
                    a[i][y] = a[i + 1][y];
                    a[i + 1][y] = 0;
                    move = 1;
                }
            }
        }
    }
    return move;
}

int down_move()
{
    int x, y, i;
    int move = 0;
    for (y = 0; y < 4; y++)
    {
        for (x = 3; x > -1;)
        {
            if (a[x][y] == 0)
            {
                x++;
            }
            else
            {
                for (i = x - 1; i > -1; i--)
                {
                    if (a[i][y] != 0)
                    {
                        if (a[i][y] == a[x][y])
                        {
                            a[x][y] += a[i][y];
                            a[i][y] = 0;
                            empty++;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                x = i;
            }
        }
    }
    for (y = 0; y < 4; y++)
    {
        for (x = 3; x > -1; x--)
        {
            if (a[x][y] != 0)
            {
                for (i = x + 1; (i < 4) && (a[i][y] == 0); i++)
                {
                    a[i][y] = a[i - 1][y];
                    a[i - 1][y] = 0;
                    move = 1;
                }
            }
        }
    }
    return move;
}
void play()
{
    int new_x, new_y, temp;
    int old_empty, move;
    char ch;

    while (1)
    {
        move = 0;
        old_empty = empty;
        ch = getch();

        switch (ch)
        {
        case 97: //左移键
        case 104:
        case 68:
            move = left_move();
            break;
        case 100: //右移键
        case 108:
        case 67:
            move = right_move();
            break;
        case 119: //上移键
        case 107:
        case 65:
            move = up_move();
            break;
        case 115: //下移键
        case 106:
        case 66:
            move = down_move();
            break;
        case 'Q':
        case 'q':
            game_over();
            break;
        default:
            continue;
            break;
        }

        if (empty <= 0) //检查是否有空格
            game_over();
        if ((empty != old_empty) || (move == 1)) //若有空格变化或数据移动
        {
            do
            {
                new_x = rand() % 4;
                new_y = rand() % 4;
            } while (a[new_x][new_y] != 0); //生成一个随机位置

            cnt_value(&new_x, &new_y);
            do
            {
                temp = rand() % 4;
            } while (temp == 0 || temp == 2);
            a[new_x][new_y] = temp + 1; //给该随机位置赋初值 2 or 4
            empty--;                    //空格变化
        }
        draw();
    }
}

int cnt_one(int x, int y) //统计xy周围空格数
{
    int value = 0;

    if (x - 1 > -1)
        a[x - 1][y] ? 0 : value++;
    if (x + 1 < 4)
        a[x + 1][y] ? 0 : value++;
    if (y - 1 > -1)
        a[x][y - 1] ? 0 : value++;
    if (y + 1 < 4)
        a[x][y + 1] ? 0 : value++;
    if (x - 1 > -1 && y - 1 > -1)
        a[x - 1][y - 1] ? 0 : value++;
    if (x - 1 > -1 && y + 1 < 4)
        a[x - 1][y + 1] ? 0 : value++;
    if (x + 1 < 4 && y - 1 > -1)
        a[x + 1][y - 1] ? 0 : value++;
    if (x + 1 < 4 && y + 1 < 4)
        a[x + 1][y + 1] ? 0 : value++;

    return value;
}

void cnt_value(int *new_x, int *new_y) //挑选最佳数据生成位置
{
    int x, y, value;
    int max = 0;
    max = cnt_one(*new_x, *new_y);

    for (x = 0; x < 4; x++)
    {
        for (y = 0; y < 4; y++)
        {
            if (a[x][y] == 0)
            {
                value = cnt_one(x, y);
                if (value > max && (x != old_x || y != old_y))
                {
                    *new_x = x;
                    *new_y = y;
                    old_x = x;
                    old_y = y;
                    max = value;
                }
                if (max == 8)
                    return;
            }
        }
    }
}

int game_over() //结束游戏
{
    sleep(1);
    endwin();
    exit(0);
}

int main(int argc, char const *argv[])
{
    init();
    play();
    endwin();

    return 0;
}

#endif