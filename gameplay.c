#include "gameplay.h"

#include <string.h>
#include <math.h>
#include <curses.h>

#include "formats.h"
#include "logic.h"

#include "chips/level.h"
#include "misc.h"
#include "graphics.h"
#include "chips/levelset.h"

TWLevelSet level_set;
TWLevel level = {0};

int init_gameplay() {
    OptionalTWLevelSet opt = TWLevelSet_new("./CCLP1.ccl");
    if (!opt.has_value) {
        return -1;
    }
    level_set = opt.value;
    if (TWLevel_new(&level, &level_set, 0) != 0) {
        TWLevelSet_free(&level_set);
        return -1;
    }
    render_gameplay(&level);
    return 0;
}

void gameplay_tick(int key) {
    // GameInput game_input = DIRECTION_NIL;
    if (key == KEY_LEFT) {
        TWLevel_set_input(&level, DIRECTION_WEST);
    } else if (key == KEY_RIGHT) {
        TWLevel_set_input(&level, DIRECTION_EAST);
    } else if (key == KEY_UP) {
        TWLevel_set_input(&level, DIRECTION_NORTH);
    } else if (key == KEY_DOWN) {
        TWLevel_set_input(&level, DIRECTION_SOUTH);
    }
    TWLevel_tick(&level);
    render_gameplay(&level);
}
