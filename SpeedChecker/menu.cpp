#include "menu.h"
#include <string.h>

static MenuState g_menu_state;

// Menu names
static const char* const MENU_NAMES[] = {
    "SpeedoMeter",
#if SCREEN_MODE == 0
    "Dyno Graph",
#endif
    "StopWatch",
#if DEBUG_MODE
    "Debug"
#endif
};

// Unit strings
static const char* const UNIT_STRINGS[] = {
    "km/h",
    "mph",
    "m/s",
    "rpm",
    "sig/s"
};

void menu_init() {
    g_menu_state.current_menu = MENU_SPEEDOMETER;

    // Initialize speedometer state
    g_menu_state.speedometer.current_unit = UNIT_KMH;
    g_menu_state.speedometer.max_speed_kmh = 0.0f;
    g_menu_state.speedometer.current_acceleration_mps2 = 0.0f;
    g_menu_state.speedometer.prev_speed_kmh = 0.0f;
    g_menu_state.speedometer.last_update_ms = 0;
    g_menu_state.speedometer.tt_measuring = false;
    g_menu_state.speedometer.tt_start_ms = 0;
    g_menu_state.speedometer.tt15_ms = 0;
    g_menu_state.speedometer.tt30_ms = 0;
    g_menu_state.speedometer.idle_start_ms = 0;

    // Initialize dyno graph state
    g_menu_state.dyno.state = DYNO_IDLE;
    g_menu_state.dyno.sample_count = 0;
    g_menu_state.dyno.start_time_ms = 0;
    g_menu_state.dyno.last_sample_ms = 0;
    g_menu_state.dyno.y_scale_index = DYNO_DEFAULT_SCALE_INDEX;
    g_menu_state.dyno.current_speed_kmh = 0.0f;
    g_menu_state.dyno.current_acceleration_mps2 = 0.0f;
    g_menu_state.dyno.max_acceleration_mps2 = 0.0f;
    g_menu_state.dyno.max_speed_kmh = 0.0f;
    g_menu_state.dyno.time_to_peak_ms = 0;
    memset(g_menu_state.dyno.speed_samples, 0, sizeof(g_menu_state.dyno.speed_samples));

    // Initialize stopwatch state
    g_menu_state.stopwatch.state = SW_STOPPED;
    g_menu_state.stopwatch.start_time_ms = 0;
    g_menu_state.stopwatch.elapsed_ms = 0;
    g_menu_state.stopwatch.lap_count = 0;
    memset(g_menu_state.stopwatch.lap_times_ms, 0, sizeof(g_menu_state.stopwatch.lap_times_ms));
}

const MenuState& menu_get_state() {
    return g_menu_state;
}

void menu_prev() {
    if (g_menu_state.current_menu == 0) {
        g_menu_state.current_menu = (MenuId)(MENU_COUNT - 1);
    } else {
        g_menu_state.current_menu = (MenuId)(g_menu_state.current_menu - 1);
    }
}

void menu_next() {
    g_menu_state.current_menu = (MenuId)((g_menu_state.current_menu + 1) % MENU_COUNT);
}

