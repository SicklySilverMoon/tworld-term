#include "gameplay.h"

#include <string.h>
#include <math.h>
#include <curses.h>

#include "formats.h"
#include "logic.h"

#include "misc.h"
#include "graphics.h"

LevelSet* level_set;
LevelMetadata* level_metadata;
Level* level;

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
    level_metadata = LevelSet_get_level(level_set, 0);
    Result_LevelPtr res_level = LevelMetadata_make_level(level_metadata, &ms_logic);
    if (!res_level.success) {
        eprintf("%s\n", res_level.error);
        free(res_level.error);
        return -1;
    }
    level = res_level.value;

    return 0;
}

void gameplay_loop() {
    render_gameplay(level, level_metadata);

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
        Level_set_game_input(level, game_input);
        Level_tick(level);
        Level_set_game_input(level, DIRECTION_NIL);
        Level_tick(level);
        Level_tick(level);
        Level_tick(level);

        render_gameplay(level, level_metadata);
    }
}
