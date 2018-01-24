// Led.h

#ifndef _LED_h
#define _LED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Led
{
protected:
	uint8_t brightness = 0;

	// Smoothing
	uint16_t readings[10];
	uint8_t readIndex = 0;
	uint16_t total = 0;
	uint16_t average = 0;

public:
	void init();
	void setBrightness(uint8_t brightness);
	void readBrightnessPot(uint8_t potPin);
	uint8_t getBrightness();
	uint8_t getBrightnessPercentage();
};

#endif

