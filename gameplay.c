#include "gameplay.h"

#include <curses.h>
#include <string.h>

#include "formats.h"
#include "logic.h"

#include "misc.h"

LevelSet* level_set;
LevelMetadata* level_metadata;
Level* level;

WINDOW* level_win;
WINDOW* info_win;

int init_gameplay() {
    size_t size;
    uint8_t* data = read_file("/home/v/Documents/Chip's/DATs/CCLP1.dat", &size);
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

    int width, height;
    getmaxyx(stdscr, height, width);

    level_win = create_window(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, false);
    info_win = create_window(GAME_WINDOW_WIDTH, 0, width - GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, true);

    return 0;
}

void render_gameplay(Level const* level, LevelMetadata const* meta, WINDOW* level_win, WINDOW* info_win) {
    int w_level, h_level, w_info, h_info;
    getmaxyx(level_win, h_level, w_level);
    getmaxyx(info_win, h_info, w_info);

    int title_len = strlen(meta->title);
    int title_x = (w_info - title_len) / 2;
    // int title_y = (h_info - 1) / 2;
    int title_y = 1;
    mvwprintw(info_win, title_y, title_x, "%s", meta->title);

    wrefresh(level_win);
    wrefresh(info_win);
}

void render_gameplay_BAD() {
    render_gameplay(level, level_metadata, level_win, info_win);
}
