// Fan.h

#ifndef _FAN_h
#define _FAN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Fan
{
protected:
	uint8_t speed = 0; // 0 - 100
	uint8_t minSpeed = 100; // Minimum pwm for the fan to spin

public:
	void init();
	void setSpeed(uint8_t speed);
	uint8_t getSpeed();
	uint8_t getSpeedPercentage();
	void setSpeedPercentage(uint8_t speed);
};

#endif

