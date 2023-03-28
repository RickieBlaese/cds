#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define STR(...) #__VA_ARGS__
#define CLAMP(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))


/* ceiling round x to multiple of a */
int32_t round_to(int32_t x, int32_t a);
