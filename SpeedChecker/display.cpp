#include "display.h"
#include "menu.h"
#include <Wire.h>
 #include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char FIRMWARE_VERSION[] = "SpeedChkr v0.0.1b";

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const uint8_t logo_v1[] PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x9F, 0x98,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xF8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78,
    0x07, 0xF0, 0x0F, 0xF0, 0x7F, 0xC0, 0x1E, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x38,
    0x1F, 0xFC, 0x1F, 0xF8, 0x7F, 0xE0, 0x1E, 0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x70, 0x1E,
    0x3C, 0x1E, 0x1C, 0x3C, 0xF0, 0xE0, 0x3E, 0x3C, 0x38, 0x00, 0x00, 0x00, 0x00, 0x70, 0x70, 0x1F,
    0x30, 0x07, 0x18, 0x1C, 0xE0, 0x60, 0x7E, 0x38, 0x18, 0x00, 0x00, 0x00, 0x00, 0x60, 0x70, 0x1F,
    0x71, 0xD3, 0x00, 0x1C, 0x00, 0x60, 0xF6, 0x38, 0x1C, 0x3F, 0x0F, 0xC7, 0xF1, 0xFD, 0xFE, 0x1F,
    0x67, 0xF3, 0x00, 0x1C, 0x00, 0xE1, 0xE6, 0x38, 0x18, 0x7F, 0x1F, 0xC7, 0xF9, 0xFD, 0xFE, 0x3C,
    0xE6, 0x73, 0x81, 0xF8, 0x01, 0xE1, 0xC6, 0x38, 0x1C, 0xE0, 0x1C, 0x00, 0x38, 0xE0, 0x70, 0x78,
    0xEE, 0x33, 0x83, 0xF8, 0x1F, 0xC3, 0x86, 0x1F, 0xFC, 0xE0, 0x18, 0x00, 0x38, 0x60, 0x70, 0xF8,
    0xCE, 0x33, 0x81, 0xFC, 0x7F, 0x87, 0x06, 0x1F, 0xFC, 0xE0, 0x18, 0x07, 0xF8, 0x60, 0x70, 0xFC,
    0xE6, 0x3B, 0x00, 0x1C, 0xF8, 0x07, 0x0F, 0x07, 0xDC, 0xE0, 0x18, 0x0F, 0xF8, 0x60, 0x70, 0xFC,
    0x67, 0xFF, 0x00, 0x1C, 0xE0, 0x07, 0xFF, 0x80, 0x1C, 0xE0, 0x18, 0x1C, 0x38, 0x60, 0x70, 0x98,
    0x63, 0xDE, 0x00, 0x1C, 0xE0, 0x07, 0xFF, 0x80, 0x1C, 0xE0, 0x18, 0x1C, 0x38, 0x60, 0x70, 0x00,
    0x30, 0x00, 0x18, 0x1C, 0xE0, 0x00, 0x06, 0x18, 0x38, 0xE0, 0x18, 0x1C, 0x38, 0x60, 0x70, 0x00,
    0x3C, 0x00, 0x1F, 0xFC, 0xFF, 0xE0, 0x06, 0x1F, 0xF8, 0xFF, 0x18, 0x1F, 0xF8, 0x60, 0x7E, 0x00,
    0x1F, 0xF8, 0x1F, 0xF8, 0xFF, 0xE0, 0x06, 0x1F, 0xF8, 0x7F, 0x18, 0x0F, 0xF8, 0x60, 0x3E, 0x00,
    0x07, 0xF0, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

bool display_init() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        return false;
    }
    display.clearDisplay();
    return true;
}

void display_show_splash() {
    unsigned long start = millis();
    uint16_t progress = 0;
    uint8_t prng = (uint8_t)(start | 1);

    while (progress < SCREEN_WIDTH) {
        unsigned long elapsed = millis() - start;
        uint16_t target = (uint32_t)elapsed * SCREEN_WIDTH / SPLASH_DURATION_MS;
        prng ^= prng << 3;
        prng ^= prng >> 5;
        prng ^= prng << 1;
        int8_t jitter = (int8_t)(prng % 5) - 2;
        int16_t next_progress = (int16_t)target + jitter;
        if (next_progress < (int16_t)progress) {
            next_progress = progress;
        }
        if (next_progress > SCREEN_WIDTH) {
            next_progress = SCREEN_WIDTH;
        }
        progress = (uint16_t)next_progress;

        display.clearDisplay();
#if SCREEN_MODE == 0
        display.drawBitmap(0, 14, logo_v1, SCREEN_WIDTH, 32, SSD1306_WHITE);
#elif SCREEN_MODE == 1
        display.drawBitmap(0, 0, logo_v1, SCREEN_WIDTH, 32, SSD1306_WHITE);
#endif
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print(FIRMWARE_VERSION);
        display.fillRect(0, SCREEN_HEIGHT - 3, progress, 3, SSD1306_WHITE);
        display.display();

        prng ^= prng << 2;
        prng ^= prng >> 7;
        prng ^= prng << 3;
        uint8_t delay_ms = 30 + (prng % 120);
        delay(delay_ms);
    }
}

