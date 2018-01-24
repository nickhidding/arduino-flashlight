// Battery.h

#ifndef _BATTERY_h
#define _BATTERY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define LEADACID = 1;
#define LION = 2;
#define LIPO = 3;

class Battery
{
protected:


public:
	void init();
};

#endif

