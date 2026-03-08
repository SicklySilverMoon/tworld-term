#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "formats.h"
#include "gameplay.h"

int main(void) {
    initscr();	// Init the library.
    noecho();	// Do not echo user input.
    cbreak();	// Do not buffer user input, retain Ctrl-Z & Ctrl-C functions.
    //raw(); 	// Do nut buffer any user input. Present all input to program.
    keypad(stdscr, TRUE); // Enable extended character (e.g. F-keys, numpad) input.
    curs_set(0); // Change cursor appearance. 0 invisible, 1 normal, 2 strong.

    if (has_colors() == FALSE) {
        endwin();
    	eprintf("Your terminal doesn't support colours");
        return 1;
    }

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // fore & background colours.

    attron(COLOR_PAIR(1));

    int y, x;
    getmaxyx(stdscr, y, x);
    y = y * 0.5;
    x = (x * 0.5) - 6;
    mvwprintw(stdscr, y, x, "Hello World!");
    mvwprintw(stdscr, 2, 2, "Hello World!");
    refresh();

    attroff(COLOR_PAIR(1));

    init_gameplay();
    render_gameplay_BAD();

    getch();	// Wait for key press before exiting.

    endwin();	// Main ncurses clean-up.

    return 0;
}
