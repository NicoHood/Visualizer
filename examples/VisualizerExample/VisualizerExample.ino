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
#define DITHER      255  // try this to disable flickering
#define CORRECTION TypicalLEDStrip

// Define the array of leds
CRGB leds[NUM_LEDS];

// Effects
CHeartbeatDynamic h(&leds[0], 10, FramesPerSecond(60));
CHeartbeatSolid hS(CRGB::Blue, &leds[10], 10, FramesPerSecond(200), 0);
CRainbowswirl r(0, &leds[0], 10, FramesPerSecond(1000));

void setup() {
  // debug Serial
  Serial.begin(115200);
  Serial.println("Startup");

  // You can put this anywhere in the setup
  // Just call it before you use any visualization
  //Visualizer.begin(leds, NUM_LEDS);

  // FastLED setup
  FastLED.addLeds<CHIPSET, LED_PINS COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.setDither(DITHER);
  FastLED.show(); // needed to reset leds to zero

  // todo needed?
  h.begin();
  r.begin();
  delay(100);
}

void loop() {
  // variable to determine if leds have changed
  uint32_t time = millis(); //todo move up
  bool ledChange = false;

  // update strip color (pretending new colors were set)
  //static bool effectChange = false;
  bool colorChange = h.available();
  colorChange |= r.update(time, colorChange);

  // execute effect on the underlying colors
  // update color next time before the effect want to update/scale down brightness
  bool effectChange =  h.update(time, colorChange);

  ledChange |= colorChange;
  ledChange |= effectChange;
  bool kChange = hS.update(time);
  ledChange |= kChange;

  // update Leds
  if (ledChange) {
    FastLED.show();

    //    Serial.print(colorChange);
    //    Serial.print(" ");
    //    Serial.print(effectChange);
    //    Serial.print(" ");
    //    Serial.print(kChange);
    //    Serial.print(" ");
    //    Serial.println(leds[0].r);
    //
    //    if (r.finished())
    //      while (1);
  }

  return;
  /*

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
    */
}
