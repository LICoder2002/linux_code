#include <ncurses.h>
#include <string.h>

int main()
{
    initscr();//初始化ncurses窗口
    raw();
    noecho();
    curs_set(0);
    const char* str = "My First Window";

    mvprintw(LINES/2, (COLS - strlen(str)) / 2  , str);
    refresh();

    getch();
    endwin();
    return 0;

}
