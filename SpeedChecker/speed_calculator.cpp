#include "speed_calculator.h"

static float top_speed_kmh = 0.0f;
static unsigned long last_min_interval_us = 0;

void speed_calculator_init() {
    top_speed_kmh = 0.0f;
    last_min_interval_us = 0;
}

SpeedData calculate_speed(const PulseData& pulse_data,
                          unsigned long window_ms,
                          unsigned long now_us) {
    SpeedData data;
    float window_sec = window_ms / 1000.0f;
    float current_speed = 0.0f;

    // Calculate pulses per second
    if (window_sec > 0.0f) {
        data.pulses_per_second = (unsigned long)(pulse_data.pulse_count / window_sec);
    } else {
        data.pulses_per_second = 0;
    }

    // Calculate speed using pulse count method (preferred for >= 2 pulses)
    if (pulse_data.pulse_count >= 2 && window_sec > 0.0f) {
        float speed_mps = (pulse_data.pulse_count * DISTANCE_PER_PULSE_M) / window_sec;
        current_speed = speed_mps * 3.6f;
    }
    // Fall back to interval-based method for single pulse
    else if (pulse_data.last_interval_us > 0) {
        float seconds = pulse_data.last_interval_us / 1000000.0f;
        float speed_mps = DISTANCE_PER_PULSE_M / seconds;
        current_speed = speed_mps * 3.6f;
    }

    // Clamp to max speed
    if (current_speed > MAX_SPEED_KMH) {
        current_speed = MAX_SPEED_KMH;
    }

    // Update top speed
    if (current_speed > top_speed_kmh) {
        top_speed_kmh = current_speed;
    }

    // Check signal activity
    data.signal_active = (pulse_data.pulse_count > 0) ||
        ((pulse_data.last_pulse_us != 0) &&
         ((now_us - pulse_data.last_pulse_us) < (SPEED_UPDATE_MS * 2000UL)));

    // Zero speed if no signal
    if (!data.signal_active) {
        current_speed = 0.0f;
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
