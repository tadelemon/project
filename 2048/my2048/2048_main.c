#include"2048.h"
#include<curses.h>
int main(int argc, char const *argv[])
{
    init();
    play();
    endwin();

    return 0;
}
