# SpeedChecker

A paddle-wheel speedometer for Mini 4WD cars using Arduino.

## Overview

SpeedChecker measures the speed of Mini 4WD cars using a reflective optical sensor (QRE1113) to detect wheel spoke rotations. Speed is displayed in real-time on a small OLED screen.

## Hardware

| Component | Specification |
|-----------|---------------|
| Microcontroller | ATmega32U4 (Arduino Leonardo / Pro Micro) |
| Display | 128x32 OLED (SSD1306, I2C address 0x3C) |
| Sensor | QRE1113 reflective optical sensor |
| Sensor Pin | D1 (TXO) - interrupt capable |

## Configuration

Default parameters in `src/SpeedChecker.ino`:

| Parameter | Value | Description |
|-----------|-------|-------------|
| `wheel_diameter_mm` | 19.0 | Wheel diameter in millimeters |
| `spokes_per_rev` | 3 | Number of spokes on the paddle wheel |
| `speed_update_ms` | 500 | Display update interval (ms) |
| `min_pulse_interval_us` | 120 | Debounce threshold (microseconds) |
| `max_speed_kmh` | 99.0 | Maximum displayed speed |

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
+------------------+
| XX.X km/h    SIG |  <- Speed + signal indicator
|                  |
| PPS XX  MIN XXus |  <- Debug info
+------------------+
```

## Building

1. Install required libraries in Arduino IDE:
   - Adafruit GFX Library
   - Adafruit SSD1306
2. Open `src/SpeedChecker.ino`
3. Select board: Arduino Leonardo (or Pro Micro)
4. Upload

## License

MIT