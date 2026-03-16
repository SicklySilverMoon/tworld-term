#include "graphics.h"

#include <string.h>

#include "menubar.h"
#include "misc.h"

WINDOW* level_win;
WINDOW* info_win;

short combine_colours(NCURSES_COLOUR_T top, NCURSES_COLOUR_T bottom) {
    return top * 16 + bottom + 1;
}

void split_colours(short combined, NCURSES_COLOUR_T* top, NCURSES_COLOUR_T* bottom) {
    *top = (combined - 1) / 16;
    *bottom = (combined - 1) % 16;
}

int init_graphics() {
    if (COLOURS < 16) {
        endwin();
        eprintf("Your terminal doesn't support bright colours\n");
        return 1;
    }
    if (COLOUR_PAIRS < 256) {
        endwin();
        eprintf("Terminal doesn't support enough colour pairs\n");
        return 1;
    }

    for (NCURSES_COLOUR_T top = 0; top < 16; top += 1) {
        for (NCURSES_COLOUR_T bot = 0; bot < 16; bot += 1) {
            init_pair(combine_colours(top, bot), top, bot);
        }
    }

    if (init_menubar() != 0) {
        return -1;
    }
    if (init_gameplay_graphics() != 0) {
        return -1;
    }

    doupdate();
    return 0;
}

int init_gameplay_graphics() {
    int width, height;
    getmaxyx(stdscr, height, width);

    level_win = create_window(0, 0, GAME_WINDOW_WIDTH + 2, GAME_WINDOW_HEIGHT + 2, true);
    if (!level_win) {
        return -1;
    }
    keypad(level_win, TRUE);
    // wtimeout(level_win, 0);
    // nodelay(level_win, TRUE);

    info_win = create_window(GAME_WINDOW_WIDTH + 2, 0, width - (GAME_WINDOW_WIDTH + 2), GAME_WINDOW_HEIGHT + 2, true);
    if (!info_win) {
        return -1;
    }
    keypad(info_win, TRUE);
    // wtimeout(info_win, 0);
    // nodelay(info_win, TRUE);

    doupdate();
    return 0;
}

