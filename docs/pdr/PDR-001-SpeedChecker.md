# Product Design Requirements (PDR)

## SpeedChecker - Mini 4WD Speedometer

**Document ID:** PDR-001
**Version:** 2.1
**Date:** 2026-02-06
**Status:** Implemented

---

## 1. Product Overview

### 1.1 Purpose
SpeedChecker is a portable speedometer device designed to measure and display the real-time speed of Mini 4WD cars during testing and tuning sessions.

### 1.2 Target Users
- Mini 4WD hobbyists and racers
- Hobby shop owners for customer demonstrations
- Enthusiasts tuning motor/gear combinations

### 1.3 Use Cases
1. **Track-side testing** - Measure car speed during practice runs
2. **Tuning validation** - Compare speed before/after modifications
3. **Motor break-in** - Monitor speed consistency during motor conditioning

---

## 2. Functional Requirements

| ID | Requirement | Priority | Status |
|----|-------------|----------|--------|
| FR-01 | Measure speed from 0 to 99 km/h | Must | ✅ Implemented |
| FR-02 | Display speed with 0.1 resolution | Must | ✅ Implemented |
| FR-03 | Update display at minimum 2 Hz (500ms) | Must | ✅ Implemented (400ms) |
| FR-04 | Detect wheel rotation via reflective sensor | Must | ✅ Implemented |
| FR-05 | Show signal status indicator | Must | ✅ Implemented |
| FR-06 | Support configurable wheel diameter | Should | ✅ Implemented |
| FR-07 | Support configurable spoke count | Should | ✅ Implemented |
| FR-08 | Display diagnostic info (PPS, min interval) | Should | ✅ Implemented (Debug Mode) |
| FR-09 | Show startup splash screen with logo | Could | ✅ Implemented |
| FR-10 | Store/recall max speed per session | Could | ✅ Implemented |
| FR-11 | Multi-unit speed display (km/h, mph, m/s, RPM, sig/s) | Should | ✅ Implemented |
| FR-12 | Menu system with navigation | Must | ✅ Implemented |
| FR-13 | Dyno graph with acceleration analysis | Should | ✅ Implemented |
| FR-14 | Stopwatch with lap timing | Should | ✅ Implemented |
| FR-15 | Support 128x64 and 128x32 displays | Should | ✅ Implemented |
| FR-16 | Button interface for control | Must | ✅ Implemented |
| FR-17 | Time-to-speed measurement (TT15, TT30) | Should | ✅ Implemented |
| FR-18 | Speed gauge bar on compact display | Could | ✅ Implemented |
| FR-19 | Auto-reset Peak/TT after configurable idle | Should | ✅ Implemented |

---

## 3. Non-Functional Requirements

### 3.1 Performance
| Parameter | Requirement |
|-----------|-------------|
| Speed accuracy | ±5% at speeds > 5 km/h |
| Minimum detectable speed | 1 km/h |
| Maximum detectable speed | 99 km/h |
| Response latency | < 500ms from measurement to display |
| Pulse debounce | Filter pulses < 120 µs apart |

### 3.2 Physical
| Parameter | Specification |
|-----------|---------------|
| Form factor | Handheld, single PCB |
| Display visibility | Readable in indoor lighting |
| Power source | USB or battery (3.3V-5V) |
| Operating temp | 10°C to 40°C |

### 3.3 Usability
- No user input required during normal operation
- Automatic measurement when car passes sensor
- Clear visual indication of signal detection

---

## 4. Hardware Specifications

### 4.1 Bill of Materials (BOM)

| Item | Part | Quantity | Notes |
|------|------|----------|-------|
| MCU | ATmega32U4 (Pro Micro) | 1 | 5V/16MHz variant preferred |
| Display | SSD1306 OLED | 1 | 128x64 or 128x32, I2C interface |
| Sensor | QRE1113 | 1 | Reflective optical sensor |
| Buttons | Tactile switch | 4 | Menu navigation and control |
| Resistor | 10kΩ | 1 | Pull-up for sensor output (optional, internal pull-up used for buttons) |
| Connector | Micro USB | 1 | Power/programming |

### 4.2 Pin Assignment

| MCU Pin | Function | Connection |
|---------|----------|------------|
| D2 (ATmega328P) or D3 (ATmega32U4) | Hardware Interrupt | Sensor output (auto-selected in config.h) |
| D2 (SDA) | I2C Data | OLED SDA |
| D3 (SCL) | I2C Clock | OLED SCL |
| D4 | Digital Input | Button 1 (Menu Prev) |
| D5 | Digital Input | Button 2 (Menu Next) |
| D6 | Digital Input | Button 3 (Action 1) |
| D7 | Digital Input | Button 4 (Action 2) |
| VCC | 5V Power | All components |
| GND | Ground | All components |

### 4.3 Sensor Mounting
- Distance from wheel: 3-8mm
- Orientation: Perpendicular to wheel surface
- Detection target: Wheel spokes or reflective tape markers

---

## 5. Software Architecture

### 5.1 Main Loop
```
┌─────────────────────────────────────────┐
│              Initialization             │
│  - Setup display                        │
│  - Configure interrupt                  │
│  - Show splash screen                   │
└─────────────────┬───────────────────────┘
                  ▼
┌─────────────────────────────────────────┐
│              Main Loop                  │◄──┐
│  - Check update interval                │   │
│  - Calculate speed from pulses          │   │
│  - Update display                       │   │
│  - Reset pulse counter                  │   │
└─────────────────┬───────────────────────┘   │
                  └───────────────────────────┘

┌─────────────────────────────────────────┐
│          ISR (Interrupt)                │
│  - Debounce check                       │
│  - Record timestamp                     │
│  - Increment pulse counter              │
└─────────────────────────────────────────┘
```

