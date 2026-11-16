# HW6: Sensor, Panel, and Scenario Replay

This folder reads the iom361 sensor and switches through their
registers, lights LEDs by read-modify-write, keeps a UTC clock, and
replays a scenario file into a trace, one line per tick. A press of
switch k toggles LED k.

- sensor.h / sensor.c: reading_t and sensor_read
- panel.h / panel.c: panel_pressed, panel_released, panel_led, panel_toggle
- scenario.h / scenario.c: scenario_t, scenario_load, scenario_at, scenario_last_tick
- main.c: `./replay scenario.txt`, the loop that prints the trace
- iom361_r4.h / iom361_r4.c: the I/O module emulator (from the course)
- tests/test_hw06.c: the unit tests
- scenarios/ and expected/: the regression scenarios and their traces
- regress.sh: runs ./replay on every scenario and diffs it with expected/

## Building and testing

From inside hw06:

    make                               # builds replay
    ./replay scenarios/basic.txt       # replays one scenario
    make test                          # unit tests, then sh regress.sh
    make clean                         # deletes everything the build made

The unit tests print PASS or FAIL per test and a summary. `make test`
fails if any unit test or any regression trace fails.

## Scenario files

    start 1791273600                # 2026-10-06 08:00:00 UTC
    # tick  temp   rh    switches
    0       21.5   40    0x0
    2       21.5   40    0x1        # switch 0 pressed

`#` starts a comment, and blank lines are skipped. The first other line
is `start <unix timestamp>`. Each line after it is `tick temp rh switches`
with the switches in hex and ticks increasing. Values hold between lines.
At most 1000 lines.

## Trace

    2026-10-06 08:00:02 temp=21.5 rh=40.0 switches=0x1 pressed=0x1 leds=0x1

## Design notes

**sensor_read changes nothing on a failed read.** Both registers are
read into local variables first. If either read fails, the function
returns the return code before it writes anything to `*r`.

**Float division.** `ST` is a `uint32_t`, so `ST / 1048576` would be
integer division and always give 0. Dividing by `1048576.0f` makes it
a float division.

**Rounding to 0.1.** The register is a 20-bit integer, so 91.0 comes
back as 90.99998. `roundf(x * 10) / 10` makes it 91.0. That is why the
link line needs `-lm`.

**pressed and released.** Pressed is `now & ~before` (on now, off
before). Released is `~now & before`. A held switch is 1 in both, so
it is in neither.

**Read-modify-write.** panel_led reads LEDS_REG, changes only bit n
(`| bit` to turn it on, `& ~bit` to turn it off, `^ bit` to toggle),
and writes it back. If it just wrote `1u << n`, every other LED would
turn off.

**Return values.** panel_led and panel_toggle return 0 on success, -1
if n is not 0 to 31, and the iom361 return code if a register read or
write fails. scenario_load returns 0, or the number of the first wrong
line, counting every line in the file, comments too. A file with no
start line returns the line where the start line should have been. A
file that cannot be opened returns 1.

**Only main sets the world.** `_iom361_set_sensor1` and
`_iom361_set_switches` are called only in main.c, never in sensor.c or
panel.c, because on a real board they would not exist.

**The clock is UTC.** `gmtime` and `strftime`, so the trace is the same
on every computer. midnight.txt checks the date change at midnight.