void display_draw_speed(const SpeedData& data) {
    char buf[8];
    char min_buf[12];
    char pps_buf[16];

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 0);
    dtostrf(data.current_speed_kmh, 4, 1, buf);
    display.print(buf);
    display.print("km/h");
    display.setTextSize(1);
    display.setCursor(96, 0);
    display.print(data.signal_active ? "SIG" : "NO");

    display.setTextSize(1);
    display.setCursor(0, 20);
    snprintf(pps_buf, sizeof(pps_buf), "PPS %lu", data.pulses_per_second);
    display.print(pps_buf);
    display.print(" MIN ");
    snprintf(min_buf, sizeof(min_buf), "%luus", data.min_interval_us);
    display.print(min_buf);

    display.display();
}

void display_draw_speed_with_buttons(const SpeedData& data, const ButtonState& buttons) {
    char buf[8];
    char min_buf[12];
    char pps_buf[16];

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 0);
    dtostrf(data.current_speed_kmh, 4, 1, buf);
    display.print(buf);
    display.print("km/h");
    display.setTextSize(1);
    display.setCursor(96, 0);
    display.print(data.signal_active ? "SIG" : "NO");

    display.setTextSize(1);
    display.setCursor(0, 20);
    snprintf(pps_buf, sizeof(pps_buf), "PPS %lu", data.pulses_per_second);
    display.print(pps_buf);
    display.print(" MIN ");
    snprintf(min_buf, sizeof(min_buf), "%luus", data.min_interval_us);
    display.print(min_buf);

    // Draw button indicators at bottom
    display.setCursor(0, 56);
    display.print("BTN:");
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        int16_t x = 30 + i * 20;
        int16_t y = 54;
        if (buttons.pressed[i]) {
            display.fillRect(x, y, 12, 10, SSD1306_WHITE);
        } else {
            display.drawRect(x, y, 12, 10, SSD1306_WHITE);
        }
        display.setCursor(x + 3, y + 1);
        display.setTextColor(buttons.pressed[i] ? SSD1306_BLACK : SSD1306_WHITE);
        display.print(i + 1);
        display.setTextColor(SSD1306_WHITE);
    }

    display.display();
}

void display_clear() {
    display.clearDisplay();
    display.display();
}

// ========== Menu Rendering Functions ==========

static void display_draw_header(MenuId menu_id) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const char* name = menu_get_menu_name(menu_id);
    int16_t name_width = strlen(name) * 6;

#if MENU_HEADER_FORMAT == 0
    // Centered menu name (within content area)
    int16_t x = (CONTENT_WIDTH - name_width) / 2;
    display.setCursor(x, 2);
    display.print(name);

#elif MENU_HEADER_FORMAT == 1
    // "< Menu Name >" with navigation hints (within content area)
    display.setCursor(2, 2);
    display.print("<");
    int16_t x = (CONTENT_WIDTH - name_width) / 2;
    display.setCursor(x, 2);
    display.print(name);
    display.setCursor(CONTENT_WIDTH - 10, 2);
    display.print(">");

#elif MENU_HEADER_FORMAT == 2
    // "[1/3] Menu Name" with page indicator
    char header[24];
    snprintf(header, sizeof(header), "[%d/%d] %s", menu_id + 1, MENU_COUNT, name);
    display.setCursor(2, 2);
    display.print(header);
#endif

    // Draw separator line (stop at sidebar)
    display.drawLine(0, MENU_HEADER_HEIGHT, CONTENT_WIDTH, MENU_HEADER_HEIGHT, SSD1306_WHITE);
}

