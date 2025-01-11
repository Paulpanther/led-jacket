#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include "strip.hpp"
#include "display.hpp"

#define USE_DISPLAY 1
#define USE_STRIP 1

void setup() {
  setupDisplay();
  setupLedStrip();
}


void loop() {
  loopDisplay();
  updateLedStrip();
}
