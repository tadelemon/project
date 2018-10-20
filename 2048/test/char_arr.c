#include <stdio.h>
int main(int argc, char const *argv[])
{
    char arr[4] = {0}; //不能被单个字符初始化，但可以被整 0 或者 0x00
                       //初始化为符号 '\0' 也就是数字 0
    arr[2] = getchar();
    printf("%c   %d\n", arr[2], arr[0]);

    return 0;
}