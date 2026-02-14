#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "config.h"
#include "speed_calculator.h"

// Menu identifiers
enum MenuId : uint8_t {
    MENU_SPEEDOMETER = 0,
#if SCREEN_MODE == 0
    MENU_DYNO_GRAPH = 1,
    MENU_STOPWATCH = 2,
#else
    // Dyno disabled on smaller screens
    MENU_STOPWATCH = 1,
#endif
#if DEBUG_MODE
  #if SCREEN_MODE == 0
    MENU_DEBUG = 3,
    MENU_COUNT = 4
  #else
    MENU_DEBUG = 2,
    MENU_COUNT = 3
  #endif
#else
  #if SCREEN_MODE == 0
    MENU_COUNT = 3
  #else
    MENU_COUNT = 2  // Speedometer + Stopwatch only on small screen
  #endif
#endif
};

// Speed display units
enum SpeedUnit : uint8_t {
    UNIT_KMH = 0,
    UNIT_MPH = 1,
    UNIT_MPS = 2,
    UNIT_RPM = 3,
    UNIT_SPS = 4,
    UNIT_COUNT = 5
};

// Dyno graph state
enum DynoState : uint8_t {
    DYNO_IDLE = 0,
    DYNO_RECORDING = 1,
    DYNO_COMPLETE = 2
};

// Stopwatch state
enum SwState : uint8_t {
    SW_STOPPED = 0,
    SW_RUNNING = 1
};

// Speedometer menu state
struct SpeedometerState {
    SpeedUnit current_unit;
    float max_speed_kmh;
    float current_acceleration_mps2;  // Live acceleration in m/s²
    float prev_speed_kmh;             // Previous speed for delta calculation
    unsigned long last_update_ms;     // Timestamp for time delta
    bool tt_measuring;                // Currently measuring time-to-speed
    unsigned long tt_start_ms;        // When wheel started spinning
    unsigned long tt15_ms;            // Time to 15 km/h (0 = not reached)
    unsigned long tt30_ms;            // Time to 30 km/h (0 = not reached)
    unsigned long idle_start_ms;      // When speed first dropped to 0
};

// Dyno graph menu state
struct DynoGraphState {
    DynoState state;
    uint8_t sample_count;
    float speed_samples[DYNO_SAMPLES];
    unsigned long start_time_ms;
    unsigned long last_sample_ms;
    uint8_t y_scale_index;           // Current Y-axis scale index
    float current_speed_kmh;          // Current speed during run
    float current_acceleration_mps2;  // Current acceleration in m/s²
    float max_acceleration_mps2;      // Maximum acceleration recorded
    float max_speed_kmh;              // Peak speed during run
    unsigned long time_to_peak_ms;    // Time to reach peak speed
};

// Stopwatch menu state
struct StopwatchState {
    SwState state;
    unsigned long start_time_ms;
    unsigned long elapsed_ms;
    unsigned long lap_times_ms[MAX_LAPS];
    uint8_t lap_count;
};

// Complete menu system state
struct MenuState {
    MenuId current_menu;
    SpeedometerState speedometer;
    DynoGraphState dyno;
    StopwatchState stopwatch;
};

// Initialize menu system
void menu_init();

// Get current menu state (read-only)
const MenuState& menu_get_state();

// Navigate to previous menu (Button 1)
void menu_prev();

// Navigate to next menu (Button 2)
void menu_next();

// Handle button 1 action (context-dependent)
void menu_action_button1(const SpeedData& speed_data);

// Handle button 2 action (context-dependent)
void menu_action_button2();

// Update menu state (called each loop iteration)
void menu_update(const SpeedData& speed_data, unsigned long now_ms);

// Update TT measurement with higher frequency raw speed (called every sub-window)
void menu_update_tt(float raw_speed_kmh, unsigned long now_ms);

// Update stopwatch elapsed time (for high-frequency updates)
void stopwatch_update_elapsed(unsigned long now_ms);

// Convert speed from km/h to specified unit
float menu_convert_speed(float speed_kmh, SpeedUnit unit);

// Get unit string for display
const char* menu_get_unit_string(SpeedUnit unit);

// Get menu name for header
const char* menu_get_menu_name(MenuId menu);

#endif
