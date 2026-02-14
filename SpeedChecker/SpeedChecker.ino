#include "config.h"
#include "sensor.h"
#include "speed_calculator.h"
#include "display.h"
#include "button.h"
#include "menu.h"

void setup() {
    if (!display_init()) {
        while (true); // OLED not found
    }
#if !DEBUG_MODE
    display_show_splash();
#endif
    sensor_init();
    speed_calculator_init();
    button_init();
    menu_init();
}

void loop() {
    static unsigned long last_update_ms = 0;
    static unsigned long last_display_ms = 0;
    static unsigned long last_tt_update_ms = 0;

    // Sub-window accumulation for 400ms main cycle
    static unsigned long acc_pulse_count = 0;
    static unsigned long acc_last_pulse_us = 0;
    static unsigned long acc_last_interval_us = 0;
    static unsigned long acc_min_interval_us = 0xFFFFFFFF;

    // Sub-speed accumulation for averaging
    static float sub_speed_sum = 0.0f;
    static uint8_t sub_speed_count = 0;
    static float prev_sub_speed = 0.0f;

    unsigned long now_ms = millis();

    // Process button events every loop iteration for responsive input
    ButtonEvent events = button_update();

    // Handle menu navigation (Button 3=prev, Button 4=next)
    if (events.just_pressed[2]) {
        menu_prev();
    }
    if (events.just_pressed[3]) {
        menu_next();
    }

    // Handle context-dependent button actions (Button 1 and 2)
    // Must be outside the timed block to catch the just_pressed event
    static SpeedData last_speed_data = {0};
    if (events.just_pressed[0]) {
        menu_action_button1(last_speed_data);
    }
    if (events.just_pressed[1]) {
        menu_action_button2();
    }

    // Fast sub-cycle (100ms) for precise TT measurement
    unsigned long tt_interval = SPEED_UPDATE_MS / SPEED_TT_SUB_UPDATES;
    if (now_ms - last_tt_update_ms >= tt_interval) {
        unsigned long sub_window_ms = now_ms - last_tt_update_ms;
        PulseData sub_data = sensor_read_and_reset();

        // Accumulate for main 400ms cycle
        acc_pulse_count += sub_data.pulse_count;
        if (sub_data.last_pulse_us != 0) acc_last_pulse_us = sub_data.last_pulse_us;
        if (sub_data.last_interval_us != 0) acc_last_interval_us = sub_data.last_interval_us;
        if (sub_data.min_interval_us < acc_min_interval_us) acc_min_interval_us = sub_data.min_interval_us;

        // Calculate raw speed for this sub-window
        float raw_speed = 0.0f;
        if (sub_data.pulse_count >= 2 && sub_window_ms > 0) {
            float speed_mps = (sub_data.pulse_count * DISTANCE_PER_PULSE_M) / (sub_window_ms / 1000.0f);
            raw_speed = speed_mps * 3.6f;
        } else if (sub_data.pulse_count == 1 && sub_data.last_interval_us > 0) {
            float seconds = sub_data.last_interval_us / 1000000.0f;
            raw_speed = (DISTANCE_PER_PULSE_M / seconds) * 3.6f;
        }

        // Ignore unrealistic speeds entirely (noise)
        if (raw_speed > MAX_SPEED_KMH) {
            // Don't add to average, don't update prev, don't update TT
        } else {
            // Apply delta limiter at sub-update level
            float delta = raw_speed - prev_sub_speed;
            if (delta > MAX_SPEED_DELTA_KMH) {
                raw_speed = prev_sub_speed + MAX_SPEED_DELTA_KMH;
            } else if (delta < -MAX_SPEED_DELTA_KMH) {
                raw_speed = prev_sub_speed - MAX_SPEED_DELTA_KMH;
            }

            sub_speed_sum += raw_speed;
            sub_speed_count++;
            prev_sub_speed = raw_speed;

            // Update TT measurement with 100ms precision
            menu_update_tt(raw_speed, now_ms);
        }

        last_tt_update_ms = now_ms;
    }

    // Main 400ms speed calculation cycle
    if (now_ms - last_update_ms >= SPEED_UPDATE_MS) {
        unsigned long window_ms = now_ms - last_update_ms;

        // Average valid sub-window speeds
        float avg_speed = 0.0f;
        if (sub_speed_count > 0) {
            avg_speed = sub_speed_sum / sub_speed_count;
        }

        // Build aggregated PulseData for signal detection and debug
        PulseData agg_data;
        agg_data.pulse_count = acc_pulse_count;
        agg_data.last_pulse_us = acc_last_pulse_us;
        agg_data.last_interval_us = acc_last_interval_us;
        agg_data.min_interval_us = acc_min_interval_us;

        SpeedData speed_data = calculate_speed(agg_data, avg_speed, window_ms, micros());
        last_speed_data = speed_data;  // Store for button actions

        // Update menu state
        menu_update(speed_data, now_ms);

        // Reset accumulators for next 400ms window
        acc_pulse_count = 0;
        acc_last_interval_us = 0;
        acc_min_interval_us = 0xFFFFFFFF;
        sub_speed_sum = 0.0f;
        sub_speed_count = 0;

        last_update_ms = now_ms;
    }

    // Determine display update interval based on current menu
    const MenuState& menu_state = menu_get_state();
    unsigned long display_interval = SPEED_UPDATE_MS;  // Default: 400ms
    if (menu_state.current_menu == MENU_STOPWATCH) {
        display_interval = 10;  // 10ms for high-end stopwatch updates (100 FPS)

        // Update stopwatch elapsed time every loop for smooth display
        // (menu_update only runs every 400ms, too slow for stopwatch)
        if (menu_state.stopwatch.state == SW_RUNNING) {
            // Access stopwatch state directly for high-frequency update
            extern void stopwatch_update_elapsed(unsigned long now_ms);
            stopwatch_update_elapsed(now_ms);
        }
    }

    // Render display at appropriate rate
    if (now_ms - last_display_ms >= display_interval) {
        display_draw_menu(menu_state, last_speed_data, now_ms);
        last_display_ms = now_ms;
    }
}
