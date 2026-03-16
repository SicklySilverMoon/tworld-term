#ifndef TWORLD_TERM_GAMEPLAY_H
#define TWORLD_TERM_GAMEPLAY_H

#include <curses.h>

#include "formats.h"

int init_gameplay();
void gameplay_tick(int key);

#endif //TWORLD_TERM_GAMEPLAY_H
