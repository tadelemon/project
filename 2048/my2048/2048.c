/* 
*Copyright (c) 2018,innYouth Ltd. 
*All rights reserved. 
*
*文件名：tadelemon
*摘要：2048小游戏，该版本升级了是否重新开始游戏，做了2048检查,避免使用全局变量增加了程序的稳定性
*强调数据生成位置的随机性，而不强调位置周围空格更多
*
*当前版本：1.1
*作者：tadelemon
*完成日期：
*
*取代版本号：1.0
*原作者: tadelemon
*完成日期：
*/

#include <curses.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define x_grid 4
#define y_grid 4

int init_game(int a[x_grid][y_grid]); //初始化函数

char play_game(int a[x_grid][y_grid], int empty);  //游戏主逻辑
int left_move(int a[x_grid][y_grid], int *empty);  //左移逻辑
int right_move(int a[x_grid][y_grid], int *empty); //右移逻辑
int up_move(int a[x_grid][y_grid], int *empty);    //上移逻辑
int down_move(int a[x_grid][y_grid], int *empty);  //下移逻辑

char draw_game(int a[x_grid][y_grid]); //图形绘制
char draw_num(int num, int x, int y);  //数字绘制

void cnt_location(int a[x_grid][y_grid], int *x, int *y); //选择新数据生成位置

char game_over(int flag); //游戏推出逻辑

int main(int argc, char const *argv[])
{
    char flag = 'n';
    do
    {
        int a[x_grid][y_grid] = {0};
        int empty = init_game(a);
        flag = play_game(a, empty);
    } while (flag == 'Y' || flag == 'y');
    endwin(); //关闭curses库并重置tty

    return 0;
}




int init_game(int a[x_grid][y_grid])
{
    initscr();   //初始化curses库
    cbreak();    //除控制字外，全为可输入字符
    noecho();    //输入不回显
    curs_set(0); //不回显光标

    srand(time(0));          //设置随机数种子
    int x = rand() % x_grid; //初始化数据
    int y = rand() % y_grid;
    a[x][y] = 2;

    draw_game(a);
    return x_grid * y_grid - 1; //返回空格数
}

char draw_game(int a[x_grid][y_grid])
{
    char flag = 'n';
    clear(); //清除屏幕内容：这意味着每一次程序周期都将重绘所有内容
    //绘制边框
    for (int x = 0; x < 2 * x_grid + 1; x += 2)
    {
        for (int y = 0; y < 5 * y_grid + 1; y++)
        {
            mvaddch(x, y, '-');
        }
    }
    for (int y = 0; y < 5 * y_grid + 1; y += 5)
    {
        for (int x = 1; x < 2 * x_grid; x++)
        {
            mvaddch(x, y, '|');
        }
    }
    refresh(); //刷新输出
    //绘制数组所有非零数据
    for (int x = 0; x < x_grid; x++)
    {
        for (int y = 0; y < y_grid; y++)
        {
            if (a[x][y] != 0)
            {
                flag = draw_num(a[x][y], x, y);
            }
        }
    }
    refresh();   //刷新输出
    return flag; //若遇到2048返回其状态值
}
char draw_num(int num, int x, int y)
{
    char m[4] = {0}; //将字符数组全置为'\0',即空字符
    char flag = 'n'; //标定为非2048状态值
    int n = num;
    //将数字转换为字符存入字符数组，为倒序
    for (int i = 0; n != 0; i++)
    {
        m[i] = n % 10 + '0';
        n /= 10;
    }
    //计算字符绘制位置
    int j = (x + 1) * 2 - 1;
    int k = (y + 1) * 5 - 1;
    for (int i = 0; m[i] != 0; i++, k--)
    {
        mvaddch(j, k, m[i]);
    }
    //检查数据是否为2048
    if (num == 2048)
    {
        flag = 'y';
    }
    return flag;
}

char play_game(int a[x_grid][y_grid], int empty)
{
    int move = 0, old_empty, x, y, value;
    char input_key, flag, c_flag;
    while (1)
    {
        old_empty = empty;
        input_key = getch(); //接受输入
        switch (input_key)
        {
        case 97:  //'a'
        case 104: //'h'
        case 68:  //'left'
            move = left_move(a, &empty);
            break;
        case 100: //'d'
        case 108: //'l'
        case 67:  //'right'
            move = right_move(a, &empty);
            break;
        case 119: //'w'
        case 107: //'k'
        case 65:  //'up'
            move = up_move(a, &empty);
            break;
        case 115: //'s'
        case 106: //'j'
        case 66:  //'down'
            move = down_move(a, &empty);
            break;
        case 'q': //退出
        case 'Q':
            flag = game_over(2);
            return flag;
        default:
            continue;
        }
        if (empty <= 0) //判断是否有空格
        {
            flag = game_over(0);
            return flag;
        }
        if (old_empty != empty || move != 0) //判断是否发生变化
        {
            cnt_location(a, &x, &y); //选择生成位置
            do                       //生成一个 2/4 的随机值
            {
                value = rand() % 4;
            } while (value == 0 || value == 2);
            a[x][y] = value + 1;
            empty--; //格子状态变化
        }

        c_flag = draw_game(a);

        if (c_flag == 'y')
        {
            flag = game_over(1);
            return flag;
        }
        else
        {
            continue;
        }
    }
}

