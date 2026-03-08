#ifndef TWORLD_TERM_GAMEPLAY_H
#define TWORLD_TERM_GAMEPLAY_H

#include <curses.h>

#include "formats.h"

#define GAME_WINDOW_WIDTH 9
#define GAME_WINDOW_HEIGHT 9

int init_gameplay();
void render_gameplay(Level const* level, LevelMetadata const* meta, WINDOW* level_win, WINDOW* info_win);
void render_gameplay_BAD();

#endif //TWORLD_TERM_GAMEPLAY_H