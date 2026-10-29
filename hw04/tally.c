#include <stdbool.h>
#include <string.h>
#include "tally.h"

// One name and how many times it was added
typedef struct {
    char name[TALLY_LEN + 1];   // +1 for the '\0'
    int  count;
} entry_t;

// The only place struct tally is completed.
// entries[0] to entries[n - 1] are in use, kept sorted by name.
struct tally {
    bool    in_use;
    int     n;
    entry_t entries[TALLY_NAMES];
};

// The pool: every tally that can ever exist. static = only tally.c sees it.
static tally_t pool[TALLY_MAX];

tally_t *tally_create(void)
{
    // Hand out the first free tally
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!pool[i].in_use) {
            pool[i].in_use = true;
            pool[i].n = 0;      // start empty, even if it was used before
            return &pool[i];
        }
    }
    // All TALLY_MAX are in use
    return NULL;
}

void tally_destroy(tally_t *t)
{
    if (t == NULL) {
        return;
    }
    // Mark it free so tally_create can hand it out again
    t->in_use = false;
    t->n = 0;
}

int tally_add(tally_t *t, const char *name)
{
    // Bad input: change nothing
    if (t == NULL || name == NULL) {
        return -1;
    }

    // Check the length BEFORE copying anything
    size_t len = strlen(name);
    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    // Find where name belongs: the first entry that is >= name
    int i = 0;
    while (i < t->n && strcmp(t->entries[i].name, name) < 0) {
        i++;
    }

    // Already there: just count it (works even when the tally is full)
    if (i < t->n && strcmp(t->entries[i].name, name) == 0) {
        t->entries[i].count++;
        return t->entries[i].count;
    }

    // New name, but no room
    if (t->n == TALLY_NAMES) {
        return -1;
    }

    // Move entries i to n-1 up by one, starting from the END.
    // Starting from the end means nothing gets overwritten before it moves.
    for (int j = t->n; j > i; j--) {
        t->entries[j] = t->entries[j - 1];
    }

    // Copy the name into the open slot. len <= TALLY_LEN, so len + 1
    // bytes (with the '\0') always fits in name[TALLY_LEN + 1].
    memcpy(t->entries[i].name, name, len + 1);
    t->entries[i].count = 1;
    t->n++;
    return 1;
}

int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return 0;
    }
    for (int i = 0; i < t->n; i++) {
        if (strcmp(t->entries[i].name, name) == 0) {
            return t->entries[i].count;
        }
    }
    // Never added
    return 0;
}

int tally_size(const tally_t *t)
{
    if (t == NULL) {
        return 0;
    }
    return t->n;
}

void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL) {
        return;
    }
    // Entries are already sorted, so just print them in order
    for (int i = 0; i < t->n; i++) {
        fprintf(out, "%s %d\n", t->entries[i].name, t->entries[i].count);
    }
}
