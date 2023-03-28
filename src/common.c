#include "common.h"


/* ceiling round x to multiple of a */
int32_t round_to(int32_t x, int32_t a) {
    return ((x / a) + 1) * a;
}
