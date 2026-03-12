#ifndef TWORLD_TERM_LEVEL_H
#define TWORLD_TERM_LEVEL_H

#include "formats.h"
#include "format-tws.h"

typedef struct TWLevel {
    LevelMetadata* metadata;
    Level* level;
    TWSMetadata* tws_metadata;
    GameInputList inputs;
    GameInput cur_input;
    bool started;
    bool playing_solution;
    bool paused;
} TWLevel;

int TWLevel_new(TWLevel* self, LevelSet* level_set, uint16_t level_idx);
void TWLevel_free(TWLevel* self);
void TWLevel_set_input(TWLevel* self, GameInput input);
void TWLevel_tick(TWLevel* self);

#endif //TWORLD_TERM_LEVEL_H