### 5.2 Speed Calculation
```
Distance per pulse = (π × wheel_diameter) / spokes_per_rev

Method A (>=2 pulses in window):
  Speed = (pulse_count × distance_per_pulse) / time_window

Method B (<2 pulses):
  Speed = distance_per_pulse / last_pulse_interval
```

### 5.3 Menu System

The device implements a multi-menu navigation system:

| Menu | Description | Button Functions |
|------|-------------|------------------|
| Speedometer | Real-time speed, peak tracking, TT15/TT30, speed gauge | B3: Cycle units, B4: Reset max |
| Dyno Graph* | 10-second acceleration graph | B4: Reset graph |
| Stopwatch | Lap timer | B3: Start/Stop/Reset, B4: Record lap |
| Debug** | Button and sensor diagnostics | B1/B2: Navigate only |

*Only available in 128x64 mode
**Only when DEBUG_MODE enabled in config.h

### 5.4 Screen Mode Configuration

Two display modes supported via `SCREEN_MODE` in config.h:

| Mode | Resolution | Features | Use Case |
|------|-----------|----------|----------|
| 0 | 128x64 | Full UI with header, sidebar, all menus | Standard displays |
| 1 | 128x32 | Compact UI, no header/sidebar, dyno disabled | Smaller/cheaper displays |

**Layout Differences:**
- Mode 0: 14px header + 10px button sidebar (118px content width)
- Mode 1: Full screen content (128px width, 32px height)

**Compact Display Layout (Mode 1):**
```
┌──────────────────────────────────┐
│  12.3         km/h    Peak: 18.5│  Speed + unit + peak
│▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░│  Speed gauge bar
│──────────────────────────────────│  Separator line
│T15:1.23                T30:2.34 │  Time-to-speed
└──────────────────────────────────┘
```

### 5.5 Speedometer Features

| Feature | Description | Config |
|---------|-------------|--------|
| Peak speed | Tracks maximum speed per run | Auto-resets after idle |
| TT15 / TT30 | Time to reach 15 and 30 km/h | Starts when wheel spins (>1 km/h) |
| Speed gauge | Visual bar proportional to speed | `GAUGE_MAX_SPEED_KMH` (default 40) |
| Auto-reset | Clears Peak/TT after idle period | `AUTO_RESET_IDLE_SEC` (default 5, 0=disabled) |

---

## 6. Verification & Testing

### 6.1 Test Cases

| ID | Test | Expected Result |
|----|------|-----------------|
| T-01 | Power on | Splash screen displays, then main UI |
| T-02 | No signal | Speed shows 0.0, SIG indicator off |
| T-03 | Slow rotation (~5 km/h) | Accurate speed ±5% |
| T-04 | Fast rotation (~30 km/h) | Accurate speed ±5% |
| T-05 | Signal loss | Speed decays to 0 within 2 update cycles |
| T-06 | Noise immunity | No false readings from ambient light |

### 6.2 Calibration Procedure
1. Use motor with known RPM output
2. Attach reference wheel (known diameter)
3. Compare displayed speed vs calculated speed
4. Adjust `wheel_diameter_mm` if needed

---

## 7. Implemented Features (v2.1)

| Feature | Status | Notes |
|---------|--------|-------|
| Peak speed tracking | ✅ Implemented | Speedometer menu, auto-resets after idle |
| Time-to-speed (TT15/TT30) | ✅ Implemented | Measures time to 15 and 30 km/h |
| Speed gauge bar | ✅ Implemented | Visual bar on compact display (configurable max) |
| Auto-reset on idle | ✅ Implemented | Configurable idle timeout (default 5s, 0=disabled) |
| Lap timing | ✅ Implemented | Stopwatch menu with 5 lap memory |
| Multi-unit display | ✅ Implemented | km/h, mph, m/s, RPM, sig/s |
| Acceleration graph | ✅ Implemented | Dyno graph with auto-scaling (128x64 only) |
| Multi-display support | ✅ Implemented | 128x64 and 128x32 modes |
| Button interface | ✅ Implemented | 4-button navigation and control |

## 8. Future Enhancements

| Priority | Feature | Description |
|----------|---------|-------------|
| High | Data export | Save dyno/lap data via USB serial |
| Medium | Bluetooth output | Stream data to smartphone app |
| Medium | Custom unit calibration | User-adjustable conversion factors |
| Low | Multi-lane | Support 2-3 sensor inputs for racing |
| Low | SD card logging | Record speed data over time |
| Low | EEPROM settings | Persist user preferences across power cycles |

---

## 9. Constraints & Limitations

- **Single direction** - Cannot distinguish forward vs reverse
- **Spoke detection** - Requires visible spokes or reflective markers
- **Indoor use** - Direct sunlight may interfere with sensor
- **Speed range** - Below 1 km/h, measurements become unreliable

---

## 10. References

- [Mini 4WD Wiki](https://mini4wd.fandom.com/)
- [QRE1113 Datasheet](https://www.onsemi.com/pdf/datasheet/qre1113-d.pdf)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
