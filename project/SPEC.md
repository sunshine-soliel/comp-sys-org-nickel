# Microwave oven: specification

## States

| State   | Meaning                                 |
|---------|-----------------------------------------|
| IDLE    | no time set (initial state)             |
| SET     | time set, waiting for START             |
| COOKING | magnetron on                            |
| PAUSED  | cooking interrupted with time remaining |
| DONE    | time up                                 |

## Events (first draft)

| Event         | Comes from                                                     |
|---------------|----------------------------------------------------------------|
| ADD_30        | ADD_30 switch goes from 0 to 1 (press)                         |
| START         | START switch goes from 0 to 1 (press)                          |
| DOOR_OPENED   | DOOR_OPEN switch is on (level)                                 |
| DOOR_CLOSED   | DOOR_OPEN switch is off (level)                                |
| TICK          | one second passes, time remaining goes down by 1 while cooking |
| TIME_UP       | time remaining reaches 0 while cooking                         |
| OVERHEAT      | cavity temperature above 90.0 C (rule 3)                       |
| DONE_TIMEOUT  | 3 ticks have passed in DONE                                    |

