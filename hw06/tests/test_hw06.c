#include <stdio.h>
#include <stdbool.h>
#include "../iom361_r4.h"
#include "../sensor.h"
#include "../panel.h"
#include "../scenario.h"

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

// Writes text to a file, so a test can load it as a scenario
void write_file(const char *path, const char *text)
{
    FILE *f = fopen(path, "w");
    if (f != NULL) {
        fputs(text, f);
        fclose(f);
    }
}

static scenario_t s;   // static: too big for the stack

int main(void)
{
    int rc;
    uint32_t *base = iom361_initialize(4, 4, &rc);
    iom361_set_display(false);

    // ----- sensor -----
    reading_t r;
    _iom361_set_sensor1(91.0f, 40.0f);
    rc = sensor_read(base, 1000, &r);
    check(rc == 0 && r.temp_c == 91.0f, "sensor: 91.0 C reads back as 91.0");
    check(r.rh_pct == 40.0f, "sensor: 40.0 %RH reads back as 40.0");
    check(r.t == 1000, "sensor: timestamp is stored");

    _iom361_set_sensor1(-12.3f, 55.5f);
    rc = sensor_read(base, 1001, &r);
    check(rc == 0 && r.temp_c == -12.3f && r.rh_pct == 55.5f, "sensor: -12.3 C, 55.5 %RH");

    // A failed read: base + 1 is not the base iom361 gave us, so rc is 1
    reading_t before_fail = { 42, 1.5f, 2.5f };
    reading_t r2 = before_fail;
    rc = sensor_read(base + 1, 2000, &r2);
    check(rc != 0, "sensor: bad base returns an error");
    check(r2.t == 42 && r2.temp_c == 1.5f && r2.rh_pct == 2.5f,
          "sensor: failed read changes nothing");

    // ----- pressed / released -----
    check(panel_pressed(0x1, 0x0) == 0x1, "pressed: a press");
    check(panel_released(0x1, 0x0) == 0x0, "released: a press is not a release");
    check(panel_released(0x0, 0x1) == 0x1, "released: a release");
    check(panel_pressed(0x0, 0x1) == 0x0, "pressed: a release is not a press");
    check(panel_pressed(0x1, 0x1) == 0x0 && panel_released(0x1, 0x1) == 0x0,
          "pressed/released: a held switch is neither");
    check(panel_pressed(0x6, 0x5) == 0x2 && panel_released(0x6, 0x5) == 0x1,
          "pressed/released: 0101 -> 0110 mixes all three");

    // ----- panel_led / panel_toggle keep the other LEDs -----
    iom361_writeReg(base, LEDS_REG, 0x5, &rc);      // LEDs 0 and 2 on
    panel_led(base, 1, 1);
    check(iom361_readReg(base, LEDS_REG, &rc) == 0x7, "panel_led: on LED 1 keeps 0 and 2");
    panel_led(base, 0, 0);
    check(iom361_readReg(base, LEDS_REG, &rc) == 0x6, "panel_led: off LED 0 keeps 1 and 2");
    panel_toggle(base, 3);
    check(iom361_readReg(base, LEDS_REG, &rc) == 0xE, "panel_toggle: LED 3 on, others kept");
    panel_toggle(base, 3);
    check(iom361_readReg(base, LEDS_REG, &rc) == 0x6, "panel_toggle: LED 3 off again");
    check(panel_led(base, 32, 1) == -1 && panel_led(base, -1, 1) == -1,
          "panel_led: n out of range returns -1");
    check(panel_toggle(base, 32) == -1, "panel_toggle: n out of range returns -1");
    check(iom361_readReg(base, LEDS_REG, &rc) == 0x6, "panel: bad n changes nothing");

    // ----- scenario: load with comments and blank lines -----
    write_file("tests/tmp_ok.txt",
        "# a comment\n"
        "\n"
        "start 1791273600   # 2026-10-06 08:00:00 UTC\n"
        "   \n"
        "# tick temp rh switches\n"
        "0  21.5  40  0x0\n"
        "\n"
        "5  22.0  41.5  0xA   # comment at the end\n");
    rc = scenario_load("tests/tmp_ok.txt", &s);
    check(rc == 0, "scenario: load with comments and blank lines");
    check(s.start == 1791273600 && s.count == 2, "scenario: start time and 2 lines");
    check(s.lines[1].tick == 5 && s.lines[1].switches == 0xA && s.lines[1].rh_pct == 41.5f,
          "scenario: fields of line 2, switches in hex");

    // values hold between lines
    check(scenario_at(&s, 0) == &s.lines[0], "scenario: tick 0 uses line 1");
    check(scenario_at(&s, 4) == &s.lines[0], "scenario: tick 4 still uses line 1");
    check(scenario_at(&s, 5) == &s.lines[1], "scenario: tick 5 uses line 2");
    check(scenario_at(&s, 99) == &s.lines[1], "scenario: after the last line, it holds");
    check(scenario_last_tick(&s) == 5, "scenario: last tick is 5");

    // ----- scenario: wrong files -----
    write_file("tests/tmp_nostart.txt",
        "# no start line\n"
        "0  21.5  40  0x0\n");
    check(scenario_load("tests/tmp_nostart.txt", &s) == 2, "scenario: no start line -> line 2");

    write_file("tests/tmp_missing.txt",
        "start 1791273600\n"
        "0  21.5  40  0x0\n"
        "3  21.5  40\n");
    check(scenario_load("tests/tmp_missing.txt", &s) == 3, "scenario: missing field -> line 3");

    write_file("tests/tmp_order.txt",
        "start 1791273600\n"
        "0  21.5  40  0x0\n"
        "# comment\n"
        "5  21.5  40  0x1\n"
        "5  21.5  40  0x0\n");
    check(scenario_load("tests/tmp_order.txt", &s) == 5, "scenario: tick not increasing -> line 5");

    check(scenario_load("tests/does_not_exist.txt", &s) != 0, "scenario: missing file is an error");

    remove("tests/tmp_ok.txt");
    remove("tests/tmp_nostart.txt");
    remove("tests/tmp_missing.txt");
    remove("tests/tmp_order.txt");

    printf("\n%d passed, %d failed\n", pass_count, fail_count);
    if (fail_count > 0) {
        return 1;
    }
    return 0;
}
