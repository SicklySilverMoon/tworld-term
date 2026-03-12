#include "menubar.h"

#include <string.h>

#include "graphics.h"
#include "misc.h"

WINDOW* menu_bar;

size_t menu_selection;

#define BREAK ((void*) 0x01)

int init_menubar() {
    int width = getmaxx(stdscr);
    menu_bar = newwin(1, width, 0, 0);
    if (!menu_bar) {
        return -1;
    }
    wbkgd(menu_bar, COLOUR_PAIR(0) | A_REVERSE); //default user colours
    werase(menu_bar);
    mvwprintw(menu_bar, 0, 0, " File (F1)  Level (F2)  Options (F3)  Solution (F4)  Help (F5) ");
    wnoutrefresh(menu_bar);

    return 0;
}

WINDOW* open_menu(char const* entries[], size_t num_entries, int x_offset) {
    int width = 0;
    int height = 0;

    for (size_t i = 0; i < num_entries; i++) {
        height += 1;
        if (entries[i] == BREAK)
            continue;
        int len = strlen(entries[i]);
        if (len > width)
            width = len;
    }

    width += 2;

    WINDOW* win = create_window(x_offset, 0, width + 2, height + 2, true);

    for (size_t i = 0; i < num_entries; i++) {
        if (entries[i] != BREAK) {
            mvwprintw(win, i + 1, 1, "%s", entries[i]);
        } else {
            // char buf[MB_CUR_MAX * 2];
            // c32_to_mb(buf, U'─');
            for (int j = 0; j < width; j++) {
                mvwaddch(win, i + 1, j + 1, ACS_HLINE);
            }
        }
    }

    wrefresh(win);

    return win;
}

static void draw_file_menu(bool in_gameplay) {
    char const* entries[] = {
        "Scores",
        "Solution Files",
        BREAK,
        "Leave Game",
    };
}

static void draw_level_menu() {
    char const* entries[] = {
        "Start",
        "(Un)pause",
        "Restart",
        BREAK,
        "Next",
        "Previous",
        "Go To",
        BREAK,
        "View Prologue",
        "View Epilogue",
    };
}

static void draw_options_menu() {
    char const* entries[] = {
        "Graphics",
        "Controls",
        BREAK,
    };
}

static void draw_solution_menu() {
    char const* entries[] = {
        "Play",
        "Verify",
        BREAK,
        "Replace",
        "Delete",
    };
}

static void draw_help_menu() {

}
