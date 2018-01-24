// Temperature.h

#ifndef _TEMPERATURE_h
#define _TEMPERATURE_h

#include <OneWire.h>
#include <DallasTemperature.h>

// Dallas one wire bus
#define ONE_WIRE_BUS 2

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Temperature
{
protected:
	float temperature;
	OneWire oneWire;
	DallasTemperature sensors;
	bool requested;
	unsigned long requestMillis = 0;
	unsigned long readMillis = 0;
	const uint16_t sensorInterval = 1000;
	const uint16_t waitConversionTime = 750;

public:
	Temperature() : oneWire(ONE_WIRE_BUS), sensors(&oneWire)
	{
	}
	void init();
	void requestTemperature();
	void readTemperature();
	float getTemperature();
};

#endif

