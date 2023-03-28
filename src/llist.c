#include "llist.h"


void llnode_create(llnode_t *llnode, size_t item_size, llnode_t *next, llnode_t *prev) {
    llnode->item_size = item_size;
    llnode->next = next;
    llnode->prev = prev;
    llnode->value = malloc(llnode->item_size);
}

void llnode_create_zero(llnode_t *llnode, size_t item_size, llnode_t *next, llnode_t *prev) {
    llnode_create(llnode, item_size, next, prev);
    memset(llnode->value, 0, llnode->item_size);
}

void llnode_create_set(llnode_t *llnode, size_t item_size, const void *value, llnode_t *next, llnode_t *prev) {
    llnode_create(llnode, item_size, next, prev);
    llnode_set(llnode, value);
}

void llnode_set(llnode_t *llnode, const void *value) {
    memcpy(llnode->value, value, llnode->item_size);
}

void *llnode_get(const llnode_t *llnode) {
    return llnode->value;
}

void llnode_get_copy(const llnode_t *llnode, void *dest) {
    memcpy(dest, llnode->value, llnode->item_size);
}

void llnode_destroy(llnode_t *llnode) {
    free(llnode->value);
}



void llist_create(llist_t *llist, size_t item_size) {
    llist->head = llist->tail = NULL;
    llist->size = 0;
    llist->item_size = item_size;
}

void llist_create_zero(llist_t *llist, size_t num, size_t item_size) {
    llist->size = num;
    llist->item_size = item_size;

    llist->head = malloc(sizeof(llnode_t));
    llist->head->prev = NULL;
    llnode_t *node = llist->head;

    for (size_t i = 0; i < num - 1; i++) {
        node->value = calloc(1, llist->item_size);
        node->item_size = llist->item_size;

        node->next = malloc(sizeof(llnode_t));
        llnode_t *prevnode = node;
        node = node->next;
        node->prev = prevnode;
    }

    llist->tail = node;
    llist->tail->item_size = llist->item_size;
    llist->tail->next = NULL;
    llist->tail->value = calloc(1, item_size);
}

void llist_create_set(llist_t *llist, size_t num, size_t item_size, const void *init_value) {
    llist->size = num;
    llist->item_size = item_size;

    llist->head = malloc(sizeof(llnode_t));
    llist->head->prev = NULL;
    llnode_t *node = llist->head;

    for (size_t i = 0; i < num - 1; i++) {
        node->value = malloc(item_size);
        memcpy(node->value, init_value, llist->item_size);
        node->item_size = item_size;

        node->next = malloc(sizeof(llnode_t));
        llnode_t *prevnode = node;
        node = node->next;
        node->prev = prevnode;
    }

    llist->tail = node;
    llist->tail->next = NULL;
}

void *llist_get(const llist_t *llist, size_t index) {
    if (index >= llist->size) {
        fprintf(stderr, "error: %s index %zu out of bounds (llist.size = %zu)\n", __func__, index, llist->size);
        exit(EXIT_FAILURE);
    }

    size_t current_index;
    if (index > llist->size / 2) {
        current_index = llist->size - 1;
        for (llnode_t *node = llist->tail; node->prev != NULL; node = node->prev, current_index--) {
            if (current_index == index) {
                return llnode_get(node);
            }
        }
    }

    /* assume we would have returned */
    current_index = 0;
    for (llnode_t *node = llist->head; node->next != NULL; node = node->next, current_index++) {
        if (current_index == index) {
            return llnode_get(node);
        }
    }

    /* if we get here, index was bad */
    fprintf(stderr, "error: %s index %zu out of bounds (llist.size = %zu)\n", __func__, index, llist->size);
    exit(EXIT_FAILURE);
}

void llist_get_copy(const llist_t *llist, size_t index, void *dest) {
    memcpy(dest, llist_get(llist, index), llist->item_size);
}

