#include "sensor.h"

static volatile unsigned long pulse_count = 0;
static volatile unsigned long last_pulse_us = 0;
static volatile unsigned long last_interval_us = 0;
static volatile unsigned long min_interval_us = 0xFFFFFFFF;

static void pulse_isr() {
    unsigned long now_us = micros();
    if (MIN_PULSE_INTERVAL_US > 0 &&
        last_pulse_us != 0 &&
        (now_us - last_pulse_us) < MIN_PULSE_INTERVAL_US) {
        return;
    }
    if (last_pulse_us != 0) {
        unsigned long interval_us = now_us - last_pulse_us;
        last_interval_us = interval_us;
        if (interval_us < min_interval_us) {
            min_interval_us = interval_us;
        }
    }
    last_pulse_us = now_us;
    pulse_count++;
}

void sensor_init() {
    pinMode(SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), pulse_isr, RISING);
}

PulseData sensor_read_and_reset() {
    PulseData data;
    noInterrupts();
    data.pulse_count = pulse_count;
    data.last_pulse_us = last_pulse_us;
    data.last_interval_us = last_interval_us;
    data.min_interval_us = min_interval_us;
    pulse_count = 0;
    min_interval_us = 0xFFFFFFFF;
    interrupts();
    return data;
}

unsigned long sensor_get_last_pulse_us() {
    noInterrupts();
    unsigned long val = last_pulse_us;
    interrupts();
    return val;
}
