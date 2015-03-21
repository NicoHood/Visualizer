/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 Visualizer example
 */

#include "FastLED.h"
#include <Visualizer.h>

#define LED_PINS     // DATA_PIN, or DATA_PIN, CLOCK_PIN, or empty for SPI
#define COLOR_ORDER GRB
#define CHIPSET     LPD8806
#define NUM_LEDS    20

#define BRIGHTNESS  63 // reduce power consumption
#define DITHER      0  // try this to disable flickering
#define CORRECTION TypicalLEDStrip

// Define the array of leds
CRGB leds[NUM_LEDS];

// Effects
CHeartbeatDynamic h(leds, NUM_LEDS, FramesPerSecond(200));

void setup() {
  // debug Serial
  Serial.begin(115200);
  Serial.println("Startup");

  // You can put this anywhere in the setup
  // Just call it before you use any visualization
  Visualizer.begin(leds, NUM_LEDS);

  FastLED.addLeds<CHIPSET, LED_PINS COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(DITHER);
  FastLED.show(); // needed to reset leds to zero

  // todo needed?
  h.begin();
}

void loop() {
  // variable to determine if leds have changed
  bool ledChange = false;

  // update strip color (pretending new colors were set)
  bool colorChange = true;
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  ledChange |= colorChange;

  // execute effect on the underlying colors
  uint32_t time = millis(); //todo move up
  bool effectChange =  h.update(time, colorChange);
  if (effectChange) {
    //todo update color next time before the effect want to update/scale down brightness
  }
  ledChange |= effectChange;

  // update Leds
  if (ledChange)
    FastLED.show();

  return;


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

  // blink leds in Heartbeat pattern
  while (1) {
    bool end = Visualizer.Heartbeat(CRGB::White);
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

