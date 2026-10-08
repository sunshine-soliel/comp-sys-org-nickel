#ifndef STATUS_H
#define STATUS_H

#include <stdbool.h>
#include <stdint.h>

// bit positions and widths in the 16-bit status word
#define HEAT_POS        0
#define HEAT_WIDTH      1
#define COOL_POS        1
#define COOL_WIDTH      1
#define FAN_POS         2
#define FAN_WIDTH       1
#define FAULT_POS       3
#define FAULT_WIDTH     1
#define MODE_POS        4
#define MODE_WIDTH      3
#define RESERVED_POS    7
#define RESERVED_WIDTH  1
#define SETPOINT_POS    8
#define SETPOINT_WIDTH  8

// mode values
#define MODE_OFF        0
#define MODE_HEAT       1
#define MODE_COOL       2
#define MODE_AUTO       3
#define MODE_FAN_ONLY   4
#define MODE_LAST_VALID MODE_FAN_ONLY   // 5-7 are invalid

// one member per field
typedef struct {
    bool    heat;
    bool    cool;
    bool    fan;
    bool    fault;
    uint8_t mode;       // raw value 0-7 and above MODE_LAST_VALID is invalid
    bool    reserved;   // should be 0
    int8_t  setpoint;   // degrees C
} status_t;

status_t status_unpack(uint16_t word);

#endif