不能使用引用

#include <stdio.h>
#include<stdlib.h>
void turn(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;
}
int main()
{
    int a = 10, b = 57;
    turn(a, b);
    printf("a=%d,b=%d\n", a, b);
}