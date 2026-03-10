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

WINDOW* level_win;
WINDOW* info_win;

int init_gameplay() {
    size_t size;
    uint8_t* data = read_file("/home/v/Documents/Chip's/DATs/CCLP1.ccl", &size);
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

    level_win = create_window(0, 0, GAME_WINDOW_WIDTH + 2, GAME_WINDOW_HEIGHT + 2, true);
    info_win = create_window(GAME_WINDOW_WIDTH + 2, 0, width - (GAME_WINDOW_WIDTH + 2), GAME_WINDOW_HEIGHT + 2, true);

    return 0;
}

static void c32_to_mb(char buf[MB_CUR_MAX * 2], char32_t c) { //MB_CUR_MAX * 2 can hold at minimum 2 chars
    mbstate_t state = {0};
    size_t written = c32rtomb(buf, c, &state);
    c32rtomb(buf + written, U'\0', &state); //NUL term, proper style
}

static void render_info(Level* level, LevelMetadata const* meta, WINDOW* info_win) {
    int w_info, h_info;
    getmaxyx(info_win, h_info, w_info);

    if (meta->title) {
        // wcolor_set(info_win, combine_colours(COLOUR_WHITE, COLOUR_RED), NULL);
        int title_len = strlen(meta->title);
        int title_x = (w_info - title_len) / 2;
        mvwprintw(info_win, 1, title_x, "%s", meta->title);
    }

    char const chips_label[] = "CHIPS";
    size_t chips_label_len = sizeof(chips_label) - 1;
    int chips_x = w_info / 4 - chips_label_len / 2;
    mvwprintw(info_win, 3, chips_x, "%s", chips_label);
    mvwprintw(info_win, 4, chips_x, "%-5zu", (size_t)Level_get_chips_left(level));

    char const time_label[] = "TIME";
    size_t time_label_len = sizeof(time_label) - 1;
    int time_x = (w_info * 3) / 4 - time_label_len / 2;
    mvwprintw(info_win, 3, time_x, "%s", time_label);
    size_t time = Level_get_time_limit(level);
    if (time != 0) {
        time = (time - Level_get_current_tick(level)) / 20 + Level_get_time_offset(level);
    }
    mvwprintw(info_win, 4, time_x, "%-4zu", time);

    char const keys_label[] = "KEYS";
    size_t keys_label_len = sizeof(keys_label) - 1;
    int keys_x = w_info / 4 - keys_label_len / 2;
    mvwprintw(info_win, 6, keys_x, "%s", keys_label);
    uint8_t* keys = Level_get_player_keys(level);
    for (size_t i = 0; i < 4; i++) {
        TileGraphic graphic;
        if (keys[i]) {
            if (i == 0) {
                graphic = get_graphic(Key_Red, Empty);
            } else if (i == 1) {
                graphic = get_graphic(Key_Blue, Empty);
            } else if (i == 2) {
                graphic = get_graphic(Key_Yellow, Empty);
            } else {
                graphic = get_graphic(Key_Green, Empty);
            }
        } else {
            graphic = get_graphic(Empty, Empty);
        }
        char buf[MB_CUR_MAX * 2];
        c32_to_mb(buf, graphic.tile_char);
        wattron(info_win, COLOR_PAIR(graphic.colour));
        mvwaddstr(info_win, 7, keys_x + i, buf);
        wattroff(info_win, COLOR_PAIR(graphic.colour));
    }

    char const boots_label[] = "BOOTS";
    size_t boots_label_len = sizeof(boots_label) - 1;
    int boots_x = (w_info * 3) / 4 - boots_label_len / 2;
    mvwprintw(info_win, 6, boots_x, "%s", boots_label);
    uint8_t* boots = Level_get_player_boots(level);
    for (size_t i = 0; i < 4; i++) {
        TileGraphic graphic;
        if (boots[i]) {
            if (i == 0) {
                graphic = get_graphic(Boots_Ice, Empty);
            } else if (i == 1) {
                graphic = get_graphic(Boots_Slide, Empty);
            } else if (i == 2) {
                graphic = get_graphic(Boots_Fire, Empty);
            } else {
                graphic = get_graphic(Boots_Water, Empty);
            }
        } else {
            graphic = get_graphic(Empty, Empty);
        }
        char buf[MB_CUR_MAX * 2];
        c32_to_mb(buf, graphic.tile_char);
        wattron(info_win, COLOR_PAIR(graphic.colour));
        mvwaddstr(info_win, 7, boots_x + i, buf);
        wattroff(info_win, COLOR_PAIR(graphic.colour));
    }

    if (meta->author) {
        int author_len = strlen(meta->author);
        int author_x = (w_info - author_len) / 2;
        mvwprintw(info_win, 9, author_x, "%s", meta->author);
    }

    wrefresh(info_win);
}

