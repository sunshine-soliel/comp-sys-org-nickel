#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../tally.h"

// Running totals, shared by every test
int pass_count = 0;
int fail_count = 0;

// Prints PASS or FAIL for one test and adds to the totals.
// result: true if the test passed
// name:   a label to print
void check(bool result, const char *name)
{
    if (result == true) {
        printf("PASS  %s\n", name);
        pass_count = pass_count + 1;
    } else {
        printf("FAIL  %s\n", name);
        fail_count = fail_count + 1;
    }
}

int main(void)
{
    // ----- counting up -----
    tally_t *t = tally_create();
    check(t != NULL,                     "create works");
    check(tally_size(t) == 0,            "new tally is empty");
    check(tally_add(t, "open") == 1,     "first add returns 1");
    check(tally_add(t, "open") == 2,     "second add returns 2");
    check(tally_add(t, "open") == 3,     "third add returns 3");
    check(tally_count(t, "open") == 3,   "count after 3 adds");
    check(tally_size(t) == 1,            "same name counted once in size");

    // ----- a name never added -----
    check(tally_count(t, "close") == 0,  "name never added counts 0");

    // ----- empty name -----
    check(tally_add(t, "") == -1,        "empty name refused");
    check(tally_size(t) == 1,            "empty name changes nothing");

    // ----- TALLY_LEN and TALLY_LEN + 1 -----
    char exact[TALLY_LEN + 1];
    memset(exact, 'a', TALLY_LEN);
    exact[TALLY_LEN] = '\0';
    char too_long[TALLY_LEN + 2];
    memset(too_long, 'b', TALLY_LEN + 1);
    too_long[TALLY_LEN + 1] = '\0';

    check(tally_add(t, exact) == 1,         "name of exactly TALLY_LEN accepted");
    check(tally_count(t, exact) == 1,       "TALLY_LEN name copied correctly");
    check(tally_add(t, too_long) == -1,     "name of TALLY_LEN + 1 refused");
    check(tally_count(t, too_long) == 0,    "TALLY_LEN + 1 name not stored");
    check(tally_size(t) == 2,               "size after length tests");

    // ----- NULL arguments -----
    check(tally_add(NULL, "open") == -1,    "add NULL tally");
    check(tally_add(t, NULL) == -1,         "add NULL name");
    check(tally_count(NULL, "open") == 0,   "count NULL tally");
    check(tally_count(t, NULL) == 0,        "count NULL name");
    check(tally_size(NULL) == 0,            "size NULL tally");
    tally_print(NULL, stdout);              // must not crash
    tally_print(t, NULL);                   // must not crash
    tally_destroy(NULL);                    // must not crash
    check(true,                             "print/destroy NULL don't crash");
    tally_destroy(t);

    // ----- full tally -----
    t = tally_create();
    char name[8];
    bool all_added = true;
    for (int i = 0; i < TALLY_NAMES; i++) {
        snprintf(name, sizeof name, "n%02d", i);
        if (tally_add(t, name) != 1) {
            all_added = false;
        }
    }
    check(all_added,                        "TALLY_NAMES names fit");
    check(tally_size(t) == TALLY_NAMES,     "size is TALLY_NAMES");
    check(tally_add(t, "new") == -1,        "full tally refuses new name");
    check(tally_size(t) == TALLY_NAMES,     "refused name changes nothing");
    check(tally_add(t, "n05") == 2,         "full tally still counts existing name");
    tally_destroy(t);

    // ----- TALLY_MAX tallies, then one more -----
    tally_t *many[TALLY_MAX];
    bool all_created = true;
    for (int i = 0; i < TALLY_MAX; i++) {
        many[i] = tally_create();
        if (many[i] == NULL) {
            all_created = false;
        }
    }
    check(all_created,                      "TALLY_MAX tallies can be created");
    check(tally_create() == NULL,           "one more than TALLY_MAX is NULL");

    // ----- create after destroy -----
    tally_add(many[1], "old");
    tally_destroy(many[1]);
    many[1] = tally_create();
    check(many[1] != NULL,                  "create after destroy works");
    check(tally_size(many[1]) == 0,         "reused tally starts empty");
    for (int i = 0; i < TALLY_MAX; i++) {
        tally_destroy(many[i]);
    }

    // ----- two tallies are independent -----
    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_add(a, "open");
    tally_add(a, "open");
    tally_add(b, "close");
    check(tally_count(a, "open") == 2,      "tally a has its own count");
    check(tally_count(b, "open") == 0,      "tally b doesn't see a's name");
    check(tally_count(a, "close") == 0,     "tally a doesn't see b's name");
    check(tally_size(a) == 1 && tally_size(b) == 1, "sizes are separate");
    tally_destroy(a);
    tally_destroy(b);

    // ----- print order -----
    // Add out of order (middle, end, start) so every insert case runs,
    // print into a temporary file, then read it back and compare.
    t = tally_create();
    tally_add(t, "motion");
    tally_add(t, "open");
    tally_add(t, "close");
    tally_add(t, "open");
    tally_add(t, "doorbell");

    FILE *f = tmpfile();
    char got[256] = "";
    if (f != NULL) {
        tally_print(t, f);
        rewind(f);
        size_t len = fread(got, 1, sizeof got - 1, f);
        got[len] = '\0';
        fclose(f);
    }
    check(strcmp(got, "close 1\ndoorbell 1\nmotion 1\nopen 2\n") == 0,
          "print is in alphabetical order");
    tally_destroy(t);

    // ----- Summary -----
    printf("\n%d passed, %d failed\n", pass_count, fail_count);

    // Exit code: 0 means everything passed, 1 means something failed
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}
