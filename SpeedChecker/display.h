#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "speed_calculator.h"

// Initialize OLED display
// Returns: true if successful, false if OLED not found
bool display_init();

// Show startup splash screen with logo and progress bar
void display_show_splash();

// Render speed data to display
void display_draw_speed(const SpeedData& data);

// Clear display
void display_clear();

#endif