void menu_action_button1(const SpeedData& speed_data) {
    switch (g_menu_state.current_menu) {
        case MENU_SPEEDOMETER:
            // Cycle through units
            g_menu_state.speedometer.current_unit =
                (SpeedUnit)((g_menu_state.speedometer.current_unit + 1) % UNIT_COUNT);
            break;

#if SCREEN_MODE == 0
        case MENU_DYNO_GRAPH:
            // No action for button 3 on dyno graph
            break;
#endif

        case MENU_STOPWATCH:
            if (g_menu_state.stopwatch.state == SW_STOPPED) {
                if (g_menu_state.stopwatch.elapsed_ms == 0) {
                    // Start stopwatch (first press when reset)
                    g_menu_state.stopwatch.state = SW_RUNNING;
                    g_menu_state.stopwatch.start_time_ms = millis();
                    g_menu_state.stopwatch.lap_count = 0;
                } else {
                    // Reset stopwatch (press again after stopping)
                    g_menu_state.stopwatch.elapsed_ms = 0;
                    g_menu_state.stopwatch.lap_count = 0;
                    memset(g_menu_state.stopwatch.lap_times_ms, 0, sizeof(g_menu_state.stopwatch.lap_times_ms));
                }
            } else if (g_menu_state.stopwatch.state == SW_RUNNING) {
                // Stop stopwatch (keep elapsed time for display)
                g_menu_state.stopwatch.state = SW_STOPPED;
                g_menu_state.stopwatch.elapsed_ms = millis() - g_menu_state.stopwatch.start_time_ms;
            }
            break;

        default:
            break;
    }
}

void menu_action_button2() {
    switch (g_menu_state.current_menu) {
        case MENU_SPEEDOMETER:
            // Reset max speed
            g_menu_state.speedometer.max_speed_kmh = 0.0f;
            speed_calculator_reset_top();
            break;

#if SCREEN_MODE == 0
        case MENU_DYNO_GRAPH:
            // Reset graph
            g_menu_state.dyno.state = DYNO_IDLE;
            g_menu_state.dyno.sample_count = 0;
            g_menu_state.dyno.y_scale_index = DYNO_DEFAULT_SCALE_INDEX;
            g_menu_state.dyno.current_speed_kmh = 0.0f;
            g_menu_state.dyno.current_acceleration_mps2 = 0.0f;
            g_menu_state.dyno.max_acceleration_mps2 = 0.0f;
            g_menu_state.dyno.max_speed_kmh = 0.0f;
            g_menu_state.dyno.time_to_peak_ms = 0;
            memset(g_menu_state.dyno.speed_samples, 0, sizeof(g_menu_state.dyno.speed_samples));
            break;
#endif

        case MENU_STOPWATCH:
            // Record lap (only when running)
            if (g_menu_state.stopwatch.state == SW_RUNNING) {
                if (g_menu_state.stopwatch.lap_count < MAX_LAPS) {
                    unsigned long elapsed = millis() - g_menu_state.stopwatch.start_time_ms;
                    g_menu_state.stopwatch.lap_times_ms[g_menu_state.stopwatch.lap_count] = elapsed;
                    g_menu_state.stopwatch.lap_count++;
                }
            }
            break;

        default:
            break;
    }
}

void stopwatch_update_elapsed(unsigned long now_ms) {
    // Fast update for stopwatch elapsed time (called every loop for smooth display)
    if (g_menu_state.stopwatch.state == SW_RUNNING) {
        g_menu_state.stopwatch.elapsed_ms = now_ms - g_menu_state.stopwatch.start_time_ms;
    }
}

