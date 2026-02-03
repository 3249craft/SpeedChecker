# Product Design Requirements (PDR)

## SpeedChecker - Mini 4WD Speedometer

**Document ID:** PDR-001
**Version:** 1.0
**Date:** 2026-02-02
**Status:** Draft

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

| ID | Requirement | Priority |
|----|-------------|----------|
| FR-01 | Measure speed from 0 to 99 km/h | Must |
| FR-02 | Display speed with 0.1 km/h resolution | Must |
| FR-03 | Update display at minimum 2 Hz (500ms) | Must |
| FR-04 | Detect wheel rotation via reflective sensor | Must |
| FR-05 | Show signal status indicator | Must |
| FR-06 | Support configurable wheel diameter | Should |
| FR-07 | Support configurable spoke count | Should |
| FR-08 | Display diagnostic info (PPS, min interval) | Should |
| FR-09 | Show startup splash screen with logo | Could |
| FR-10 | Store/recall max speed per session | Could |

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
| Display | SSD1306 OLED 128x32 | 1 | I2C interface |
| Sensor | QRE1113 | 1 | Reflective optical sensor |
| Resistor | 10kΩ | 1 | Pull-up for sensor output |
| Connector | Micro USB | 1 | Power/programming |

### 4.2 Pin Assignment

| MCU Pin | Function | Connection |
|---------|----------|------------|
| D1 (TXO) | INT3 | Sensor output |
| D2 (SDA) | I2C Data | OLED SDA |
| D3 (SCL) | I2C Clock | OLED SCL |
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

## 7. Future Enhancements

| Priority | Feature | Description |
|----------|---------|-------------|
| High | Max speed hold | Display and store maximum recorded speed |
| Medium | Lap timing | Measure time between sensor triggers |
| Medium | Bluetooth output | Stream data to smartphone app |
| Low | Multi-lane | Support 2-3 sensor inputs for racing |
| Low | SD card logging | Record speed data over time |

---

## 8. Constraints & Limitations

- **Single direction** - Cannot distinguish forward vs reverse
- **Spoke detection** - Requires visible spokes or reflective markers
- **Indoor use** - Direct sunlight may interfere with sensor
- **Speed range** - Below 1 km/h, measurements become unreliable

---

## 9. References

- [Mini 4WD Wiki](https://mini4wd.fandom.com/)
- [QRE1113 Datasheet](https://www.onsemi.com/pdf/datasheet/qre1113-d.pdf)
- [SSD1306 Datasheet](https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf)
