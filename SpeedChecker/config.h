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

#endif
