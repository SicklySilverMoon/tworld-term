#ifndef TWORLD_TERM_LEVEL_H
#define TWORLD_TERM_LEVEL_H

#include "formats.h"
#include "format-tws.h"

#include "levelset.h"

typedef struct TWLevelScore {
    bool unlocked;
    uint32_t best_time;
    Direction rff_dir;
    int8_t init_step_parity;
    uint64_t prng_seed;
    GameInputList inputs;
} TWLevelScore;

typedef struct TWLevel {
    LevelMetadata* metadata;
    Level* level;
    TWSMetadata* tws_metadata;
    GameInput cur_input;
    bool started;
    bool playing_solution;
    bool paused;
    TWLevelScore best_score;
    TWLevelScore current_score;
} TWLevel;

int TWLevel_new(TWLevel* self, TWLevelSet* level_set, uint16_t level_idx);
void TWLevel_free(TWLevel* self);
void TWLevel_pause(TWLevel* self);
void TWLevel_unpause(TWLevel* self);
void TWLevel_set_input(TWLevel* self, GameInput input);
void TWLevel_tick(TWLevel* self);

#endif //TWORLD_TERM_LEVEL_H
