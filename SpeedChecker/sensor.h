#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include "config.h"

struct PulseData {
    unsigned long pulse_count;
    unsigned long last_pulse_us;
    unsigned long last_interval_us;
    unsigned long min_interval_us;
};

// Initialize sensor pin and attach interrupt
void sensor_init();

// Get pulse data with interrupts disabled (thread-safe)
// Also resets counters for next window
PulseData sensor_read_and_reset();

// Get last pulse timestamp without resetting (for signal detection)
unsigned long sensor_get_last_pulse_us();

#endif
