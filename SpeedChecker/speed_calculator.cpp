#include "speed_calculator.h"

static float top_speed_kmh = 0.0f;
static unsigned long last_min_interval_us = 0;
static float smoothed_speed_kmh = 0.0f;

void speed_calculator_init() {
    top_speed_kmh = 0.0f;
    last_min_interval_us = 0;
    smoothed_speed_kmh = 0.0f;
}

SpeedData calculate_speed(const PulseData& pulse_data,
                          float avg_sub_speed_kmh,
                          unsigned long window_ms,
                          unsigned long now_us) {
    SpeedData data;
    float window_sec = window_ms / 1000.0f;
    float current_speed = avg_sub_speed_kmh;

    // Calculate pulses per second (for debug display)
    if (window_sec > 0.0f) {
        data.pulses_per_second = (unsigned long)(pulse_data.pulse_count / window_sec);
    } else {
        data.pulses_per_second = 0;
    }

    // Check signal activity
    data.signal_active = (pulse_data.pulse_count > 0) ||
        ((pulse_data.last_pulse_us != 0) &&
         ((now_us - pulse_data.last_pulse_us) < (SPEED_UPDATE_MS * 2000UL)));

    // Zero speed if no signal
    if (!data.signal_active) {
        current_speed = 0.0f;
        smoothed_speed_kmh = 0.0f;
    } else {
        // Apply EMA smoothing (delta limiting is handled at sub-update level)
        smoothed_speed_kmh = SPEED_EMA_ALPHA * current_speed + (1.0f - SPEED_EMA_ALPHA) * smoothed_speed_kmh;
        current_speed = smoothed_speed_kmh;
    }

    // Update top speed (using filtered value)
    if (current_speed > top_speed_kmh) {
        top_speed_kmh = current_speed;
    }

    // Update min interval tracking
    if (pulse_data.min_interval_us != 0xFFFFFFFF) {
        last_min_interval_us = pulse_data.min_interval_us;
    }

    data.current_speed_kmh = current_speed;
    data.top_speed_kmh = top_speed_kmh;
    data.min_interval_us = last_min_interval_us;

    return data;
}

void speed_calculator_reset_top() {
    top_speed_kmh = 0.0f;
}
