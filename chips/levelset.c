#include "levelset.h"

#include <stdlib.h>
#include <string.h>

#include "level.h"
#include "misc.h"

OptionalTWLevelSet TWLevelSet_new(char const* path) {
    OptionalTWLevelSet opt = {0};
    opt.has_value = false;
    size_t size;
    uint8_t* data = read_file(path, &size);
    Result_LevelSetPtr res = parse_ccl(data, size);
    free(data);
    if (!res.success) {
        eprintf("%s\n", res.error);
        free(res.error);
        return opt;
    }
    opt.has_value = true;
    opt.value.level_set = res.value;
    opt.value.scores = calloc(opt.value.level_set->levels_n, sizeof(*opt.value.scores));
    // todo: load solutions (if any exist)
    return opt;
}

void TWLevelSet_free(TWLevelSet* self) {
    if (self->level_set)
        LevelSet_free(self->level_set);
    free(self->scores);
    memset(self, 0, sizeof(TWLevelSet));
}
