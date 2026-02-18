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

    // Update speed calculations at defined interval
    if (now_ms - last_update_ms >= SPEED_UPDATE_MS) {
        unsigned long window_ms = now_ms - last_update_ms;
        PulseData pulse_data = sensor_read_and_reset();
        SpeedData speed_data = calculate_speed(pulse_data, window_ms, micros());
        last_speed_data = speed_data;  // Store for button actions

        // Update menu state
        menu_update(speed_data, now_ms);

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
