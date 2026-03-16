#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <curses.h>

#include "formats.h"
#include "gameplay.h"

#include "graphics.h"

typedef enum TWState {
    TW_STATE_MAIN_MENU,
    TW_STATE_GAME,
    TW_STATE_MENUBAR,
} TWState;

TWState state = TW_STATE_GAME;
TWState prior_state = TW_STATE_MAIN_MENU;

int get_key() {
    return wgetch(stdscr);
}

int main(void) {
    setlocale(LC_ALL, "");

    initscr();	// Init the library.
    noecho();	// Do not echo user input.
    cbreak();	// Do not buffer user input, retain Ctrl-Z & Ctrl-C functions.
    //raw(); 	// Do nut buffer any user input. Present all input to program.
    keypad(stdscr, TRUE); // Enable extended character (e.g. F-keys, numpad) input.
    wtimeout(stdscr, 55); // set key press timeout to 55ms, roughly emulating TW's 1.1 real seconds to MS' 1 in game sec
    curs_set(0); // Change cursor appearance. 0 invisible, 1 normal, 2 strong.

    if (has_colors() == FALSE) {
        endwin();
    	eprintf("Your terminal doesn't support colours\n");
        return 1;
    }
    start_color();
    use_default_colors();
    refresh(); //force a refresh BEFORE other things

    if (init_graphics()) {
        return 1;
    }
    init_gameplay();
    while (true) {
        int key = get_key();
        if (key == 'q') {
            break;
        }
        if (key >= KEY_F0 && key <= KEY_F(64)) {
            if (state == TW_STATE_MENUBAR) {
                state = prior_state;
            } else {
                prior_state = state;
                state = TW_STATE_MENUBAR;
            }
        } else if (state == TW_STATE_MAIN_MENU) {
            //todo: create a menu file and call into
        } else if (state == TW_STATE_MENUBAR) {
            //todo: call into the menubar
        } else if (state == TW_STATE_GAME) {
            gameplay_tick(key);
        }
    }

    endwin();	// Main ncurses clean-up.

    return 0;
}
