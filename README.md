# SpeedChecker

A multi-function speedometer and performance analyzer for Mini 4WD cars using Arduino.

## Overview

SpeedChecker measures the speed of Mini 4WD cars using a reflective optical sensor (QRE1113) to detect wheel spoke rotations. Features include real-time speed display with multiple units, dyno graph for acceleration analysis, and a stopwatch with lap timing.

## Hardware

| Component | Specification |
|-----------|---------------|
| Microcontroller | ATmega32U4 (Arduino Leonardo / Pro Micro) |
| Display | 128x64 or 128x32 OLED (SSD1306, I2C address 0x3C) |
| Sensor | QRE1113 reflective optical sensor |
| Buttons | 4 tactile buttons (D4, D5, D6, D7) |
| Sensor Pin | D2 (ATmega328P) or D3 (ATmega32U4) - auto-selected by processor |

## Project Structure

```
SpeedChecker/
├── SpeedChecker.ino      # Main application (orchestration)
├── config.h              # All configuration constants
├── sensor.h              # Sensor module interface
├── sensor.cpp            # Sensor implementation (ISR, pulse counting)
├── speed_calculator.h    # Speed calculation interface
├── speed_calculator.cpp  # Speed computation logic
├── button.h              # Button module interface
├── button.cpp            # Button debouncing and state management
├── menu.h                # Menu system interface
├── menu.cpp              # Menu navigation and state logic
├── display.h             # Display module interface
└── display.cpp           # OLED rendering, menus, splash screen
```

## Features

- **Multi-Menu System**: Navigate between Speedometer, Dyno Graph, and Stopwatch
- **Speed Units**: km/h, mph, m/s, RPM, signals/second
- **Dyno Graph**: 10-second acceleration graph with auto-scaling Y-axis
- **Stopwatch**: Lap timing with up to 5 laps
- **Debug Mode**: View button states and sensor diagnostics
- **Screen Modes**: Support for both 128x64 and 128x32 displays

## Configuration

Key parameters in `SpeedChecker/config.h`:

### Display Configuration
| Parameter | Value | Description |
|-----------|-------|-------------|
| `SCREEN_MODE` | 0 or 1 | 0 = 128x64 (default), 1 = 128x32 (compact) |
| `SCREEN_WIDTH` | 128 | Display width in pixels |
| `SCREEN_HEIGHT` | 64/32 | Display height (auto-set by SCREEN_MODE) |

### Sensor Configuration
| Parameter | Value | Description |
|-----------|-------|-------------|
| `WHEEL_DIAMETER_MM` | 19.0 | Wheel diameter in millimeters |
| `SPOKES_PER_REV` | 3 | Number of spokes on the paddle wheel |
| `SPEED_UPDATE_MS` | 400 | Display update interval (ms) |
| `MIN_PULSE_INTERVAL_US` | 120 | Debounce threshold (microseconds) |

### Menu Configuration
| Parameter | Value | Description |
|-----------|-------|-------------|
| `DEBUG_MODE` | false | Enable debug menu |
| `MENU_HEADER_FORMAT` | 0 | Header style: 0=centered, 1="< Name >", 2="[1/3] Name" |

## Wiring

```
Arduino            Component
---------          ---------
VCC       -------> OLED VCC, QRE1113 VCC, Buttons (pull-up)
GND       -------> OLED GND, QRE1113 GND, Buttons (switched)
SDA       -------> OLED SDA
SCL       -------> OLED SCL
D2*       -------> QRE1113 OUT  (* ATmega328P: D2, ATmega32U4: D3)
D4        -------> Button 1 (Menu Previous)
D5        -------> Button 2 (Menu Next)
D6        -------> Button 3 (Action 1)
D7        -------> Button 4 (Action 2)
```

**Note**: Buttons connect between pin and GND (active LOW with internal pull-up)

## How It Works

1. The QRE1113 sensor detects spokes passing by, generating pulses
2. An interrupt service routine captures pulse timing with debounce filtering
3. Speed is calculated using either:
   - Pulse count over the update window (when >= 2 pulses)
   - Interval between consecutive pulses (for slower speeds)
4. The menu system allows navigation between different display modes
5. Buttons control menu navigation and mode-specific functions

## Menu System

### Speedometer
- Display current speed in multiple units (km/h, mph, m/s, RPM, sig/s)
- Track and display maximum speed
- Button 3: Cycle through units
- Button 4: Reset max speed

### Dyno Graph (128x64 mode only)
- 10-second acceleration recording
- Auto-scaling Y-axis (20-120 km/h)
- Time to peak display
- Button 4: Reset graph

### Stopwatch
- Start/Stop/Reset functionality
- Lap timing (up to 5 laps)
- Button 3: Start/Stop/Reset
- Button 4: Record lap (when running)

## Display Layout

### 128x64 Mode (SCREEN_MODE = 0)
```
+------------------+--+
| Menu Name        |B4|  <- Header with menu name
+------------------+--+
|                  |B3|
| Content Area     +--+
|                  |B2|
|                  +--+
+------------------+B1+
```

### 128x32 Mode (SCREEN_MODE = 1)
```
+--------------------+
| Content (Full)     |  <- No header, no sidebar
|                    |
+--------------------+
```
- Header and button sidebar hidden
- Dyno graph disabled
- Speedometer and Stopwatch only

## Building

1. Install required libraries in Arduino IDE:
   - Go to **Sketch > Include Library > Manage Libraries**
   - Search and install **Adafruit GFX Library**
   - Search and install **Adafruit SSD1306**
2. Open `SpeedChecker/SpeedChecker.ino`
3. Select your board (e.g., Arduino Nano, Arduino Leonardo)
4. Upload

## License

MIT