static TileGraphic get_single_tile_graphic(TileID tile) {
    if (TileID_is_actor(tile)) {
        tile = TileID_actor_get_id(tile);
    }
    switch (tile) {
        default:
        case Nothing:
        case Empty:
            return (TileGraphic){U' ', combine_colours(COLOUR_WHITE, COLOUR_WHITE)};
        case Wall:
        case Overlay_Buffer:
        case Floor_Reserved2:
        case Floor_Reserved1:
        case Floor_Final:
            return (TileGraphic){U'#', combine_colours(COLOUR_WHITE, COLOUR_BRIGHT_BLACK)};
        case Water:
            return (TileGraphic){U'≈', combine_colours(COLOUR_BRIGHT_WHITE, COLOUR_BLUE)};
        case Fire:
            return (TileGraphic){U'☼', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_BRIGHT_RED)};
        case Slide_North:
            return (TileGraphic){U'▲', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_GREEN)};
        case Slide_West:
            return (TileGraphic){U'◄', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_GREEN)};
        case Slide_South:
            return (TileGraphic){U'▼', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_GREEN)};
        case Slide_East:
            return (TileGraphic){U'►', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_GREEN)};
        case Slide_Random:
            return (TileGraphic){U'↕', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_GREEN)};
        case Ice:
            return (TileGraphic){U'≡', combine_colours(COLOUR_BRIGHT_WHITE, COLOUR_BRIGHT_CYAN)};
        case IceWall_Northwest:
            return (TileGraphic){U'┘', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_CYAN)};
        case IceWall_Northeast:
            return (TileGraphic){U'└', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_CYAN)};
        case IceWall_Southwest:
            return (TileGraphic){U'┐', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_CYAN)};
        case IceWall_Southeast:
            return (TileGraphic){U'┌', combine_colours(COLOUR_BLACK, COLOUR_BRIGHT_CYAN)};
        case Gravel:
            return (TileGraphic){U'░', combine_colours(COLOUR_WHITE, COLOUR_BLACK)};
        case Dirt:
            return (TileGraphic){U'▒', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_RED)};
        case Bomb:
            return (TileGraphic){U'♂', combine_colours(COLOUR_RED, COLOUR_WHITE)};
        case Beartrap:
            return (TileGraphic){U'◙', combine_colours(COLOUR_YELLOW, COLOUR_WHITE)};
        case Burglar:
            return (TileGraphic){U'☻', combine_colours(COLOUR_CYAN, COLOUR_WHITE)};
        case HintButton:
            return (TileGraphic){U'?', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_WHITE)};
        case Button_Blue:
            return (TileGraphic){U'•', combine_colours(COLOUR_BLUE, COLOUR_WHITE)};
        case Button_Green:
            return (TileGraphic){U'•', combine_colours(COLOUR_GREEN, COLOUR_WHITE)};
        case Button_Red:
            return (TileGraphic){U'•', combine_colours(COLOUR_BRIGHT_RED, COLOUR_WHITE)};
        case Button_Brown:
            return (TileGraphic){U'•', combine_colours(COLOUR_YELLOW, COLOUR_WHITE)};
        case Teleport:
            return (TileGraphic){U'◙', combine_colours(COLOUR_BRIGHT_CYAN, COLOUR_WHITE)};
        case Wall_North:
            return (TileGraphic){U'▀', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Wall_West:
            return (TileGraphic){U'▐', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Wall_South:
            return (TileGraphic){U'▄', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Wall_East:
            return (TileGraphic){U'▌', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Wall_Southeast:
            return (TileGraphic){U'└', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case HiddenWall_Perm:
            return (TileGraphic){U' ', combine_colours(COLOUR_WHITE, COLOUR_WHITE)};
        case HiddenWall_Temp:
            return (TileGraphic){U' ', combine_colours(COLOUR_WHITE, COLOUR_WHITE)};
        case BlueWall_Real:
        case BlueWall_Fake:
            return (TileGraphic){U'#', combine_colours(COLOUR_BRIGHT_CYAN, COLOUR_CYAN)};
        case SwitchWall_Open:
            return (TileGraphic){U'◘', combine_colours(COLOUR_GREEN, COLOUR_WHITE)};
        case SwitchWall_Closed:
            return (TileGraphic){U'◘', combine_colours(COLOUR_GREEN, COLOUR_BRIGHT_BLACK)};
        case PopupWall:
            return (TileGraphic){U'◙', combine_colours(COLOUR_BRIGHT_BLACK, COLOUR_WHITE)};
        case CloneMachine:
            return (TileGraphic){U'+', combine_colours(COLOUR_BRIGHT_BLACK, COLOUR_BLACK)};
        case Door_Red:
            return (TileGraphic){U'♥', combine_colours(COLOUR_BRIGHT_RED, COLOUR_BRIGHT_BLACK)};
            // return (TileGraphic){U'D', combine_colours(COLOUR_BRIGHT_RED, COLOUR_BRIGHT_BLACK)};
        case Door_Blue:
            return (TileGraphic){U'♠', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_BLACK)};
            // return (TileGraphic){U'D', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_BLACK)};
        case Door_Yellow:
            return (TileGraphic){U'♦', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_BRIGHT_BLACK)};
            // return (TileGraphic){U'D', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_BRIGHT_BLACK)};
        case Door_Green:
            return (TileGraphic){U'♣', combine_colours(COLOUR_BRIGHT_GREEN, COLOUR_BRIGHT_BLACK)};
            // return (TileGraphic){U'D', combine_colours(COLOUR_BRIGHT_GREEN, COLOUR_BRIGHT_BLACK)};
        case Socket:
            return (TileGraphic){U'§', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_WHITE)};
        case Exit:
            return (TileGraphic){U'╬', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_CYAN)};
        case ICChip:
            return (TileGraphic){U'¶', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
            // return (TileGraphic){U'$', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Key_Red:
            return (TileGraphic){U'♥', combine_colours(COLOUR_BRIGHT_RED, COLOUR_WHITE)};
            // return (TileGraphic){U'K', combine_colours(COLOUR_BRIGHT_RED, COLOUR_WHITE)};
        case Key_Blue:
            return (TileGraphic){U'♠', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_WHITE)};
            // return (TileGraphic){U'K', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_WHITE)};
        case Key_Yellow:
            return (TileGraphic){U'♦', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_WHITE)};
            // return (TileGraphic){U'K', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_WHITE)};
        case Key_Green:
            return (TileGraphic){U'♣', combine_colours(COLOUR_BRIGHT_GREEN, COLOUR_WHITE)};
            // return (TileGraphic){U'K', combine_colours(COLOUR_BRIGHT_GREEN, COLOUR_WHITE)};
        case Boots_Ice:
            return (TileGraphic){U'b', combine_colours(COLOUR_CYAN, COLOUR_WHITE)};
        case Boots_Slide:
            return (TileGraphic){U'b', combine_colours(COLOUR_GREEN, COLOUR_WHITE)};
        case Boots_Fire:
            return (TileGraphic){U'b', combine_colours(COLOUR_RED, COLOUR_WHITE)};
        case Boots_Water:
            return (TileGraphic){U'b', combine_colours(COLOUR_BLUE, COLOUR_WHITE)};
        case Drowned_Chip:
            return (TileGraphic){U'*', combine_colours(COLOUR_BRIGHT_WHITE, COLOUR_BLUE)};
        case Burned_Chip:
            return (TileGraphic){U'*', combine_colours(COLOUR_BRIGHT_BLACK, COLOUR_BRIGHT_RED)};
        case Bombed_Chip:
            return (TileGraphic){U'*', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Exited_Chip:
            return (TileGraphic){U'C', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_CYAN)};
        case Exit_Extra_1:
            return (TileGraphic){U'╫', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_CYAN)};
        case Exit_Extra_2:
            return (TileGraphic){U'╪', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_BRIGHT_CYAN)};
        case Chip:
        case Pushing_Chip:
        case Swimming_Chip:
            return (TileGraphic){U'C', combine_colours(COLOUR_BRIGHT_GREEN, COLOUR_WHITE)};
        case Block:
        case Block_Static:
            return (TileGraphic){U'█', combine_colours(COLOUR_YELLOW, COLOUR_WHITE)};
        case Tank:
            return (TileGraphic){U'π', combine_colours(COLOUR_BLUE, COLOUR_WHITE)};
        case Ball:
            return (TileGraphic){U'O', combine_colours(COLOUR_BRIGHT_MAGENTA, COLOUR_WHITE)};
        case Glider:
            return (TileGraphic){U'¥', combine_colours(COLOUR_BRIGHT_BLUE, COLOUR_WHITE)};
        case Fireball:
            return (TileGraphic){U'&', combine_colours(COLOUR_BRIGHT_RED, COLOUR_WHITE)};
        case Walker:
            return (TileGraphic){U'Φ', combine_colours(COLOUR_BRIGHT_CYAN, COLOUR_WHITE)};
        case Blob:
            return (TileGraphic){U'ß', combine_colours(COLOUR_GREEN, COLOUR_WHITE)};
        case Teeth:
            return (TileGraphic){U'Σ', combine_colours(COLOUR_RED, COLOUR_WHITE)};
        case Bug:
            return (TileGraphic){U'δ', combine_colours(COLOUR_BRIGHT_YELLOW, COLOUR_WHITE)};
        case Paramecium:
            return (TileGraphic){U'ε', combine_colours(COLOUR_YELLOW, COLOUR_WHITE)};
        case Entity_Reserved2:
        case Entity_Reserved1:
        case Entity_Last:
            return (TileGraphic){U'*', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
        case Water_Splash:
            return (TileGraphic){U'*', combine_colours(COLOUR_WHITE, COLOUR_BLUE)};
        case Bomb_Explosion:
            return (TileGraphic){U'*', combine_colours(COLOUR_RED, COLOUR_WHITE)};
        case Entity_Explosion:
            return (TileGraphic){U'*', combine_colours(COLOUR_MAGENTA, COLOUR_WHITE)};
        case Animation_Reserved1:
            return (TileGraphic){U'*', combine_colours(COLOUR_BLACK, COLOUR_WHITE)};
    }
    return (TileGraphic){U' ', combine_colours(COLOUR_BLACK, COLOUR_BLACK)};
}

