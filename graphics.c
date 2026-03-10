#include "graphics.h"

short combine_colours(NCURSES_COLOUR_T top, NCURSES_COLOUR_T bottom) {
    return top * 16 + bottom + 1;
}

void split_colours(short combined, NCURSES_COLOUR_T* top, NCURSES_COLOUR_T* bottom) {
    *top = (combined - 1) / 16;
    *bottom = (combined - 1) % 16;
}

int init_graphics() {
    if (COLORS < 16) {
        endwin();
        eprintf("Your terminal doesn't support bright colours\n");
        return 1;
    }
    if (COLOR_PAIRS < 257) {
        endwin();
        eprintf("Terminal doesn't support enough color pairs\n");
        return 1;
    }

    for (NCURSES_COLOUR_T top = 0; top < 16; top += 1) {
        for (NCURSES_COLOUR_T bot = 0; bot < 16; bot += 1) {
            init_pair(combine_colours(top, bot), top, bot);
        }
    }
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
