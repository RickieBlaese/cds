#include "common.h"


typedef struct llnode {
    void *value;
    /* const */ size_t item_size;
    struct llnode *next, *prev;
} llnode_t;


void llnode_create(llnode_t *llnode, size_t item_size, llnode_t *next, llnode_t *prev);

void llnode_create_zero(llnode_t *llnode, size_t item_size, llnode_t *next, llnode_t *prev);

void llnode_create_set(llnode_t *llnode, size_t item_size, const void *value, llnode_t *next, llnode_t *prev);

void llnode_set(llnode_t *llnode, const void *value);

void *llnode_get(const llnode_t *llnode);

void llnode_get_copy(const llnode_t *llnode, void *dest);

void llnode_destroy(llnode_t *llnode);


typedef struct llist {
    size_t size;
    /* const */ size_t item_size;
    llnode_t *head, *tail;
} llist_t;


void llist_create(llist_t *llist, size_t item_size);

void llist_create_zero(llist_t *llist, size_t num, size_t item_size);

void llist_create_set(llist_t *llist, size_t num, size_t item_size, const void *init_value);

void *llist_get(const llist_t *llist, size_t index);

void llist_get_copy(const llist_t *llist, size_t index, void *dest);

void llist_set(llist_t *llist, size_t index, const void *value);

void llist_push_head(llist_t *llist, const void *value);

void llist_push_tail(llist_t *llist, const void *value);

/* places in dest OR pass NULL to not */
void llist_pop_head(llist_t *llist, void *dest);

/* places in dest OR pass NULL to not */
void llist_pop_tail(llist_t *llist, void *dest);

void llist_clear(llist_t *llist);

void llist_dbg(const llist_t *llist, FILE *out);

void llist_destroy(llist_t *llist);
