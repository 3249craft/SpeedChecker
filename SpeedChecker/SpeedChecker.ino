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

    // Update display at defined interval
    if (now_ms - last_update_ms >= SPEED_UPDATE_MS) {
        unsigned long window_ms = now_ms - last_update_ms;
        PulseData pulse_data = sensor_read_and_reset();
        SpeedData speed_data = calculate_speed(pulse_data, window_ms, micros());
        last_speed_data = speed_data;  // Store for button actions

        // Update menu state
        menu_update(speed_data, now_ms);

        // Render current menu
        const MenuState& menu_state = menu_get_state();
        display_draw_menu(menu_state, speed_data, now_ms);

        last_update_ms = now_ms;
    }
}
