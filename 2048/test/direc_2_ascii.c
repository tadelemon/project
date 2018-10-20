//有问题
#include<stdio.h>
#include<curses.h>
int main(int argc, char const *argv[])
{
    char ch1,ch2;
    printf("hello\n");
    ch1 =getchar();
    ch2= getchar();
    printf("%d  %d\n",ch1,ch2);
    return 0;
}
