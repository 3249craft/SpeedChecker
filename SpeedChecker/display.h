#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include "config.h"
#include "speed_calculator.h"
#include "button.h"

// Forward declaration
struct MenuState;

// Initialize OLED display
// Returns: true if successful, false if OLED not found
bool display_init();

// Show startup splash screen with logo and progress bar
void display_show_splash();

// Render speed data to display (legacy)
void display_draw_speed(const SpeedData& data);

// Render speed data with button states (legacy)
void display_draw_speed_with_buttons(const SpeedData& data, const ButtonState& buttons);

// Render current menu with all content
void display_draw_menu(const MenuState& menu, const SpeedData& data, unsigned long now_ms);

// Clear display
void display_clear();

#endif