static void get_grid(TileID grid[GAME_WINDOW_HEIGHT][GAME_WINDOW_WIDTH][2]) {
    Position chip_pos = Level_get_actor_by_idx(level, 0)->pos;
    int xdisp = ((chip_pos % MAP_WIDTH) * 8)  / 2 - (GAME_WINDOW_WIDTH / 2) * 4; //Stolen straight from TW baby don't ask me what it does
    int ydisp = ((chip_pos / MAP_HEIGHT) * 8) / 2 - (GAME_WINDOW_HEIGHT / 2) * 4; //It just works
    if (xdisp < 0)
        xdisp = 0;
    if (ydisp < 0)
        ydisp = 0;
    if (xdisp > (MAP_WIDTH - GAME_WINDOW_WIDTH) * 4)
        xdisp = (MAP_WIDTH - GAME_WINDOW_WIDTH) * 4;
    if (ydisp > (MAP_HEIGHT - GAME_WINDOW_HEIGHT) * 4)
        ydisp = (MAP_HEIGHT - GAME_WINDOW_HEIGHT) * 4;
    int const lmap = xdisp / 4;
    int const tmap = ydisp / 4;
    int const rmap = (xdisp + 3) / 4 + GAME_WINDOW_WIDTH;
    int const bmap = (ydisp + 3) / 4 + GAME_WINDOW_HEIGHT;
    for (int y = tmap; y < bmap; ++y) {
        if (y < 0 || y >= MAP_HEIGHT)
            continue;
        for (int x = lmap; x < rmap; ++x) {
            if (x < 0 || x >= MAP_WIDTH)
                continue;
            Position map_pos = y * MAP_WIDTH + x;
            grid[y - tmap][x - lmap][0] = Level_get_top_terrain(level, map_pos);
            grid[y - tmap][x - lmap][1] = Level_get_bottom_terrain(level, map_pos);
        }
    }
}

static void render_level(Level const* level, WINDOW* level_win) {
    int w_level, h_level;
    getmaxyx(level_win, h_level, w_level);

    TileID grid[GAME_WINDOW_HEIGHT][GAME_WINDOW_WIDTH][2];
    get_grid(grid);

    for (int y = 0; y < GAME_WINDOW_HEIGHT; y++) {
        for (int x = 0; x < GAME_WINDOW_WIDTH; x++) {
            TileID top = grid[y][x][0];
            TileID bottom = grid[y][x][1];
            TileGraphic graphic = get_graphic(top, bottom);

            char buf[MB_CUR_MAX * 2];
            c32_to_mb(buf, graphic.tile_char);

            wattron(level_win, COLOR_PAIR(graphic.colour));
            mvwaddstr(level_win, y + 1, x + 1, buf);
            wattroff(level_win, COLOR_PAIR(graphic.colour));
        }
    }

    wrefresh(level_win);
}

void gameplay_loop() {
    render_level(level, level_win);
    render_info(level, level_metadata, info_win);

    while (true) {
        int key = wgetch(level_win); //wait for key press
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

        render_level(level, level_win);
        render_info(level, level_metadata, info_win);
    }
}
