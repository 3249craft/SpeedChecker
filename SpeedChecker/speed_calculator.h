#ifndef SPEED_CALCULATOR_H
#define SPEED_CALCULATOR_H

#include <Arduino.h>
#include "config.h"
#include "sensor.h"

struct SpeedData {
    float current_speed_kmh;
    float top_speed_kmh;
    unsigned long pulses_per_second;
    unsigned long min_interval_us;
    bool signal_active;
};

// Initialize calculator state
void speed_calculator_init();

// Calculate speed from pulse data and time window
SpeedData calculate_speed(const PulseData& pulse_data,
                          unsigned long window_ms,
                          unsigned long now_us);

// Reset top speed tracking
void speed_calculator_reset_top();

#endif
