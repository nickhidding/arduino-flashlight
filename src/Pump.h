// Pump.h

#ifndef _PUMP_h
#define _PUMP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Pump
{
protected:
	bool enabled = false;

public:
	void init();
	void setEnabled(bool enabled);
	bool getEnabled();
};

#endif

