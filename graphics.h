#ifndef TWORLD_TERM_GRAPHICS_H
#define TWORLD_TERM_GRAPHICS_H

#include <uchar.h>
#include <curses.h>
#include <menu.h>

#include "formats.h"

#include "chips/level.h"

#define NCURSES_COLOUR_T NCURSES_COLOR_T
#define COLOUR_PAIR(p) COLOR_PAIR(p)
#define COLOURS COLORS
#define COLOUR_PAIRS COLOR_PAIRS

#define COLOUR_BLACK          COLOR_BLACK
#define COLOUR_RED            COLOR_RED
#define COLOUR_GREEN          COLOR_GREEN
#define COLOUR_YELLOW         COLOR_YELLOW
#define COLOUR_BLUE           COLOR_BLUE
#define COLOUR_MAGENTA        COLOR_MAGENTA
#define COLOUR_CYAN           COLOR_CYAN
#define COLOUR_WHITE          COLOR_WHITE

#define COLOUR_BRIGHT_BLACK   8
#define COLOUR_BRIGHT_RED     9
#define COLOUR_BRIGHT_GREEN   10
#define COLOUR_BRIGHT_YELLOW  11
#define COLOUR_BRIGHT_BLUE    12
#define COLOUR_BRIGHT_MAGENTA 13
#define COLOUR_BRIGHT_CYAN    14
#define COLOUR_BRIGHT_WHITE   15

#define GAME_WINDOW_WIDTH 9
#define GAME_WINDOW_HEIGHT 9

typedef struct TileGraphic {
    char32_t tile_char; //lets us use any possible character, although it will be thrown through a c32rtomb
    short colour;
} TileGraphic;

short combine_colours(NCURSES_COLOUR_T top, NCURSES_COLOUR_T bottom);
void split_colours(short combined, NCURSES_COLOUR_T* top, NCURSES_COLOUR_T* bottom);

int init_graphics();
int init_gameplay_graphics();
TileGraphic get_graphic(TileID top, TileID bottom);
void render_gameplay(TWLevel const* level);

#endif //TWORLD_TERM_GRAPHICS_H
