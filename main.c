#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <curses.h>

#include "formats.h"
#include "gameplay.h"

#include "graphics.h"

int main(void) {
    setlocale(LC_ALL, "");

    initscr();	// Init the library.
    noecho();	// Do not echo user input.
    cbreak();	// Do not buffer user input, retain Ctrl-Z & Ctrl-C functions.
    //raw(); 	// Do nut buffer any user input. Present all input to program.
    keypad(stdscr, TRUE); // Enable extended character (e.g. F-keys, numpad) input.
    wtimeout(stdscr, 0); // set key press timeout to none
    curs_set(0); // Change cursor appearance. 0 invisible, 1 normal, 2 strong.

    if (has_colors() == FALSE) {
        endwin();
    	eprintf("Your terminal doesn't support colours\n");
        return 1;
    }
    start_color();
    use_default_colors();
    if (init_graphics()) {
        return 1;
    }

    // printw("COLORS=%d\n", COLORS);
    // printw("COLOR_PAIRS=%d\n", COLOR_PAIRS);
    //
    // for (int i = 0; i < COLORS && i < 16; i++) {
    //     init_pair(i, i, COLOR_BLACK);
    //     attron(COLOR_PAIR(i));
    //     printw("Color %d\n", i);
    //     attroff(COLOR_PAIR(i));
    // }
    // refresh();
    // getch();

    init_gameplay_graphics();

    touchwin(stdscr); //force graphical updates to the entire system
    refresh();

    init_gameplay();
    gameplay_loop();

    endwin();	// Main ncurses clean-up.

    return 0;
}
