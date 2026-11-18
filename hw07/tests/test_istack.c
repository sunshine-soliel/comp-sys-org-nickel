#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../istack.h"

// Running totals, shared by every test
int pass_count = 0;
int fail_count = 0;

// Prints PASS or FAIL for one test and adds to the totals.
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
    int v;
    int rc;

    // ----- a new stack -----
    istack_t *s = istack_create();
    check(s != NULL, "create: returns a stack");
    check(istack_count(s) == 0 && istack_is_empty(s), "create: starts empty");
    check(istack_capacity(s) == ISTACK_START && ISTACK_START == 8, "create: room for 8");

    // ----- pop and peek on an empty stack leave *v alone -----
    v = 12345;
    rc = istack_pop(s, &v);
    check(rc == -1 && v == 12345, "empty: pop returns -1 and leaves *v alone");
    rc = istack_peek(s, &v);
    check(rc == -1 && v == 12345, "empty: peek returns -1 and leaves *v alone");

    // ----- last in, first out -----
    istack_push(s, 10);
    istack_push(s, 20);
    istack_push(s, 30);
    rc = istack_peek(s, &v);
    check(rc == 0 && v == 30 && istack_count(s) == 3, "peek: top is 30, count stays 3");
    istack_pop(s, &v);
    check(v == 30, "LIFO: first pop is 30");
    istack_pop(s, &v);
    check(v == 20, "LIFO: second pop is 20");
    istack_pop(s, &v);
    check(v == 10 && istack_is_empty(s), "LIFO: third pop is 10, now empty");

    // pop and peek with v == NULL still work
    istack_push(s, 7);
    check(istack_peek(s, NULL) == 0 && istack_count(s) == 1, "peek: v may be NULL");
    check(istack_pop(s, NULL) == 0 && istack_count(s) == 0, "pop: v may be NULL");

    // ----- full at 8, the 9th push doubles to 16 -----
    for (int i = 0; i < 8; i++) {
        istack_push(s, i * 100);
    }
    check(istack_count(s) == 8 && istack_capacity(s) == 8, "grow: 8 pushes fill the room of 8");
    rc = istack_push(s, 800);
    check(rc == 0 && istack_capacity(s) == 16, "grow: 9th push doubles the room to 16");
    check(istack_count(s) == 9, "grow: count is 9");
    bool kept = true;
    for (int i = 8; i >= 0; i--) {
        istack_pop(s, &v);
        if (v != i * 100) {
            kept = false;
        }
    }
    check(kept, "grow: every value kept after the realloc");
    check(istack_capacity(s) == 16, "grow: room never shrinks after popping");
    istack_destroy(s);

    // ----- 1000 pushes and pops -----
    s = istack_create();
    bool ok = true;
    for (int i = 0; i < 1000; i++) {
        if (istack_push(s, i) != 0) {
            ok = false;
        }
    }
    check(ok && istack_count(s) == 1000, "1000: all pushes succeed");
    check(istack_capacity(s) == 1024, "1000: room doubled 8 -> 1024");
    for (int i = 999; i >= 0; i--) {
        if (istack_pop(s, &v) != 0 || v != i) {
            ok = false;
        }
    }
    check(ok && istack_is_empty(s), "1000: pops come back 999 down to 0");
    istack_destroy(s);

    // ----- istack_to_array: bottom first, and a copy -----
    s = istack_create();
    istack_push(s, 1);
    istack_push(s, 2);
    istack_push(s, 3);
    size_t n = 99;
    int *a = istack_to_array(s, &n);
    check(a != NULL && n == 3, "to_array: 3 values");
    check(a != NULL && a[0] == 1 && a[1] == 2 && a[2] == 3, "to_array: bottom first");
    if (a != NULL) {
        a[2] = -1;                    // change the copy...
    }
    istack_peek(s, &v);
    check(v == 3, "to_array: changing the array does not change the stack");
    istack_push(s, 4);                // ...and the stack
    check(a != NULL && a[0] == 1 && n == 3, "to_array: pushing does not change the array");
    free(a);                          // the caller owns it

    // to_array on an empty stack
    istack_t *empty = istack_create();
    n = 99;
    a = istack_to_array(empty, &n);
    check(a == NULL && n == 0, "to_array: empty stack gives NULL and n = 0");
    istack_destroy(empty);

    // ----- NULL arguments -----
    v = 555;
    n = 99;
    check(istack_push(NULL, 1) == -1, "NULL: push returns -1");
    check(istack_pop(NULL, &v) == -1 && v == 555, "NULL: pop returns -1, *v alone");
    check(istack_peek(NULL, &v) == -1 && v == 555, "NULL: peek returns -1, *v alone");
    check(istack_count(NULL) == 0, "NULL: count is 0");
    check(istack_is_empty(NULL) == true, "NULL: is_empty is true");
    check(istack_capacity(NULL) == 0, "NULL: capacity is 0");
    check(istack_to_array(NULL, &n) == NULL && n == 0, "NULL: to_array gives NULL and n = 0");
    check(istack_to_array(s, NULL) == NULL, "NULL: to_array with n NULL gives NULL");
    istack_destroy(NULL);
    check(true, "NULL: destroy does nothing");
    istack_destroy(s);

    // ----- two stacks are independent -----
    istack_t *x = istack_create();
    istack_t *y = istack_create();
    for (int i = 0; i < 20; i++) {
        istack_push(x, i);            // x grows to 32
    }
    istack_push(y, -5);
    check(istack_count(x) == 20 && istack_count(y) == 1, "two: counts are separate");
    check(istack_capacity(x) == 32 && istack_capacity(y) == 8, "two: rooms are separate");
    istack_peek(y, &v);
    check(v == -5, "two: y's top is its own");
    istack_pop(x, &v);
    check(v == 19 && istack_count(y) == 1, "two: popping x leaves y alone");
    istack_destroy(y);
    istack_peek(x, &v);
    check(v == 18, "two: destroying y leaves x alone");

    // ----- destroying a stack that is not empty -----
    // x still holds 19 values. make memcheck reports a leak if this misses any.
    istack_destroy(x);
    check(true, "destroy: a stack that is not empty");

    printf("\n%d passed, %d failed\n", pass_count, fail_count);
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}
