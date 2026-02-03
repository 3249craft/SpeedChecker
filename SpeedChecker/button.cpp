#include "button.h"

static ButtonState g_prev_state;
static ButtonState g_current_state;
static unsigned long g_last_change_ms[BUTTON_COUNT];

void button_init() {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        g_prev_state.pressed[i] = false;
        g_current_state.pressed[i] = false;
        g_last_change_ms[i] = 0;
    }
}

ButtonState button_read() {
    ButtonState state;
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        // INPUT_PULLUP: LOW when pressed, HIGH when released
        state.pressed[i] = (digitalRead(BUTTON_PINS[i]) == LOW);
    }
    return state;
}

ButtonEvent button_update() {
    ButtonEvent events;
    unsigned long now_ms = millis();
    ButtonState raw_state = button_read();

    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        events.just_pressed[i] = false;
        events.just_released[i] = false;
        events.held[i] = false;

        // Check if raw state differs from debounced state
        if (raw_state.pressed[i] != g_current_state.pressed[i]) {
            // Apply debounce: only change if enough time has passed
            if ((now_ms - g_last_change_ms[i]) >= BUTTON_DEBOUNCE_MS) {
                g_prev_state.pressed[i] = g_current_state.pressed[i];
                g_current_state.pressed[i] = raw_state.pressed[i];
                g_last_change_ms[i] = now_ms;

                // Detect edges
                if (g_current_state.pressed[i] && !g_prev_state.pressed[i]) {
                    events.just_pressed[i] = true;
                } else if (!g_current_state.pressed[i] && g_prev_state.pressed[i]) {
                    events.just_released[i] = true;
                }
            }
        }

        events.held[i] = g_current_state.pressed[i];
    }

    return events;
}

ButtonState button_get_state() {
    return g_current_state;
}