static void display_draw_button_hints(const char* b1, const char* b2, const char* b3, const char* b4) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Draw vertical separator line (full height)
    const int16_t sidebar_x = CONTENT_WIDTH;
    display.drawLine(sidebar_x, 0, sidebar_x, SCREEN_HEIGHT - 1, SSD1306_WHITE);

    // Button order: 4 at top, 3, 2, 1 at bottom (matching physical layout)
    const char* hints[4] = {b4, b3, b2, b1};
    const int16_t hint_height = SCREEN_HEIGHT / 4;  // 16 pixels each

    for (uint8_t i = 0; i < 4; i++) {
        int16_t y = i * hint_height;

        // Draw single character hint centered
        if (hints[i] != nullptr && strlen(hints[i]) > 0) {
            display.setCursor(sidebar_x + 2, y + 4);
            display.print(hints[i][0]);  // Only first character
        }

        // Draw separator between buttons (except last)
        if (i < 3) {
            int16_t sep_y = y + hint_height;
            display.drawLine(sidebar_x + 1, sep_y, SCREEN_WIDTH - 1, sep_y, SSD1306_WHITE);
        }
    }
}

static void display_draw_speedometer(const SpeedData& data, const SpeedometerState& state) {
    char buf[16];

    // Convert speeds to current unit
    float current_speed = menu_convert_speed(data.current_speed_kmh, state.current_unit);
    float max_speed = menu_convert_speed(state.max_speed_kmh, state.current_unit);
    const char* unit_str = menu_get_unit_string(state.current_unit);

    // Draw current speed (large, size 2 to fit in content area)
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, MENU_CONTENT_Y + 2);
    dtostrf(current_speed, 5, 1, buf);
    display.print(buf);

    // Draw unit (smaller, next to speed)
    display.setTextSize(1);
    display.setCursor(66, MENU_CONTENT_Y + 2);
    display.print(unit_str);

    // Draw signal indicator
#if SCREEN_MODE == 1
    // Compact screen: max speed right-aligned, then separator line below
    display.setCursor(68, 12);
    display.print("Peak:");
    dtostrf(max_speed, 5, 1, buf);
    display.print(buf);
    display.drawLine(0, 23, SCREEN_WIDTH - 1, 23, SSD1306_WHITE);
    // Speed gauge: 1px line above baseline, proportional to speed (max 50 km/h)
    int16_t gauge_w = (int16_t)(data.current_speed_kmh * SCREEN_WIDTH / GAUGE_MAX_SPEED_KMH);
    if (gauge_w > SCREEN_WIDTH) gauge_w = SCREEN_WIDTH;
    if (gauge_w > 0) {
        display.drawLine(0, 22, gauge_w - 1, 22, SSD1306_WHITE);
    }
    // TT15 and TT30 at bottom
    display.setCursor(0, 25);
    display.print("T15:");
    if (state.tt15_ms > 0) {
        dtostrf(state.tt15_ms / 1000.0f, 4, 2, buf);
        display.print(buf);
    } else {
        display.print("-.--");
    }
    display.setCursor(66, 25);
    display.print("T30:");
    if (state.tt30_ms > 0) {
        dtostrf(state.tt30_ms / 1000.0f, 4, 2, buf);
        display.print(buf);
    } else {
        display.print("-.--");
    }
#else
    display.setCursor(66, MENU_CONTENT_Y + 12);
    display.print(data.signal_active ? "SIG" : "---");

    // Draw max speed indicator
    display.setTextSize(1);
    display.setCursor(0, MENU_CONTENT_Y + 22);
    display.print("Peak:");
    dtostrf(max_speed, 4, 1, buf);
    display.print(buf);
    display.print(unit_str);
#endif

#if SCREEN_MODE == 0
    // Draw acceleration (128x64 has room for an extra line)
    display.setCursor(0, MENU_CONTENT_Y + 32);
    display.print("ACC:");
    dtostrf(state.current_acceleration_mps2, 5, 1, buf);
    display.print(buf);
    display.print("m/s2");
#endif

#if DEBUG_MODE
    // Draw debug info: PPS and MIN interval
    char pps_buf[16];
    display.setCursor(0, MENU_CONTENT_Y + 42);
    snprintf(pps_buf, sizeof(pps_buf), "PPS:%lu", data.pulses_per_second);
    display.print(pps_buf);
    display.setCursor(64, MENU_CONTENT_Y + 42);
    snprintf(pps_buf, sizeof(pps_buf), "MIN:%luus", data.min_interval_us);
    display.print(pps_buf);
#endif
}

