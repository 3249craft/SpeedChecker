#include "config.h"
#include "sensor.h"
#include "speed_calculator.h"
#include "display.h"

void setup() {
    if (!display_init()) {
        while (true); // OLED not found
    }
    display_show_splash();
    sensor_init();
    speed_calculator_init();
}

void loop() {
    static unsigned long last_update_ms = 0;
    unsigned long now_ms = millis();

    if (now_ms - last_update_ms >= SPEED_UPDATE_MS) {
        unsigned long window_ms = now_ms - last_update_ms;
        PulseData pulse_data = sensor_read_and_reset();
        SpeedData speed_data = calculate_speed(pulse_data, window_ms, micros());
        display_draw_speed(speed_data);
        last_update_ms = now_ms;
    }
}
