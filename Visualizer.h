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
	bool Heartbeat(CRGB color);
	bool rainbowswirl(uint16_t duration);

private:
	// Variables to point to the led array and led length
	CRGB * leds;
	uint16_t numLeds; // int?

	// Variables to save temporary visualization values
	uint8_t HeartbeatOffset;
	uint16_t rainbowOffset;
};

extern CVisualizer Visualizer;

//================================================================================
// Effects
//================================================================================

//TODO keywords/documentation
#define FramesPerSecond(f) (1000 / (f))

// speed setting limited to 2 bytes
// = max update period every 65.535s (about 1 min)
typedef uint16_t Effect_Speed_t;
typedef uint8_t Effect_NumLeds_t; //TODO settings note

#define EFFECT_DEFAULT_SPEED FramesPerSecond(60)
#define EFFECT_DIRECTION_FORTH true
#define EFFECT_DIRECTION_BACK false

/*
CEffect class contains general function for all Effects
and manages their timing and execution.
Other effects should inherit from this class.
*/

class CEffect{
public:
	inline CEffect(void) : CEffect(NULL, 0) {}
	inline CEffect(CRGB* l, Effect_NumLeds_t len) : CEffect(l, len, EFFECT_DEFAULT_SPEED) {}
	inline CEffect(CRGB* l, Effect_NumLeds_t len, Effect_Speed_t speed)
		: leds(l), numLeds(len), interval(speed), time(0) {}

	inline void begin(void) { begin(millis()); }
	void begin(const uint32_t newTime);

	void end(void);

	inline bool available(void) { return update(millis()); }
	bool available(uint32_t newTime);

	inline void next(void) { next(millis()); }
	void next(uint32_t newTime);

	virtual void reset(void) = 0;
	virtual bool write(bool step) = 0;

	inline bool update(bool forceWrite = false) { return update(millis(), forceWrite); }
	bool update(uint32_t newTime, bool forceWrite = false);

	inline void setLeds(CRGB* l) { leds = l; }
	inline void setNumLeds(Effect_NumLeds_t len) { numLeds = len; }
	inline void setSpeed(Effect_Speed_t speed) { interval = speed; }


protected:
	// variables to point to the led array, length and speed setting
	CRGB* leds;
	Effect_NumLeds_t numLeds; //TODO negative order, backwards?
	Effect_Speed_t interval;

private:
	uint32_t time;
};

//================================================================================
// Heartbeat
//================================================================================

/*
 Blinks Leds in a Heartbeat pattern
 Leds are filled with the input color
 or dimmed with the underlying color.
 */

class CHeartbeatSolid : public CEffect{
public:
	// use parent constructors
	inline CHeartbeatSolid(CRGB color) : CHeartbeatSolid(color, NULL, 0) {}
	inline CHeartbeatSolid(CRGB color, CRGB* l, Effect_NumLeds_t len) : CHeartbeatSolid(color, l, len, EFFECT_DEFAULT_SPEED) {}
	inline CHeartbeatSolid(CRGB color, CRGB* l, Effect_NumLeds_t len, Effect_Speed_t speed)
		: CEffect(l, len, speed), offset(0), colorSolid(color) {}

	void reset(void);
	bool write(bool step);
	bool finished(void); //TODO inline?

	inline void setColor(CRGB color) { colorSolid = color; }

protected:
	uint8_t offset;
	CRGB colorSolid;
};

class CHeartbeatDynamic : public CEffect{
public:
	// use parent constructors
	inline CHeartbeatDynamic(void) : CHeartbeatDynamic(NULL, 0) {}
	inline CHeartbeatDynamic(CRGB* l, Effect_NumLeds_t len) : CHeartbeatDynamic(l, len, EFFECT_DEFAULT_SPEED) {}
	inline CHeartbeatDynamic(CRGB* l, Effect_NumLeds_t len, Effect_Speed_t speed)
		: CEffect(l, len, speed), offset(0) {}

	void reset(void);
	bool write(bool step);
	bool finished(void); //TODO inline?

protected:
	uint8_t offset;
};

#endif