#include "common.h"

#define INIT_ALLOCATE_BLOCK_SIZE 10
#define INIT_DEALLOCATE_BLOCK_SIZE 20


typedef struct vec {
    void *data;
    /* const */ size_t item_size;
    size_t size, reserved_size;
    size_t allocate_block_size, deallocate_block_size;
} vec_t;


void vec_create(vec_t *vec, size_t item_size);

void vec_create_zero(vec_t *vec, size_t item_size, size_t num);

void vec_create_set(vec_t *vec, size_t item_size, size_t num, const void *init_value);

void vec_zero(vec_t *vec);

/* only allocates the space */
void vec_reserve(vec_t *vec, size_t new_size);

/* actually modifies vec.size */
void vec_resize(vec_t *vec, size_t new_size);

void *vec_get(const vec_t *vec, size_t index);

void vec_get_copy(const vec_t *vec, size_t index, void *dest);

void vec_set(vec_t *vec, size_t index, const void *value);

void vec_push(vec_t *vec, const void *value);

/* places in dest OR pass NULL to not place */
void vec_pop(vec_t *vec, void *dest);

void vec_erase(vec_t *vec, size_t begin_index, size_t end_index);

void vec_clear(vec_t *vec);

void vec_dbg(const vec_t *vec, FILE *out, void (*print_func)(const void *value, FILE *out));

void vec_destroy(vec_t *vec);
