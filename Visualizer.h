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
	uint16_t numLeds; // int?

	// Variables to save temporary visualization values
	uint8_t heartbeatOffset;
	uint16_t rainbowOffset;
};

extern CVisualizer Visualizer;

//================================================================================
// Effects
//================================================================================

//TODO keywords/documentation
#define FramesPerSecond(f) (1000 / (f))

class CEffect{
public:
	CEffect(void);
	void begin(CRGB* l, uint16_t len, uint32_t newInterval);

	inline bool available(void) { return update(millis()); }
	inline bool available(uint32_t newTime){
		// check if interval period has elapsed
		bool a = (newTime - time >= interval);
		//return a; //TODO 4kb more code?
		if (a) return true;
		else return false;
	}

	// save last time we updated the leds and execute next effect step
	inline void next(void) { next(millis()); }
	inline void next(uint32_t newTime) { time = newTime; }

	virtual bool write(bool step) = 0; //TODO virtual needed? = 0?
	virtual void write(CRGB color) = 0; //TODO virtual needed? =0?

	inline bool update(bool forceWrite = false) { return update(millis(), forceWrite); }
	bool update(uint32_t newTime, bool forceWrite = false);

	inline bool update(CRGB color, bool forceWrite = false) { return update(color, millis(), forceWrite); }
	bool update(CRGB color, uint32_t newTime, bool forceWrite = false);

	void reset(void); //TODO reset time name? virtual? end()?
	virtual void end(void) = 0; //TODO?


protected: //TODO private vs protected
	uint32_t interval; //TODO uint32_t? //todo const?

	// variables to point to the led array and led length
	CRGB * leds;
	uint16_t numLeds; //TODO negative order, backwards

private:
	uint32_t time; //TODO uint32_t?
};

//================================================================================
// HeartBeat
//================================================================================

/*
 Blinks Leds in a heartbeat pattern
 Leds are filled with the input color
 or dimmed with the underlying color.
 */

class CHeartBeat : public CEffect{
public:
	CHeartBeat(void);
	bool write(bool step); //TODO virtual?
	void write(CRGB color);

	bool nextStep(void); // private/protected
	bool finished(void); //TODO inline

	void end(void);

	uint8_t offset;
};

#endif