void menu_update(const SpeedData& speed_data, unsigned long now_ms) {
    // Calculate live acceleration for speedometer
    if (g_menu_state.speedometer.last_update_ms > 0) {
        float time_delta_s = (now_ms - g_menu_state.speedometer.last_update_ms) / 1000.0f;
        if (time_delta_s > 0.0f) {
            float speed_delta_mps = (speed_data.current_speed_kmh - g_menu_state.speedometer.prev_speed_kmh) / 3.6f;
            g_menu_state.speedometer.current_acceleration_mps2 = speed_delta_mps / time_delta_s;
        }
    }
    g_menu_state.speedometer.prev_speed_kmh = speed_data.current_speed_kmh;
    g_menu_state.speedometer.last_update_ms = now_ms;

    // Update speedometer max speed
    if (speed_data.current_speed_kmh > g_menu_state.speedometer.max_speed_kmh) {
        g_menu_state.speedometer.max_speed_kmh = speed_data.current_speed_kmh;
    }

    // Time-to-speed measurement (TT15, TT30)
    if (!g_menu_state.speedometer.tt_measuring) {
        // Start new measurement when wheel begins spinning
        if (speed_data.current_speed_kmh > 1.0f) {
            g_menu_state.speedometer.tt_measuring = true;
            g_menu_state.speedometer.tt_start_ms = now_ms;
            g_menu_state.speedometer.tt15_ms = 0;
            g_menu_state.speedometer.tt30_ms = 0;
        }
    } else {
        unsigned long elapsed = now_ms - g_menu_state.speedometer.tt_start_ms;
        if (g_menu_state.speedometer.tt15_ms == 0 && speed_data.current_speed_kmh >= 15.0f) {
            g_menu_state.speedometer.tt15_ms = elapsed;
        }
        if (g_menu_state.speedometer.tt30_ms == 0 && speed_data.current_speed_kmh >= 30.0f) {
            g_menu_state.speedometer.tt30_ms = elapsed;
        }
        // Stop measuring when wheel stops
        if (speed_data.current_speed_kmh < 0.5f) {
            g_menu_state.speedometer.tt_measuring = false;
        }
    }

    // Auto-reset Peak/TT after idle period
    if (AUTO_RESET_IDLE_SEC > 0) {
        if (speed_data.current_speed_kmh < 0.5f) {
            if (g_menu_state.speedometer.idle_start_ms == 0) {
                g_menu_state.speedometer.idle_start_ms = now_ms;
            } else if (now_ms - g_menu_state.speedometer.idle_start_ms >= (unsigned long)AUTO_RESET_IDLE_SEC * 1000UL) {
                g_menu_state.speedometer.max_speed_kmh = 0.0f;
                g_menu_state.speedometer.tt15_ms = 0;
                g_menu_state.speedometer.tt30_ms = 0;
                g_menu_state.speedometer.idle_start_ms = 0;
            }
        } else {
            g_menu_state.speedometer.idle_start_ms = 0;
        }
    }

#if SCREEN_MODE == 0
    // Update dyno graph recording (only when on dyno menu)
    if (g_menu_state.current_menu == MENU_DYNO_GRAPH) {
        switch (g_menu_state.dyno.state) {
            case DYNO_IDLE:
                // Start recording when wheel starts spinning (using configurable threshold)
                if (speed_data.signal_active && speed_data.current_speed_kmh > DYNO_MIN_START_SPEED_KMH) {
                    g_menu_state.dyno.state = DYNO_RECORDING;
                    g_menu_state.dyno.start_time_ms = now_ms;
                    g_menu_state.dyno.last_sample_ms = now_ms;
                    g_menu_state.dyno.sample_count = 0;
                    g_menu_state.dyno.y_scale_index = DYNO_DEFAULT_SCALE_INDEX;
                    g_menu_state.dyno.current_speed_kmh = speed_data.current_speed_kmh;
                    g_menu_state.dyno.current_acceleration_mps2 = 0.0f;
                    g_menu_state.dyno.max_acceleration_mps2 = 0.0f;
                    g_menu_state.dyno.max_speed_kmh = speed_data.current_speed_kmh;
                    g_menu_state.dyno.time_to_peak_ms = 0;
                    // Record first sample
                    g_menu_state.dyno.speed_samples[0] = speed_data.current_speed_kmh;
                    g_menu_state.dyno.sample_count = 1;
                }
                break;

            case DYNO_RECORDING:
                {
                    unsigned long elapsed = now_ms - g_menu_state.dyno.start_time_ms;
                    unsigned long duration_ms = (unsigned long)DYNO_DURATION_SEC * 1000UL;

                    // Update current speed
                    float prev_speed_kmh = g_menu_state.dyno.current_speed_kmh;
                    g_menu_state.dyno.current_speed_kmh = speed_data.current_speed_kmh;

                    // Track max speed and time to peak
                    if (speed_data.current_speed_kmh > g_menu_state.dyno.max_speed_kmh) {
                        g_menu_state.dyno.max_speed_kmh = speed_data.current_speed_kmh;
                        g_menu_state.dyno.time_to_peak_ms = elapsed;
                    }

                    // Calculate acceleration (convert km/h to m/s, then divide by time)
                    // acceleration = (v2 - v1) / t, where v is in m/s
                    float speed_delta_mps = (speed_data.current_speed_kmh - prev_speed_kmh) / 3.6f;
                    float time_delta_s = (now_ms - g_menu_state.dyno.last_sample_ms) / 1000.0f;
                    if (time_delta_s > 0.0f) {
                        g_menu_state.dyno.current_acceleration_mps2 = speed_delta_mps / time_delta_s;

                        // Track max acceleration (only positive acceleration)
                        if (g_menu_state.dyno.current_acceleration_mps2 > g_menu_state.dyno.max_acceleration_mps2) {
                            g_menu_state.dyno.max_acceleration_mps2 = g_menu_state.dyno.current_acceleration_mps2;
                        }
                    }

                    // Dynamic Y-axis scaling - increase scale only when exceeding current max
                    float current_y_max = DYNO_Y_SCALES[g_menu_state.dyno.y_scale_index];
                    if (speed_data.current_speed_kmh > current_y_max) {
                        // Increase scale when exceeding current max
                        if (g_menu_state.dyno.y_scale_index < DYNO_Y_SCALE_COUNT - 1) {
                            g_menu_state.dyno.y_scale_index++;
                        }
                    }

                    // Check if recording complete
                    if (elapsed >= duration_ms) {
                        g_menu_state.dyno.state = DYNO_COMPLETE;
                    } else {
                        // Record sample at regular intervals
                        unsigned long sample_interval = duration_ms / DYNO_SAMPLES;
                        if ((now_ms - g_menu_state.dyno.last_sample_ms) >= sample_interval &&
                            g_menu_state.dyno.sample_count < DYNO_SAMPLES) {
                            g_menu_state.dyno.speed_samples[g_menu_state.dyno.sample_count] =
                                speed_data.current_speed_kmh;
                            g_menu_state.dyno.sample_count++;
                            g_menu_state.dyno.last_sample_ms = now_ms;
                        }
                    }
                }
                break;

            case DYNO_COMPLETE:
                // Stay in complete state until reset
                break;
        }
    }
#endif

    // Update stopwatch elapsed time (for display when running)
    if (g_menu_state.stopwatch.state == SW_RUNNING) {
        g_menu_state.stopwatch.elapsed_ms = now_ms - g_menu_state.stopwatch.start_time_ms;
    }
}

