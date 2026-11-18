#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

#define ISTACK_START 8                  /* room at first; doubles when full */

typedef struct istack istack_t;         /* opaque: the struct is in istack.c */

/* Takes nothing.
Returns a new, empty stack with room for ISTACK_START values,
or NULL if memory runs out (and then nothing is leaked).
Assumes the caller calls istack_destroy on it when done. */
istack_t *istack_create(void);

/* Takes a stack from istack_create (or NULL).
Frees the stack and the array of values it holds. NULL does nothing.
Assumes s is not used again after this. */
void istack_destroy(istack_t *s);

/* Takes a stack and a value.
Pushes v on top. If the stack is full, it first doubles the room with realloc.
Returns 0, or -1 if s is NULL or memory runs out: then the stack is unchanged.
Assumes s came from istack_create and was not destroyed. */
int istack_push(istack_t *s, int v);

/* Takes a stack and where to put the top value (v may be NULL).
Copies the top value into *v and removes it from the stack.
Returns 0, or -1 if s is NULL or empty: then *v is unchanged.
Assumes s came from istack_create and was not destroyed. */
int istack_pop(istack_t *s, int *v);

/* Takes a stack and where to put the top value (v may be NULL).
Copies the top value into *v, but leaves it on the stack.
Returns 0, or -1 if s is NULL or empty: then *v is unchanged.
Assumes s came from istack_create and was not destroyed. */
int istack_peek(const istack_t *s, int *v);

/* Takes a stack.
Returns how many values it holds: 0 for NULL. */
size_t istack_count(const istack_t *s);

/* Takes a stack.
Returns true if it holds no values: true for NULL. */
bool istack_is_empty(const istack_t *s);

/* Takes a stack.
Returns how many values fit right now without growing: 0 for NULL. */
size_t istack_capacity(const istack_t *s);

/* Takes a stack and where to put the number of values.
Returns a NEW array with a copy of the values, bottom first, and stores
their number in *n. The caller owns the array and must free it.
Returns NULL and sets *n = 0 if s is NULL or empty, or memory runs out.
Assumes n is not NULL (if it is, returns NULL). */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
