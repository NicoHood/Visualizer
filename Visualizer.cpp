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

#include "Visualizer.h"

//================================================================================
// Visualizer
//================================================================================

CVisualizer Visualizer;

CVisualizer::CVisualizer(void){
	leds = NULL;
	numLeds = 0;
}

void CVisualizer::begin(CRGB* l, uint16_t length){
	// save pointer to led array and length
	leds = l;
	numLeds = length;

	// reset all temporary led values
	reset();
}

void CVisualizer::end(void){
	// clear led array
	clear();

	// reset all temporary led values
	reset();
}

void CVisualizer::clear(void){
	// clear led array
	fill(CRGB(0, 0, 0));
}

void CVisualizer::reset(void){
	// reset all temporary led values
	heartbeatOffset = 0;
	rainbowOffset = 0;
}

uint8_t CVisualizer::fade(uint16_t current, uint16_t target) {
	// fadein - fadeout effect

	// check if fade in/out is needed
	uint16_t borders = ((target + 1) / 16);
	if (borders < 3)
		return 255;
	uint8_t brightness;

	// Fade in
	if (current < borders)
		//brightness = sin8(current * 255L / borders / 2 - 64);
		brightness = current * 255L / borders;
	// out of range
	else if (current > target)
		brightness = 0;
	// Fade out
	else if (current > (target - borders))
		//brightness = sin8((target - current) * 255L / borders / 2 - 64);
		brightness = (target - current) * 255L / borders;
	// Max brightness
	else
		brightness = 255;

	return brightness;
}

bool CVisualizer::fill(CRGB color) {
	// fill all leds with the passed in color
	for (int i = 0; i < numLeds; i++)
		leds[i] = color;
	return true;
}

bool CVisualizer::fill(CRGB color, uint8_t brightness) {
	// fill all leds with the passed in color and dim
	color.nscale8_video(brightness);
	fill(color);
	return true;
}

bool CVisualizer::fill(CHSV color) {
	// fill all leds with the passed in color
	fill(CRGB(color));
	return true;
}

bool CVisualizer::fillrainbow(uint8_t hueOffset, uint8_t brightness) {
	// fills array with rainbow colors
	for (int i = 0; i < numLeds; i++)
		leds[i].setHSV((i * 255L / (numLeds - 1)) + hueOffset, 255, brightness);
	return true;
}


bool CVisualizer::dim(uint8_t brightness) {
	// dim all leds with the passed in value
	for (int i = 0; i < numLeds; i++)
		leds[i].nscale8_video(brightness);
	return true;
}

bool CVisualizer::heartbeat(CRGB color) {
	// blinks Leds in a heartbeat pattern
	heartbeatOffset++;

	// cos8 outputs 1-255 so we use the opposite
	color.nscale8_video(255 - cos8(heartbeatOffset));
	fill(color);
	// return true if effect is finished
	if (!heartbeatOffset)
		return true;
	else
		return false;
}

bool CVisualizer::rainbowswirl(uint16_t duration) {
	// fills array with rainbow colors and rotate
	// comes with a fadein - fadeout effect

	// this will output errors for duration = 0
	uint8_t brightness = fade(rainbowOffset, duration - 1);

	// write to led array
	fillrainbow(rainbowOffset, brightness);

	// return true if effect is finished
	rainbowOffset++;
	if (rainbowOffset == duration){
		rainbowOffset = 0;
		return true;
	}
	else
		return false;
}

//================================================================================
// Effects
//================================================================================

CEffect::CEffect(void){


}

void CEffect::begin(CRGB* l, uint16_t len, uint32_t newInterval){
	leds = l;
	numLeds = len; //TODO negative
	interval = newInterval;
}

void CEffect::reset(void){
	time = 0;
}

bool CEffect::available(void){
	return update(millis());
}

bool CEffect::available(uint32_t newTime){
	// check if interval period has elapsed
	if (newTime - time >= interval){
		time = newTime;
		return true;
	}
	else
		return false;
}

bool CEffect::update(void){
	// if interval period has elapsed update leds
	if (update())
		return write();
	else return false;
}

bool CEffect::update(uint32_t newTime){
	// if interval period has elapsed update leds
	if (available(newTime)){
		write();
		return true;
	}
	else
		return false;
}

//================================================================================
// HeartBeat
//================================================================================

CHeartBeat::CHeartBeat(void){

}

bool CHeartBeat::write(CRGB color){
	// blinks Leds in a heartbeat pattern
	// Leds are filled with the input color

	// cos8 outputs 1-255 so we use the opposite
	color.nscale8_video(255 - cos8(offset));
	fill_solid(leds, numLeds, color);
}

bool CHeartBeat::write(void){
	// blinks Leds in a heartbeat pattern
	// Leds are only faded with their original color

	// cos8 outputs 1-255 so we use the opposite
	for (int i = 0; i < numLeds; i++) //TODO unsigned?
		leds[i].nscale8_video(255 - cos8(offset));
}

bool CHeartBeat::next(void){
	offset++;

	// return true if effect finished
	if (!offset)
		return true;
	else
		return false;
}

bool CHeartBeat::finished(void){
	// return true if effect finished
	if (!offset)
		return true;
	else
		return false;
}

void CHeartBeat::end(void){
	//TODO?

}