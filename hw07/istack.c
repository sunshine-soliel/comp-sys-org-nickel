#include <stdlib.h>
#include <string.h>
#include "istack.h"

// The real struct. Only this file can see inside it.
struct istack {
    int    *data;    // one array on the heap: data[0] is the bottom
    size_t  count;   // how many values are on the stack
    size_t  cap;     // how many values fit in data right now
};

istack_t *istack_create(void)
{
    // First the struct itself
    istack_t *s = malloc(sizeof *s);
    if (s == NULL) {
        return NULL;
    }

    // Then the array of values
    s->data = malloc(ISTACK_START * sizeof *s->data);
    if (s->data == NULL) {
        free(s);           // give back the struct, so nothing leaks
        return NULL;
    }

    s->count = 0;
    s->cap = ISTACK_START;
    return s;
}

void istack_destroy(istack_t *s)
{
    if (s == NULL) {
        return;
    }
    // Free the inside first: after free(s), s->data can't be read anymore
    free(s->data);
    free(s);
}

int istack_push(istack_t *s, int v)
{
    if (s == NULL) {
        return -1;
    }

    // Full: double the room first
    if (s->count == s->cap) {
        size_t new_cap = s->cap * 2;

        // realloc into a SECOND pointer. If it fails it returns NULL but the
        // old array is still there, and s->data still points to it.
        int *bigger = realloc(s->data, new_cap * sizeof *bigger);
        if (bigger == NULL) {
            return -1;     // stack unchanged
        }
        s->data = bigger;
        s->cap = new_cap;
    }

    s->data[s->count] = v;
    s->count = s->count + 1;
    return 0;
}

int istack_pop(istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;         // *v not touched
    }
    s->count = s->count - 1;
    if (v != NULL) {
        *v = s->data[s->count];
    }
    return 0;              // the room never shrinks
}

int istack_peek(const istack_t *s, int *v)
{
    if (s == NULL || s->count == 0) {
        return -1;
    }
    if (v != NULL) {
        *v = s->data[s->count - 1];
    }
    return 0;
}

size_t istack_count(const istack_t *s)
{
    if (s == NULL) {
        return 0;
    }
    return s->count;
}

bool istack_is_empty(const istack_t *s)
{
    return istack_count(s) == 0;
}

size_t istack_capacity(const istack_t *s)
{
    if (s == NULL) {
        return 0;
    }
    return s->cap;
}

int *istack_to_array(const istack_t *s, size_t *n)
{
    if (n == NULL) {
        return NULL;
    }
    *n = 0;
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    // A new array: the caller owns it and frees it
    int *copy = malloc(s->count * sizeof *copy);
    if (copy == NULL) {
        return NULL;
    }

    // data[0] is the bottom, so a straight copy is bottom first
    memcpy(copy, s->data, s->count * sizeof *copy);
    *n = s->count;
    return copy;
}
