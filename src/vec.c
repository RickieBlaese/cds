#include "vec.h"

void vec_create(vec_t *vec, size_t item_size) {
    vec->data = NULL;
    vec->size = vec->reserved_size = 0;
    vec->allocate_block_size = INIT_ALLOCATE_BLOCK_SIZE;
    vec->deallocate_block_size = INIT_DEALLOCATE_BLOCK_SIZE;
    vec->item_size = item_size;
}

void vec_create_zero(vec_t *vec, size_t item_size, size_t num) {
    vec->data = calloc(num, item_size);
    vec->size = vec->reserved_size = num;
    vec->allocate_block_size = INIT_ALLOCATE_BLOCK_SIZE;
    vec->deallocate_block_size = INIT_DEALLOCATE_BLOCK_SIZE;
    vec->item_size = item_size;
}

void vec_create_set(vec_t *vec, size_t item_size, size_t num, const void *init_value) {
    vec->data = malloc(num * item_size);
    vec->size = vec->reserved_size = num;
    vec->allocate_block_size = INIT_ALLOCATE_BLOCK_SIZE;
    vec->deallocate_block_size = INIT_DEALLOCATE_BLOCK_SIZE;
    for (size_t i = 0; i < num; i++) {
        memcpy(vec->data + i * item_size, init_value, item_size);
    }
}

void vec_zero(vec_t *vec) {
    memset(vec, 0, vec->size * vec->item_size);
}

/* only allocates the space */
void vec_reserve(vec_t *vec, size_t new_size) {
    vec->data = realloc(vec->data, new_size * vec->item_size);
    vec->reserved_size = new_size;
}

/* actually modifies vec.size */
void vec_resize(vec_t *vec, size_t new_size) {
    vec_reserve(vec, new_size);
    vec->size = new_size;
}

void *vec_get(const vec_t *vec, size_t index) {
    if (index >= vec->size) {
        fprintf(stderr, "error: %s index %zu out of bounds (vec.size = %zu)\n", __func__, index, vec->size);
        exit(EXIT_FAILURE);
    }

    return vec->data + index * vec->item_size;
}

void vec_get_copy(const vec_t *vec, size_t index, void *dest) {
    memcpy(dest, vec_get(vec, index), vec->item_size);
}

void vec_set(vec_t *vec, size_t index, const void *value) {
    if (index >= vec->size) {
        fprintf(stderr, "error: %s index %zu out of bounds (vec.size = %zu)\n", __func__, index, vec->size);
        exit(EXIT_FAILURE);
    }

    memcpy(vec_get(vec, index), value, vec->item_size);
}

void vec_push(vec_t *vec, const void *value) {
    if (vec->size < vec->reserved_size) {
        vec_set(vec, vec->size++, value);
        return;
    }

    /* now we have to reallocate */
    vec_reserve(vec, vec->reserved_size + vec->allocate_block_size);
    vec->size++;
    vec_set(vec, vec->size - 1, value);
}

/* places in dest OR pass NULL to not */
void vec_pop(vec_t *vec, void *dest) {
    if (vec->size == 0) {
        fprintf(stderr, "error: %s on an empty vec (vec.reserved_size = %zu)\n", __func__, vec->reserved_size);
        exit(EXIT_FAILURE);
    }

    if (dest != NULL) {
        memcpy(dest, vec_get(vec, vec->size - 1), vec->item_size); /* save value */
    }

    if (vec->reserved_size - vec->size <= vec->deallocate_block_size) {
        vec->size--;
        return;
    }
    
    /* now we have to reallocate */
    vec_reserve(vec, vec->reserved_size - vec->deallocate_block_size);
    vec->size = CLAMP(vec->size, 0, vec->reserved_size);
}

void vec_erase(vec_t *vec, size_t begin_index, size_t end_index) {
    memcpy(vec_get(vec, 0), vec_get(vec, begin_index), begin_index * vec->item_size);
    size_t new_reserved_size = round_to(end_index - begin_index, vec->reserved_size);
    vec_reserve(vec, new_reserved_size);
    vec->size = end_index - begin_index;
}

/* does not reallocate, use vec_resize to totally clear */
void vec_clear(vec_t *vec) {
    vec->size = 0;
}

void vec_dbg(const vec_t *vec, FILE *out, void (*print_obj)(const void *value, FILE *out)) {
    fprintf(out, "vec_t: { data: %p [", vec->data);

    for (size_t i = 0; i < vec->size; i++) {
        fputs("\n\t", out);
        print_obj(vec_get(vec, i), out);
        if (vec->size > 1 && i < vec->size - 1) {
            fputs(",", out);
        } else {
            fputs("\n", out);
        }
    }
    fprintf(out, "], size: %zu, reserved_size: %zu, item_size: %zu, allocate_block_size: %zu, deallocate_block_size: %zu }\n", vec->size, vec->reserved_size, vec->item_size, vec->allocate_block_size, vec->deallocate_block_size);
}

void vec_destroy(vec_t *vec) {
    free(vec->data);
}

