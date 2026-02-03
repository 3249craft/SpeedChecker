#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "config.h"

struct ButtonState {
    bool pressed[BUTTON_COUNT];
};

struct ButtonEvent {
    bool just_pressed[BUTTON_COUNT];
    bool just_released[BUTTON_COUNT];
    bool held[BUTTON_COUNT];
};

void button_init();
ButtonState button_read();
ButtonEvent button_update();
ButtonState button_get_state();

#endif
