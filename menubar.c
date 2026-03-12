#include "menubar.h"

#include <string.h>

#include "graphics.h"
#include "misc.h"

WINDOW* menu_bar;
WINDOW* dropdown_menu;

int menu_selection;
int dropdown_menu_selection;

#define BREAK ((void*) 0x01)

void draw_menu_bar();

int init_menubar() {
    int width = getmaxx(stdscr);
    menu_bar = newwin(1, width, 0, 0);
    if (!menu_bar) {
        return -1;
    }
    draw_menu_bar();
    return 0;
}

void draw_menu_bar() {
    char const* options[] = {
        " File (F1) ",
        " Level (F2) ",
        " Options (F3) ",
        " Solution (F4) ",
        " Help (F5) ",
    };
    if (!menu_bar)
        return;

    wbkgd(menu_bar, COLOUR_PAIR(0) | A_REVERSE); //default user colours
    werase(menu_bar);

    int current_pos = 0;
    for (int i = 0; i < (sizeof(options) / sizeof(options[0])); i++) {
        if (i == menu_selection)
            attroff(A_REVERSE);
        mvwprintw(menu_bar, 0, current_pos, "%s", options[i]);
        if (i == menu_selection)
            attron(A_REVERSE);
        current_pos += strlen(options[i]);
    }

    wnoutrefresh(menu_bar);
}

void draw_dropdown_menu(char const* entries[], int num_entries) {
    if (!dropdown_menu)
        return;

    int width = 0;
    for (int i = 0; i < num_entries; i++) {
        if (entries[i] == BREAK)
            continue;
        int len = strlen(entries[i]);
        if (len > width) {
            width = len;
        }
    }

    for (int i = 0; i < num_entries; i++) {
        if (entries[i] != BREAK) {
            if (i == dropdown_menu_selection) {
                wattron(dropdown_menu, A_REVERSE);
            }
            mvwprintw(dropdown_menu, i + 1, 1, "%s", entries[i]);
            if (i == dropdown_menu_selection) {
                wattroff(dropdown_menu, A_REVERSE);
            }
        } else {
            // char buf[MB_CUR_MAX * 2];
            // c32_to_mb(buf, U'─');
            for (int j = 0; j < width; j++) {
                mvwaddch(dropdown_menu, i + 1, j + 1, ACS_HLINE);
            }
        }
    }

    wrefresh(dropdown_menu);
}

void open_dropdown_menu(char const* entries[], int num_entries, int x_offset) {
    int width = 0;
    int height = 0;
    for (int i = 0; i < num_entries; i++) {
        height += 1;
        if (entries[i] == BREAK)
            continue;
        int len = strlen(entries[i]);
        if (len > width) {
            width = len;
        }
    }

    dropdown_menu = create_window(x_offset, 0, width + 2, height + 2, true);
    if (!dropdown_menu)
        return;
    draw_dropdown_menu(entries, num_entries);
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
