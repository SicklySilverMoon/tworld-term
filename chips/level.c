#include "level.h"

#include <stdlib.h>
#include <string.h>

#include "format-tws.h"

int TWLevel_new(TWLevel* self, TWLevelSet* level_set, uint16_t level_idx) {
    if (!self || !level_set)
        return -1;
    TWLevel_free(self);
    self->metadata = LevelSet_get_level(level_set->level_set, 0);
    Result_LevelPtr res_level = LevelMetadata_make_level(self->metadata, &ms_logic);
    if (!res_level.success) {
        eprintf("%s\n", res_level.error);
        free(res_level.error);
        TWLevel_free(self);
        return -1;
    }
    self->level = res_level.value;
    return 0;
}

void TWLevel_free(TWLevel* self) {
    if (self->level)
        Level_free(self->level);
    if (self->tws_metadata)
        TWSMetadata_free(self->tws_metadata);
    GameInputList_free(&self->best_score.inputs);
    GameInputList_free(&self->current_score.inputs);
    memset(self, 0, sizeof(TWLevel));
}

void TWLevel_pause(TWLevel* self) {
    self->paused = true;
}

void TWLevel_unpause(TWLevel* self) {
    self->paused = false;
}

void TWLevel_set_input(TWLevel* self, GameInput input) {
    if (self->playing_solution || self->paused)
        return;

    if (input != DIRECTION_NIL) {
        self->started = true;
    }
    if (!self->started)
        return;
    GameInputList_append(&self->current_score.inputs, input);
    self->cur_input = input;
}

void TWLevel_tick(TWLevel* self) {
    if (!self->started || self->paused)
        return;
    uint32_t tick = Level_get_current_tick(self->level);
    if (Level_get_win_state(self->level) != TRIRES_NOTHING)
        return;
    if (self->playing_solution) {
        Level_set_game_input(self->level, GameInputList_get_input(&self->current_score.inputs, tick));
    } else {
        Level_set_game_input(self->level, self->cur_input);
        if (self->current_score.inputs.count < tick + 1) { //If the user didn't enter an input, throw in whatever the current one is (likely NIL)
            GameInputList_append(&self->current_score.inputs, self->cur_input);
        }
    }
    Level_tick(self->level);
    self->cur_input = DIRECTION_NIL;
}
