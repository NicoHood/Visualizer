/*
 Copyright (c) 2014 NicoHood
 See the readme for credit to other people.

 Visualizer example
 */

// FastLed v2.1
#include "FastLED.h"

#include <Visualizer.h>

// How many leds in your strip?
#define NUM_LEDS 50

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 11
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() {
  // debug Serial
  Serial.begin(115200);
  Serial.println("Startup");

  // You can put this anywhere in the setup
  // Just call it before you use any visualization
  Visualizer.begin(leds, NUM_LEDS);

  // Uncomment/edit one of the following lines for your leds arrangement.
  // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, GRB>(leds, NUM_LEDS);

  // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setDither(0); // try this to disable flickering
  FastLED.show(); // needed!
}

void loop() {
  // fill all leds
  Visualizer.fill(CRGB::Blue);
  FastLED.show();
  FastLED.delay(500);

  // fill all leds and dim
  Visualizer.fill(CRGB::Red);
  Visualizer.dim(127);
  FastLED.show();
  FastLED.delay(500);

  // fill all leds and dim more efficient
  Visualizer.fill(CRGB::Orange, 50);
  FastLED.show();
  FastLED.delay(500);

  // fill all leds with HSV color
  Visualizer.fill(CHSV(HUE_PURPLE, 255, 255));
  FastLED.show();
  FastLED.delay(500);

  // blink leds in heartbeat pattern
  while (1) {
    bool end = Visualizer.heartbeat(CRGB::White);
    FastLED.show();
    if (end)
      break;
    FastLED.delay(10);
  }
  FastLED.delay(300);

  // show a colorful rainbow that turns around and fades in and out
  while (1) {
    // 256 = 1 round
    bool end = Visualizer.rainbowswirl(512);
    FastLED.show();
    if (end)
      break;
    FastLED.delay(5);
  }
  FastLED.delay(300);
}


