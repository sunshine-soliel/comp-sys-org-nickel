#include <stdio.h>
#include "../bits.h"
#include "../status.h"

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
    // ----- print_binary (check by eye) -----
    printf("expect 0101:\n");
    print_binary(5, 4);
    printf("expect 1011 0110:\n");
    print_binary(0xB6, 8);
    printf("expect 1:\n");
    print_binary(1, 1);
    printf("expect 32 ones:\n");
    print_binary(0xFFFFFFFF, 40);
    printf("\n");

    // ----- get_field -----
    check(get_field(0xB6, 2, 3) == 5,                  "get_field basic");
    check(get_field(0xFFFFFFFF, 0, 1) == 1,            "get_field width 1");
    check(get_field(0xC0FFEE00, 0, 32) == 0xC0FFEE00,  "get_field width 32");
    check(get_field(0x80000000, 31, 1) == 1,           "get_field pos 31");
    check(get_field(0xFFFFFFFF, 31, 2) == 0,           "get_field off the edge");
    check(get_field(0xFFFFFFFF, 0, 33) == 0,           "get_field width 33");
    check(get_field(0xFFFFFFFF, -1, 2) == 0,           "get_field negative pos");
    check(get_field(0xFFFFFFFF, 0, 0) == 0,            "get_field width 0");

    // ----- set_field -----
    check(set_field(0xB6, 2, 3, 2) == 0xAA,                       "set_field basic");
    check(set_field(0xB6, 4, 4, 0x3) == 0x36,                     "set_field upper nibble");
    check(set_field(0x0, 0, 1, 1) == 0x1,                         "set_field width 1");
    check(set_field(0x12345678, 0, 32, 0xCAFEBABE) == 0xCAFEBABE, "set_field width 32");
    check(set_field(0x0, 31, 1, 1) == 0x80000000,                 "set_field pos 31");
    check(set_field(0xB6, 4, 4, 0x13) == 0x36,                    "set_field value too wide");
    check(set_field(0xB6, 30, 4, 0xF) == 0xB6,                    "set_field off the edge");
    check(set_field(0xB6, 0, 0, 0xF) == 0xB6,                     "set_field width 0");

    // ----- sign_extend -----
    check(sign_extend(0xF8, 8) == -8,                  "sign_extend negative");
    check(sign_extend(0x16, 8) == 22,                  "sign_extend positive");
    check(sign_extend(0xFFFFFF05, 4) == 5,             "sign_extend ignores upper bits");
    check(sign_extend(0x0A, 4) == -6,                  "sign_extend 4-bit negative");
    check(sign_extend(0x1, 1) == -1,                   "sign_extend width 1, bit set");
    check(sign_extend(0x0, 1) == 0,                    "sign_extend width 1, bit clear");
    check(sign_extend(0x80, 8) == -128,                "sign_extend most negative 8-bit");
    check(sign_extend(0x7F, 8) == 127,                 "sign_extend most positive 8-bit");
    check(sign_extend(0x80000000, 32) == INT32_MIN,    "sign_extend most negative 32-bit");
    check(sign_extend(0x7FFFFFFF, 32) == INT32_MAX,    "sign_extend most positive 32-bit");
    check(sign_extend(0xF, 0) == 0,                    "sign_extend width 0");

    // ----- status_unpack 0x1631 (example from the PDF) -----
    status_t s = status_unpack(0x1631);
    check(s.setpoint == 22,        "0x1631 setpoint");
    check(s.mode == MODE_AUTO,     "0x1631 mode");
    check(s.heat == true,          "0x1631 heat");
    check(s.cool == false,         "0x1631 cool");
    check(s.fan == false,          "0x1631 fan");
    check(s.fault == false,        "0x1631 fault");
    check(s.reserved == false,     "0x1631 reserved");

    // ----- status_unpack 0xF82E (negative set point) -----
    s = status_unpack(0xF82E);
    check(s.setpoint == -8,        "0xF82E setpoint");
    check(s.mode == MODE_COOL,     "0xF82E mode");
    check(s.heat == false,         "0xF82E heat");
    check(s.cool == true,          "0xF82E cool");
    check(s.fan == true,           "0xF82E fan");
    check(s.fault == true,         "0xF82E fault");
    check(s.reserved == false,     "0xF82E reserved");

    // ----- status_unpack 0x0040 (mode 4, last valid) -----
    s = status_unpack(0x0040);
    check(s.mode == MODE_FAN_ONLY,       "0x0040 mode is 4");
    check(s.mode <= MODE_LAST_VALID,     "0x0040 mode is valid");

    // ----- status_unpack 0x0060 (mode 6, invalid) -----
    s = status_unpack(0x0060);
    check(s.mode == 6,                   "0x0060 mode kept as 6");
    check(s.mode > MODE_LAST_VALID,      "0x0060 mode is invalid");

    // ----- Summary -----
    printf("\n%d passed, %d failed\n", pass_count, fail_count);

    // Exit code: 0 means everything passed, 1 means something failed
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}