float menu_convert_speed(float speed_kmh, SpeedUnit unit) {
    switch (unit) {
        case UNIT_KMH:
            return speed_kmh;

        case UNIT_MPH:
            return speed_kmh * 0.621371f;

        case UNIT_MPS:
            return speed_kmh / 3.6f;

        case UNIT_RPM:
            {
                // Convert km/h to RPM using wheel circumference
                float speed_mps = speed_kmh / 3.6f;
                float circumference = PI * WHEEL_DIAMETER_M;
                float rps = speed_mps / circumference;
                return rps * 60.0f;
            }

        case UNIT_SPS:
            {
                // Signals per second = RPM * spokes * edges / 60
                float rpm = menu_convert_speed(speed_kmh, UNIT_RPM);
                return (rpm * SPOKES_PER_REV * EDGES_PER_SPOKE) / 60.0f;
            }

        default:
            return speed_kmh;
    }
}

const char* menu_get_unit_string(SpeedUnit unit) {
    if (unit < UNIT_COUNT) {
        return UNIT_STRINGS[unit];
    }
    return UNIT_STRINGS[0];
}

const char* menu_get_menu_name(MenuId menu) {
    if (menu < MENU_COUNT) {
        return MENU_NAMES[menu];
    }
    return MENU_NAMES[0];
}
