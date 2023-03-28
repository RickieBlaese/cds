#include "llist.h"
#include "vec.h"

typedef struct shit {
    int shit;
    float other;
} shit_t;

void shit_print_func(const void *value, FILE *out) {
    const shit_t this = *(shit_t*) value;
    fprintf(out, "shit_t: { shit: %i, other: %f }", this.shit, this.other);
}

int main() {
    /*
    llist_t list;
    llist_t *plist = &list;
    llist_create_zero(plist, 10, sizeof(shit_t));

    llist_dbg(plist, stdout);

    shit_t obj;
    obj.shit = 17;
    obj.other = 2.3f;
    llist_push_tail(plist, &obj);
    llist_dbg(plist, stdout);

    llist_destroy(plist);
    llist_dbg(plist, stdout);
    */

    vec_t vec;
    vec_t *pvec = &vec;
    vec_create(pvec, sizeof(shit_t));

    vec_dbg(pvec, stdout, &shit_print_func);

    for (uint32_t i = 0; i < 15; i++) {
        shit_t this;
        this.shit = i;
        this.other = 2 * i;
        vec_push(pvec, &this);
        vec_dbg(pvec, stdout, &shit_print_func);
    }

    for (uint32_t i = 0; i < 10; i++) {
        vec_pop(pvec, NULL);
        vec_dbg(pvec, stdout, &shit_print_func);
    }

    vec_destroy(pvec);

    return 0;
}