static void display_draw_dyno_graph(const DynoGraphState& state) {
    // Graph dimensions (fit within content area)
    const int16_t graph_x = 18;
    const int16_t graph_y = MENU_CONTENT_Y + 5;
    const int16_t graph_w = CONTENT_WIDTH - graph_x - 4;  // ~82 pixels
    const int16_t graph_h = 42;

    // Get current Y-axis max from scale array
    float y_max = DYNO_Y_SCALES[state.y_scale_index];

    // Draw Y-axis
    display.drawLine(graph_x, graph_y, graph_x, graph_y + graph_h, SSD1306_WHITE);
    // Draw X-axis
    display.drawLine(graph_x, graph_y + graph_h, graph_x + graph_w, graph_y + graph_h, SSD1306_WHITE);

    // Draw axis labels
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Y-axis labels with dotted grid lines - only at DYNO_Y_SCALES values
    char buf[4];
    float y_scale = (float)graph_h / y_max;

    // Draw guide lines for each DYNO_Y_SCALES value that fits in current y_max
    for (uint8_t i = 0; i < DYNO_Y_SCALE_COUNT; i++) {
        float scale_value = DYNO_Y_SCALES[i];
        if (scale_value <= y_max) {
            // Calculate Y position for this scale value
            int16_t y_pos = graph_y + graph_h - (int16_t)(scale_value * y_scale);

            // Draw label (compact format)
            snprintf(buf, sizeof(buf), "%d", (int)scale_value);
            display.setCursor(0, y_pos - 3);
            display.print(buf);

            // Draw dotted guide line
            for (int16_t x = graph_x + 2; x < graph_x + graph_w; x += 3) {
                display.drawPixel(x, y_pos, SSD1306_WHITE);
            }
        }
    }

    // Bottom label (0)
    display.setCursor(2, graph_y + graph_h - 6);
    display.print("0");

    // X-axis label (above the line)
    display.setCursor(graph_x + graph_w - 12, graph_y + graph_h - 10);
    display.print("10s");

    // Display time to peak in top-left corner (right of Y-axis)
    if (state.state == DYNO_RECORDING || state.state == DYNO_COMPLETE) {
        display.setCursor(graph_x + 4, graph_y + 2);
        display.print("TTP:");
        char ttp_buf[8];
        snprintf(ttp_buf, sizeof(ttp_buf), "%lu", state.time_to_peak_ms);
        display.print(ttp_buf);
        display.print("ms");
    }

    // Draw data points as line graph
    if (state.sample_count > 1) {
        float x_scale = (float)graph_w / (DYNO_SAMPLES - 1);
        float y_scale = (float)graph_h / y_max;

        for (uint8_t i = 1; i < state.sample_count; i++) {
            int16_t x1 = graph_x + (int16_t)((i - 1) * x_scale);
            int16_t x2 = graph_x + (int16_t)(i * x_scale);

            float speed1 = state.speed_samples[i - 1];
            float speed2 = state.speed_samples[i];
            if (speed1 > y_max) speed1 = y_max;
            if (speed2 > y_max) speed2 = y_max;

            int16_t y1 = graph_y + graph_h - (int16_t)(speed1 * y_scale);
            int16_t y2 = graph_y + graph_h - (int16_t)(speed2 * y_scale);

            display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
        }
    }
}

