# SpeedChecker

A paddle-wheel speedometer for Mini 4WD cars using Arduino.

## Overview

SpeedChecker measures the speed of Mini 4WD cars using a reflective optical sensor (QRE1113) to detect wheel spoke rotations. Speed is displayed in real-time on a small OLED screen.

## Hardware

| Component | Specification |
|-----------|---------------|
| Microcontroller | ATmega32U4 (Arduino Leonardo / Pro Micro) |
| Display | 128x64 OLED (SSD1306, I2C address 0x3C) |
| Sensor | QRE1113 reflective optical sensor |
| Sensor Pin | D1 (TXO) - interrupt capable |

## Project Structure

```
SpeedChecker/
├── SpeedChecker.ino      # Main application (orchestration)
├── config.h              # All configuration constants
├── sensor.h              # Sensor module interface
├── sensor.cpp            # Sensor implementation (ISR, pulse counting)
├── speed_calculator.h    # Speed calculation interface
├── speed_calculator.cpp  # Speed computation logic
├── display.h             # Display module interface
└── display.cpp           # OLED rendering, splash screen, logo
```

## Configuration

Parameters in `SpeedChecker/config.h`:

| Parameter | Value | Description |
|-----------|-------|-------------|
| `WHEEL_DIAMETER_MM` | 19.0 | Wheel diameter in millimeters |
| `SPOKES_PER_REV` | 3 | Number of spokes on the paddle wheel |
| `SPEED_UPDATE_MS` | 400 | Display update interval (ms) |
| `MIN_PULSE_INTERVAL_US` | 120 | Debounce threshold (microseconds) |
| `MAX_SPEED_KMH` | 99.0 | Maximum displayed speed |

## Wiring

```
Pro Micro          Component
---------          ---------
VCC       -------> OLED VCC, QRE1113 VCC
GND       -------> OLED GND, QRE1113 GND
D2 (SDA)  -------> OLED SDA
D3 (SCL)  -------> OLED SCL
D1 (TXO)  -------> QRE1113 OUT
```

## How It Works

1. The QRE1113 sensor detects spokes passing by, generating pulses
2. An interrupt service routine captures pulse timing with debounce filtering
3. Speed is calculated using either:
   - Pulse count over the update window (when >= 2 pulses)
   - Interval between consecutive pulses (for slower speeds)
4. The OLED displays: current speed (km/h), signal status, pulses per second, and minimum pulse interval

## Display Layout

```
+------------------------+
| XX.X km/h          SIG |  <- Speed + signal indicator
|                        |
| PPS XX  MIN XXus       |  <- Debug info
|                        |
|                        |
+------------------------+
```

## Building

1. Install required libraries in Arduino IDE:
   - Adafruit GFX Library
   - Adafruit SSD1306
2. Open `SpeedChecker/SpeedChecker.ino`
3. Select board: Arduino Leonardo (or Pro Micro)
4. Upload

## License

MIT
