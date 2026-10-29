#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX     4    /* tallies that can exist at the same time */
#define TALLY_NAMES  16    /* different names one tally can hold */
#define TALLY_LEN    15    /* longest name, without the '\0' */

typedef struct tally tally_t;   /* opaque */

/* Takes nothing.
Returns a new, empty tally, or NULL if TALLY_MAX tallies are already in use.
Assumes nothing. */
tally_t *tally_create(void);

/* Takes a tally from tally_create (or NULL).
Gives the tally back so tally_create can hand it out again. NULL does nothing.
Assumes t is not used again after this. */
void tally_destroy(tally_t *t);

/* Takes a tally and a name.
Counts one more occurrence of name and returns its new count.
Returns -1 and changes nothing if t or name is NULL, name is empty
or longer than TALLY_LEN, or name is new and the tally is full.
Assumes name is a '\0'-terminated string. The tally keeps its own copy. */
int tally_add(tally_t *t, const char *name);

/* Takes a tally and a name.
Returns how many times name was added: 0 if never, or if t or name is NULL.
Assumes name is a '\0'-terminated string. */
int tally_count(const tally_t *t, const char *name);

/* Takes a tally.
Returns how many different names it holds: 0 for NULL.
Assumes nothing. */
int tally_size(const tally_t *t);

/* Takes a tally and an open output stream.
Prints one line per name, "name count", in alphabetical (strcmp) order.
Prints nothing if t or out is NULL. */
void tally_print(const tally_t *t, FILE *out);

#endif