static void display_draw_stopwatch(const StopwatchState& state, unsigned long now_ms) {
    char time_buf[16];
    unsigned long display_ms = state.elapsed_ms;

    // Format as MM:SS.mm (shortened for space)
    unsigned long total_sec = display_ms / 1000;
    unsigned long minutes = total_sec / 60;
    unsigned long seconds = total_sec % 60;
    unsigned long centis = (display_ms % 1000) / 10;

    snprintf(time_buf, sizeof(time_buf), "%02lu:%02lu.%02lu", minutes, seconds, centis);

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // When stopped with laps, show all laps + final time
    if (state.state == SW_STOPPED && state.lap_count > 0) {
        // Show final time at top
        display.setCursor(0, MENU_CONTENT_Y + 2);
        display.print("FINAL: ");
        display.print(time_buf);

        // Show all laps (up to 5)
        for (uint8_t i = 0; i < state.lap_count; i++) {
            unsigned long lap_ms = state.lap_times_ms[i];
            unsigned long lap_sec = lap_ms / 1000;
            unsigned long lap_min = lap_sec / 60;
            unsigned long lap_s = lap_sec % 60;
            unsigned long lap_c = (lap_ms % 1000) / 10;

            snprintf(time_buf, sizeof(time_buf), "L%d %02lu:%02lu.%02lu",
                     i + 1, lap_min, lap_s, lap_c);
            display.setCursor(0, MENU_CONTENT_Y + 12 + i * 8);
            display.print(time_buf);
        }
    } else {
        // When running or stopped without laps, show large time display
        // Draw state indicator
        display.setCursor(0, MENU_CONTENT_Y + 2);
        switch (state.state) {
            case SW_STOPPED:
                display.print("STOPPED");
                break;
            case SW_RUNNING:
                display.print("RUNNING");
                break;
        }

        // Draw main time (large, size 2)
        display.setTextSize(2);
        display.setCursor(2, MENU_CONTENT_Y + 14);
        display.print(time_buf);

        // Draw last 2 laps when running
        if (state.state == SW_RUNNING && state.lap_count > 0) {
            display.setTextSize(1);
            uint8_t laps_to_show = (state.lap_count < 2) ? state.lap_count : 2;
            for (uint8_t i = 0; i < laps_to_show; i++) {
                uint8_t lap_idx = state.lap_count - laps_to_show + i;
                unsigned long lap_ms = state.lap_times_ms[lap_idx];
                unsigned long lap_sec = lap_ms / 1000;
                unsigned long lap_min = lap_sec / 60;
                unsigned long lap_s = lap_sec % 60;
                unsigned long lap_c = (lap_ms % 1000) / 10;

                snprintf(time_buf, sizeof(time_buf), "L%d %02lu:%02lu.%02lu",
                         lap_idx + 1, lap_min, lap_s, lap_c);
                display.setCursor(0, MENU_CONTENT_Y + 34 + i * 8);
                display.print(time_buf);
            }
        }
    }
}

#if DEBUG_MODE
static void display_draw_debug(const ButtonState& buttons) {
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, MENU_CONTENT_Y + 2);
    display.print("Button States:");

    // Draw button boxes with labels (fit in content area)
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        int16_t x = 4 + i * 24;
        int16_t y = MENU_CONTENT_Y + 16;

        // Draw button number
        display.setCursor(x + 5, y);
        display.print(i + 1);

        // Draw button state box
        int16_t box_y = y + 10;
        if (buttons.pressed[i]) {
            display.fillRect(x, box_y, 18, 12, SSD1306_WHITE);
            display.setTextColor(SSD1306_BLACK);
            display.setCursor(x + 3, box_y + 2);
            display.print("ON");
            display.setTextColor(SSD1306_WHITE);
        } else {
            display.drawRect(x, box_y, 18, 12, SSD1306_WHITE);
            display.setCursor(x + 1, box_y + 2);
            display.print("OFF");
        }
    }
}
#endif

void display_draw_menu(const MenuState& menu, const SpeedData& data, unsigned long now_ms) {
    display.clearDisplay();

#if SCREEN_MODE == 0
    // Draw header (only for 128x64 screens)
    display_draw_header(menu.current_menu);
#endif

    // Draw content based on current menu
    switch (menu.current_menu) {
        case MENU_SPEEDOMETER:
            display_draw_speedometer(data, menu.speedometer);
#if SCREEN_MODE == 0
            display_draw_button_hints("U", "R", "<", ">");
#endif
            break;

#if SCREEN_MODE == 0
        // Dyno graph disabled on smaller screens
        case MENU_DYNO_GRAPH:
            display_draw_dyno_graph(menu.dyno);
            display_draw_button_hints("", "R", "<", ">");
            break;
#endif

        case MENU_STOPWATCH:
            display_draw_stopwatch(menu.stopwatch, now_ms);
#if SCREEN_MODE == 0
            {
                // Show 'R' for Reset when stopped with time, otherwise 'S' for Start/Stop
                const char* btn1 = (menu.stopwatch.state == SW_STOPPED && menu.stopwatch.elapsed_ms > 0) ? "R" : "S";
                display_draw_button_hints(btn1, "L", "<", ">");
            }
#endif
            break;

#if DEBUG_MODE
        case MENU_DEBUG:
            {
                ButtonState buttons = button_get_state();
                display_draw_debug(buttons);
#if SCREEN_MODE == 0
                display_draw_button_hints("", "", "<", ">");
#endif
            }
            break;
#endif

        default:
            break;
    }

    display.display();
}