TileGraphic get_graphic(TileID top_tile, TileID bottom_tile) {
    TileGraphic top = get_single_tile_graphic(top_tile);
    TileGraphic bottom = get_single_tile_graphic(bottom_tile);

    if ((TileID_is_actor(top_tile) && TileID_actor_get_id(top_tile) != Block) || TileID_is_key(top_tile) || TileID_is_boots(top_tile)) {
        TileGraphic result = top;
        NCURSES_COLOUR_T top_top, top_bottom, bottom_top, bottom_bottom;
        split_colours(top.colour, &top_top, &top_bottom);
        split_colours(bottom.colour, &bottom_top, &bottom_bottom);
        result.colour = combine_colours(top_top, bottom_bottom);
        return result;
    }
    return top;
}

static void get_grid(Level const* level, TileID grid[GAME_WINDOW_HEIGHT][GAME_WINDOW_WIDTH][2]) {
    Position chip_pos = Actor_get_position(Level_get_chip_actor(level));
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

static void render_info(TWLevel const* level) {
    box(info_win, 0, 0);

    int w_info, h_info;
    getmaxyx(info_win, h_info, w_info);

    if (level->metadata->title) {
        int title_len = strlen(level->metadata->title);
        int title_x = (w_info - title_len) / 2;
        mvwprintw(info_win, 1, title_x, "%s", level->metadata->title);
    }

    char const chips_label[] = "CHIPS";
    size_t chips_label_len = sizeof(chips_label) - 1;
    int chips_x = w_info / 4 - chips_label_len / 2;
    mvwprintw(info_win, 3, chips_x, "%s", chips_label);
    mvwprintw(info_win, 4, chips_x, "%-5zu", (size_t)Level_get_chips_left(level->level));

    char const time_label[] = "TIME";
    size_t time_label_len = sizeof(time_label) - 1;
    int time_x = (w_info * 3) / 4 - time_label_len / 2;
    mvwprintw(info_win, 3, time_x, "%s", time_label);
    int64_t time = Level_get_time_limit(level->level);
    if (time != 0) {
        time = (time - Level_get_current_tick(level->level)) / 20 + Level_get_time_offset(level->level);
    }
    if (time < 0) {
        time = 0;
    }
    mvwprintw(info_win, 4, time_x, "%-4zu", time);

    char const keys_label[] = "KEYS";
    size_t keys_label_len = sizeof(keys_label) - 1;
    int keys_x = w_info / 4 - keys_label_len / 2;
    mvwprintw(info_win, 6, keys_x, "%s", keys_label);
    uint8_t* keys = Level_get_player_keys(level->level);
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
        wattron(info_win, COLOUR_PAIR(graphic.colour));
        mvwaddstr(info_win, 7, keys_x + i, buf);
        wattroff(info_win, COLOUR_PAIR(graphic.colour));
    }

    char const boots_label[] = "BOOTS";
    size_t boots_label_len = sizeof(boots_label) - 1;
    int boots_x = (w_info * 3) / 4 - boots_label_len / 2;
    mvwprintw(info_win, 6, boots_x, "%s", boots_label);
    uint8_t* boots = Level_get_player_boots(level->level);
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
        wattron(info_win, COLOUR_PAIR(graphic.colour));
        mvwaddstr(info_win, 7, boots_x + i, buf);
        wattroff(info_win, COLOUR_PAIR(graphic.colour));
    }

    if (level->metadata->author) {
        int author_len = strlen(level->metadata->author);
        int author_x = (w_info - author_len) / 2;
        mvwprintw(info_win, 9, author_x, "%s", level->metadata->author);
    }

    wnoutrefresh(info_win);
}

static void render_level(TWLevel const* level) {
    box(level_win, 0, 0);

    int w_level, h_level;
    getmaxyx(level_win, h_level, w_level);

    TileID grid[GAME_WINDOW_HEIGHT][GAME_WINDOW_WIDTH][2];
    get_grid(level->level, grid);

    for (int y = 0; y < GAME_WINDOW_HEIGHT; y++) {
        for (int x = 0; x < GAME_WINDOW_WIDTH; x++) {
            TileID top = grid[y][x][0];
            TileID bottom = grid[y][x][1];
            TileGraphic graphic = get_graphic(top, bottom);

            char buf[MB_CUR_MAX * 2];
            c32_to_mb(buf, graphic.tile_char);

            wattron(level_win, COLOUR_PAIR(graphic.colour));
            mvwaddstr(level_win, y + 1, x + 1, buf);
            wattroff(level_win, COLOUR_PAIR(graphic.colour));
        }
    }

    wnoutrefresh(level_win);
}

void render_gameplay(TWLevel const* level) {
    render_level(level);
    render_info(level);

    doupdate();
}
