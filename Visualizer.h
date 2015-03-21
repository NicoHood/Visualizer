/*
Copyright (c) 2014-2015 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef VISUALIZER_H
#define VISUALIZER_H

#if ARDUINO
#include <Arduino.h>
#endif
// for pixeltypes and math functions
#include <FastLED.h>

//================================================================================
// Visualizer
//================================================================================

class CVisualizer{
public:
	CVisualizer(void);
	void begin(CRGB* l, uint16_t length);
	void end(void);
	void clear(void);
	void reset(void);

	// Color management
	uint8_t fade(uint16_t current, uint16_t target);
	bool fill(CRGB color);
	bool fill(CRGB color, uint8_t brightness);
	bool fill(CHSV color);
	bool fillrainbow(uint8_t hueOffset, uint8_t brightness);
	bool dim(uint8_t brightness);

	// Visualizations
	bool heartbeat(CRGB color);
	bool rainbowswirl(uint16_t duration);

private:
	// Variables to point to the led array and led length
	CRGB * leds;
	uint16_t numLeds;

	// Variables to save temporary visualization values
	uint8_t heartbeatOffset;
	uint16_t rainbowOffset;
};

extern CVisualizer Visualizer;

//================================================================================
// Effects
//================================================================================

class CEffect{
public:
	CEffect(void);
	bool write(void);
	void reset(void);

private:
	uint32_t time;
	uint32_t interval; //TODO uint32_t?

	bool update(void);
};

#endif