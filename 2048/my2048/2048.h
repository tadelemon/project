/* 
*Copyright (c) 2018,likeSmile Group. 
*All rights reserved. 
*
*文件名：2048.h
*摘要：2048游戏所需要的函数声明
*
*当前版本：1.0.0
*作者：tadelemon
*完成日期：2018.9.8
*
*取代版本号：0.0.0
*原作者: shiyanlou
*完成日期：
*/ 

#ifndef _2048_H_
#define _2048_H_

void draw();    //用于绘制游戏界面

void play();    //游戏运行的逻辑主体
int left_move();    //左移
int right_move();   //右移
int up_move();      //上移
int down_move();    //下移

void init();    //初始化函数，用于完成一些必要的初始化操作
void draw_one(int x,int y); //绘制单个数字
void cnt_value(int *new_x,int *new_y);  //寻找最合适的数据生成位置
int cnt_one(int x,int y);   //统计 x,y 位置周围空格数目
int game_over();    //结束游戏

#endif