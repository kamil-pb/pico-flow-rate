#include <Wire.h>
#include <SSD1306.h>

// Pump volume in mL.
#define PUMP_VOLUME_ML 0.285

// Program state enumerations.
// MAGNET_NOT_PRESENT
//     --> MAGNET_PRESENT
//     --> MAGNET_PRESENT_HOLD
//     --> MAGNET_NOT_PRESENT
typedef enum {
  MAGNET_NOT_PRESENT,
  MAGNET_PRESENT,
  MAGNET_PRESENT_HOLD
} state_t;

// A handle to the SSD1306 OLED.
SSD1306 display(0x3c, 5, 4);

// The current program state.
state_t state;

// The last time that a magnet was detected.
unsigned long lastDetectionMs;

// The flow rate in mL/min.
double flowRateMlPerMin;

// The pump period in milliseconds.
unsigned long periodMs;

void setup() {
  // Initialize pin 16 for input.
  pinMode(16, INPUT);

  // Initialize display.
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  // Initialize program state.
  state = MAGNET_PRESENT;
  lastDetectionMs = millis();
  flowRateMlPerMin = 0;
}

void loop() {
  // The sensor is active low.
  int active = !digitalRead(16);

  // Determine if a state transition should occur.
  switch (state) {
    case MAGNET_NOT_PRESENT:
      if (active) {
        state = MAGNET_PRESENT;

        // Compute pump rotation period.
        unsigned long currentTimeMs = millis();
        periodMs = currentTimeMs - lastDetectionMs;
        lastDetectionMs = currentTimeMs;

        // Compute flow rate and convert to mL/min.
        flowRateMlPerMin = PUMP_VOLUME_ML / periodMs * 1000.0 * 60.0;
      }

      break;
    case MAGNET_PRESENT:
    case MAGNET_PRESENT_HOLD:
      if (active) {
        state = MAGNET_PRESENT_HOLD;
      } else {
        state = MAGNET_NOT_PRESENT;
      }

      break;
  }

  // Display the value on the OLED.
  display.clear();
  display.drawString(0, 0, "Flow Rate (mL/min): " + String(flowRateMlPerMin));
  display.drawString(0, 8, "RPM: " + String(1000.0 / periodMs * 60));
  display.drawString(0, 16, "Sensor Value: " + String(value));
  display.display();
}

