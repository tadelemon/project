#include<curses.h>
int a[4][4]={0};
void draw_one(int x, int y)
{
    int i, m, k, j;
    char c[5] = {0};
    i = a[x][y];
    m = 0;
    while (i > 0)
    {
        j = i % 10;
        c[m++] = j + '0';
        i /= 10;
    }
    m = 0;
    k = (y + 1) * 5 - 1;
    while (c[m] != 0)
    {
        mvaddch(2 * x + 1, k, c[m++]);
        k--;
    }
    refresh();
}
int main(int argc, char const *argv[])
{
    a[2][3]=1232;
    draw_one(2,3);

    return 0;
}
