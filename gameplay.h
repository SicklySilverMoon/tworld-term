#ifndef TWORLD_TERM_GAMEPLAY_H
#define TWORLD_TERM_GAMEPLAY_H

#include <curses.h>

#include "formats.h"

#define GAME_WINDOW_WIDTH 9
#define GAME_WINDOW_HEIGHT 9

int init_gameplay();
void gameplay_loop();

#endif //TWORLD_TERM_GAMEPLAY_H