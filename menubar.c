#include "menubar.h"

#include "graphics.h"

WINDOW* menu_bar;

int init_menubar() {
    int width = getmaxx(stdscr);
    menu_bar = newwin(1, width, 0, 0);
    if (!menu_bar) {
        return -1;
    }
    wbkgd(menu_bar, COLOUR_PAIR(0) | A_REVERSE); //default user colours
    werase(menu_bar);
    mvwprintw(menu_bar, 0, 0, " File  Level  Options  Solution  Help ");
    wnoutrefresh(menu_bar);

    return 0;
}