char game_over(int flag)
{
    char i = 'n';

    clear();
    if (flag == 0)
    {
        mvprintw(3, 10, "Game over !");
        refresh();
        sleep(1);
        clear();
        mvprintw(3, 10, "Start game again?[Y/N]");
        refresh();
        i = getch();
    }
    else if (flag == 1)
    {
        mvprintw(3, 10, "Wow ! You have won!");
        refresh();
        sleep(1);
        clear();
        mvprintw(3, 10, "Start game again?[Y/N]");
        refresh();
        i = getch();
    }
    else
    {
        mvprintw(3, 10, " Quit game !");
        refresh();
        sleep(1);
    }
    return i;
}

int left_move(int a[x_grid][y_grid], int *empty)
{
    int i = 0, move = 0;
    //相加
    for (int x = 0; x < x_grid; x++)
    {
        for (int y = 0; y < y_grid;)
        {
            if (a[x][y] == 0)
            {
                y++;
            }
            else
            {
                for (i = y + 1; i < y_grid; i++)
                {
                    if (a[x][i] != 0)
                    {
                        if (a[x][y] == a[x][i])
                        {
                            a[x][y] += a[x][i];
                            a[x][i] = 0;
                            (*empty)++;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                y = i;
            }
        }
    }
    //去除空格
    for (int x = 0; x < x_grid; x++)
    {
        for (int y = 0; y < y_grid; y++)
        {
            if (a[x][y] != 0)
            {
                for (i = y - 1; i > -1; i--)
                {
                    if (a[x][i] == 0)
                    {
                        a[x][i] = a[x][i + 1];
                        a[x][i + 1] = 0;
                        move = 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return move;
}
int right_move(int a[x_grid][y_grid], int *empty)
{
    int i = 0, move = 0;
    for (int x = 0; x < x_grid; x++)
    {
        for (int y = y_grid - 1; y > -1;)
        {
            if (a[x][y] == 0)
            {
                y--;
            }
            else
            {
                for (i = y - 1; i > -1; i--)
                {
                    if (a[x][i] != 0)
                    {
                        if (a[x][i] == a[x][y])
                        {
                            a[x][y] += a[x][i];
                            a[x][i] = 0;
                            (*empty)++;
                            break;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                y = i;
            }
        }
    }

    for (int x = 0; x < x_grid; x++)
    {
        for (int y = y_grid - 1; y > -1; y--)
        {
            if (a[x][y] != 0)
            {
                for (i = y + 1; i < y_grid; i++)
                {
                    if (a[x][i] == 0)
                    {
                        a[x][i] = a[x][i - 1];
                        a[x][i - 1] = 0;
                        move = 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return move;
}
int up_move(int a[x_grid][y_grid], int *empty)
{
    int i = 0, move = 0;
    for (int y = 0; y < y_grid; y++)
    {
        for (int x = 0; x < x_grid;)
        {
            if (a[x][y] == 0)
            {
                x++;
            }
            else
            {
                for (i = x + 1; i < x_grid; i++)
                {
                    if (a[i][y] != 0)
                    {
                        if (a[i][y] == a[x][y])
                        {
                            a[x][y] += a[i][y];
                            a[i][y] = 0;
                            (*empty)++;
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
    for (int y = 0; y < y_grid; y++)
    {
        for (int x = 0; x < x_grid; x++)
        {
            if (a[x][y] != 0)
            {
                for (i = x - 1; i > -1; i--)
                {
                    if (a[i][y] == 0)
                    {
                        a[i][y] = a[i + 1][y];
                        a[i + 1][y] = 0;
                        move = 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return move;
}
int down_move(int a[x_grid][y_grid], int *empty)
{
    int i = 0, move = 0;
    for (int y = 0; y < y_grid; y++)
    {
        for (int x = x_grid; x > -1;)
        {
            if (a[x][y] == 0)
            {
                x--;
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
                            (*empty)++;
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
    for (int y = 0; y < y_grid; y++)
    {
        for (int x = x_grid; x > -1; x--)
        {
            if (a[x][y] != 0)
            {
                for (i = x + 1; i < x_grid; i++)
                {
                    if (a[i][y] == 0)
                    {
                        a[i][y] = a[i - 1][y];
                        a[i - 1][y] = 0;
                        move = 1;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    return move;
}
void cnt_location(int a[x_grid][y_grid], int *x, int *y)
{
    do //生成随机函数
    {
        *x = rand() % x_grid;
        *y = rand() % y_grid;
    } while (a[*x][*y] != 0);
}
