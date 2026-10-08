#include "status.h"
#include "bits.h"

status_t status_unpack(uint16_t word)
{
    status_t s;

    // Flags: 1 bit each, true if set
    s.heat     = get_field(word, HEAT_POS, HEAT_WIDTH) != 0;
    s.cool     = get_field(word, COOL_POS, COOL_WIDTH) != 0;
    s.fan      = get_field(word, FAN_POS, FAN_WIDTH) != 0;
    s.fault    = get_field(word, FAULT_POS, FAULT_WIDTH) != 0;
    s.reserved = get_field(word, RESERVED_POS, RESERVED_WIDTH) != 0;

    // Mode: raw value 0-7, kept as-is (5-7 are invalid)
    s.mode = (uint8_t)get_field(word, MODE_POS, MODE_WIDTH);

    // Set point: 8-bit two's complement
    uint32_t raw_setpoint = get_field(word, SETPOINT_POS, SETPOINT_WIDTH);
    s.setpoint = (int8_t)sign_extend(raw_setpoint, SETPOINT_WIDTH);

    return s;
}