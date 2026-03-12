#include "gameplay.h"

#include <string.h>
#include <math.h>
#include <curses.h>

#include "formats.h"
#include "logic.h"

#include "chips/level.h"
#include "misc.h"
#include "graphics.h"

LevelSet* level_set = NULL;
TWLevel level = {0};

bool paused = false;
bool started = false;
GameInput game_input = DIRECTION_NIL;

int init_gameplay() {
    size_t size;
    uint8_t* data = read_file("./CCLP1.ccl", &size);
    Result_LevelSetPtr res = parse_ccl(data, size);
    free(data);
    if (!res.success) {
        eprintf("%s\n", res.error);
        free(res.error);
        return -1;
    }
    level_set = res.value;
    if (TWLevel_new(&level, level_set, 0) != 0) {
        LevelSet_free(level_set);
        level_set = NULL;
        return -1;
    }
    return 0;
}

void gameplay_loop() {
    render_gameplay(&level);

    while (true) {
        int key = game_get_key(); //wait for key press
        if (key == ERR)
            continue;
        GameInput game_input = DIRECTION_NIL;
        if (key == KEY_LEFT) {
            game_input = DIRECTION_WEST;
        } else if (key == KEY_RIGHT) {
            game_input = DIRECTION_EAST;
        } else if (key == KEY_UP) {
            game_input = DIRECTION_NORTH;
        } else if (key == KEY_DOWN) {
            game_input = DIRECTION_SOUTH;
        }
        TWLevel_set_input(&level, game_input);
        TWLevel_tick(&level);
        TWLevel_tick(&level);
        TWLevel_tick(&level);
        TWLevel_tick(&level);

        render_gameplay(&level);
    }
}
