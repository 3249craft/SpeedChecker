#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

// Firmware Info
extern const char FIRMWARE_VERSION[];
const unsigned long SPLASH_DURATION_MS = 2500;

// Sensor Configuration
const uint8_t SENSOR_PIN = 1;
const uint8_t SPOKES_PER_REV = 3;
const uint8_t EDGES_PER_SPOKE = 1;

// Wheel Geometry
const float WHEEL_DIAMETER_MM = 19.0f;
const float WHEEL_DIAMETER_M = WHEEL_DIAMETER_MM / 1000.0f;
const float DISTANCE_PER_PULSE_M = (PI * WHEEL_DIAMETER_M) / (SPOKES_PER_REV * EDGES_PER_SPOKE);

// Timing Configuration
const float MAX_SPEED_KMH = 99.0f;
const unsigned long SPEED_UPDATE_MS = 400;
const unsigned long MIN_PULSE_INTERVAL_US = 120;

// Speed Filtering
const float SPEED_EMA_ALPHA = 0.3f;        // Smoothing factor (0.0-1.0, higher = more responsive)
const float MAX_SPEED_DELTA_KMH = 15.0f;   // Max speed change per update cycle

// Button Configuration
const uint8_t BUTTON_COUNT = 4;
const uint8_t BUTTON_PINS[BUTTON_COUNT] = {4, 5, 6, 7};  // D4, D5, D6, D7
const unsigned long BUTTON_DEBOUNCE_MS = 50;

// ========== Menu Configuration ==========

// Debug Mode - shows PPS and MIN interval in SpeedoMeter
#define DEBUG_MODE false

// Menu Header Format: 0=centered, 1="< Name >", 2="[1/3] Name"
#define MENU_HEADER_FORMAT 0

// Display layout
const uint8_t MENU_HEADER_HEIGHT = 14;
const uint8_t MENU_CONTENT_Y = 16;

// Button hint sidebar (vertical on right side)
const uint8_t BUTTON_SIDEBAR_WIDTH = 10;
const uint8_t CONTENT_WIDTH = SCREEN_WIDTH - BUTTON_SIDEBAR_WIDTH;  // 118 pixels

// Dyno Graph Configuration
const float DYNO_MAX_SPEED_KMH = 20.0f;
const uint8_t DYNO_DURATION_SEC = 10;
const uint8_t DYNO_SAMPLES = 25;
const float DYNO_MIN_START_SPEED_KMH = 1.0f;  // Minimum speed to start recording

// Dyno Y-axis scaling options (in km/h)
const uint8_t DYNO_Y_SCALE_COUNT = 6;
const float DYNO_Y_SCALES[DYNO_Y_SCALE_COUNT] = {20.0f, 40.0f, 60.0f, 80.0f, 100.0f, 120.0f};
const uint8_t DYNO_DEFAULT_SCALE_INDEX = 0;  // Start with 20 km/h

// Stopwatch Configuration
const uint8_t MAX_LAPS = 5;

#endif
