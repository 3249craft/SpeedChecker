#include "menu.h"
#include <string.h>

static MenuState g_menu_state;

// Menu names
static const char* const MENU_NAMES[] = {
    "SpeedoMeter",
    "Dyno Graph",
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

    // Initialize dyno graph state
    g_menu_state.dyno.state = DYNO_IDLE;
    g_menu_state.dyno.sample_count = 0;
    g_menu_state.dyno.start_time_ms = 0;
    g_menu_state.dyno.last_sample_ms = 0;
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

        case MENU_DYNO_GRAPH:
            // No action for button 3 on dyno graph
            break;

        case MENU_STOPWATCH:
            if (g_menu_state.stopwatch.state == SW_STOPPED) {
                // Start stopwatch
                g_menu_state.stopwatch.state = SW_RUNNING;
                g_menu_state.stopwatch.start_time_ms = millis();
                g_menu_state.stopwatch.lap_count = 0;
            } else if (g_menu_state.stopwatch.state == SW_RUNNING) {
                // Record lap
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

void menu_action_button2() {
    switch (g_menu_state.current_menu) {
        case MENU_SPEEDOMETER:
            // Reset max speed
            g_menu_state.speedometer.max_speed_kmh = 0.0f;
            speed_calculator_reset_top();
            break;

        case MENU_DYNO_GRAPH:
            // Reset graph
            g_menu_state.dyno.state = DYNO_IDLE;
            g_menu_state.dyno.sample_count = 0;
            memset(g_menu_state.dyno.speed_samples, 0, sizeof(g_menu_state.dyno.speed_samples));
            break;

        case MENU_STOPWATCH:
            // Reset stopwatch
            g_menu_state.stopwatch.state = SW_STOPPED;
            g_menu_state.stopwatch.elapsed_ms = 0;
            g_menu_state.stopwatch.lap_count = 0;
            memset(g_menu_state.stopwatch.lap_times_ms, 0, sizeof(g_menu_state.stopwatch.lap_times_ms));
            break;

        default:
            break;
    }
}

void menu_update(const SpeedData& speed_data, unsigned long now_ms) {
    // Update speedometer max speed
    if (speed_data.current_speed_kmh > g_menu_state.speedometer.max_speed_kmh) {
        g_menu_state.speedometer.max_speed_kmh = speed_data.current_speed_kmh;
    }

    // Update dyno graph recording (only when on dyno menu)
    if (g_menu_state.current_menu == MENU_DYNO_GRAPH) {
        switch (g_menu_state.dyno.state) {
            case DYNO_IDLE:
                // Start recording when wheel starts spinning
                if (speed_data.signal_active && speed_data.current_speed_kmh > 0.5f) {
                    g_menu_state.dyno.state = DYNO_RECORDING;
                    g_menu_state.dyno.start_time_ms = now_ms;
                    g_menu_state.dyno.last_sample_ms = now_ms;
                    g_menu_state.dyno.sample_count = 0;
                    // Record first sample
                    g_menu_state.dyno.speed_samples[0] = speed_data.current_speed_kmh;
                    g_menu_state.dyno.sample_count = 1;
                }
                break;

            case DYNO_RECORDING:
                {
                    unsigned long elapsed = now_ms - g_menu_state.dyno.start_time_ms;
                    unsigned long duration_ms = (unsigned long)DYNO_DURATION_SEC * 1000UL;

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