void llist_set(llist_t *llist, size_t index, const void *value) {
    memcpy(llist_get(llist, index), value, llist->item_size);
}

void llist_push_head(llist_t *llist, const void *value) {
    llnode_t *new_node = malloc(sizeof(llnode_t));
    new_node->item_size = llist->item_size;
    new_node->next = llist->head;
    new_node->prev = NULL;
    llist->head->prev = new_node;
    new_node->value = malloc(llist->item_size);
    memcpy(new_node->value, value, llist->item_size);
    llist->head = new_node;
    llist->size++;
}

void llist_push_tail(llist_t *llist, const void *value) {
    llnode_t *new_node = malloc(sizeof(llnode_t));
    new_node->item_size = llist->item_size;
    new_node->next = NULL;
    new_node->prev = llist->tail;
    llist->tail->next = new_node;
    new_node->value = malloc(llist->item_size);
    memcpy(new_node->value, value, llist->item_size);
    llist->tail = new_node;
    llist->size++;
}

/* places in dest OR pass NULL to not */
void llist_pop_head(llist_t *llist, void *dest) {
    if (llist->size == 0) {
        fprintf(stderr, "error: %s on an empty llist (llist.size = %zu)\n", __func__, llist->size);
        exit(EXIT_FAILURE);
    }

    if (dest != NULL) {
        memcpy(dest, llist->head->value, llist->item_size);
    }

    llist->size--;
    llnode_t *new_head = llist->head->next;
    if (new_head == NULL) {
        /* one element only, tail and head both point to the same thing */
        llnode_destroy(llist->head);
        free(llist->head);
        llist->head = NULL;
        llist->tail = NULL;
        return;
    }
    new_head->prev = NULL;
    free(llist->head);
    llist->head = new_head;
}

/* places in dest OR pass NULL to not */
void llist_pop_tail(llist_t *llist, void *dest) {
    if (llist->size == 0) {
        fprintf(stderr, "error: %s on an empty llist (llist.size = %zu)\n", __func__, llist->size);
        exit(EXIT_FAILURE);
    }

    if (dest != NULL) {
        memcpy(dest, llist->tail->value, llist->item_size);
    }

    llist->size--;
    llnode_t *new_tail = llist->tail->prev;
    if (new_tail == NULL) {
        /* one element only, tail and head both point to the same thing */
        llnode_destroy(llist->head);
        free(llist->head);
        llist->head = NULL;
        llist->tail = NULL;
        return;
    }
    new_tail->next = NULL;
    free(llist->tail);
    llist->tail = new_tail;
}

void llist_clear(llist_t *llist) {
    if (llist->size == 0) {
        fprintf(stderr, "error: %s on an empty llist (llist.size = %zu)\n", __func__, llist->size);
        exit(EXIT_FAILURE);
    }

    llnode_t *node = llist->head, *prevnode = NULL;
    while (node->next != NULL) {
        if (prevnode != NULL) {
            llnode_destroy(prevnode);
        }
        free(prevnode);
        prevnode = node;
        node = node->next;
    }
    llnode_destroy(prevnode);
    free(prevnode);
    llnode_destroy(node);
    free(node);
    llist->size = 0;
    llist->head = NULL;
    llist->tail = NULL;
}

void llist_dbg(const llist_t *llist, FILE *out) {
    fprintf(out, "llist_t: { nodes: [");
    for (llnode_t *node = llist->head; node != NULL; node = node->next) {
        fprintf(out, "\n\t%p: { value %p: %u, item_size: %zu, next: %p, prev: %p }", node, node->value, *(unsigned char*) node->value, node->item_size, node->next, node->prev);
        if (node->next != NULL) { /* is not last */
            fputs(",", out);
        }
    }

    fprintf(out, "\n], head: %p, tail: %p, size: %zu, item_size: %zu }\n", llist->head, llist->tail, llist->size, llist->item_size);
}

void llist_destroy(llist_t *llist) {
    llist_clear(llist);
}
