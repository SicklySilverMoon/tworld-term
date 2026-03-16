#ifndef TWORLD_TERM_LEVELSET_H
#define TWORLD_TERM_LEVELSET_H

#include "formats.h"

typedef struct TWLevelSet {
    LevelSet* level_set;
    struct TWLevelScore* scores;
} TWLevelSet;

typedef struct OptionalTWLevelSet {
    bool has_value;
    TWLevelSet value;
} OptionalTWLevelSet;
OptionalTWLevelSet TWLevelSet_new(char const* path);
void TWLevelSet_free(TWLevelSet* self);

#endif //TWORLD_TERM_LEVELSET_H
