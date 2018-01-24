// 
// 
// 

#include "Temperature.h"

void Temperature::init()
{
	sensors.begin();
	sensors.setWaitForConversion(false);
}

void Temperature::requestTemperature()
{
	unsigned long currentMillis = millis();
	
	// Read
	if (requested) {
		if (currentMillis - requestMillis >= waitConversionTime) {
			readMillis = currentMillis;

			this->readTemperature();
			requested = false;
		}
	}

	// Request
	if (!requested) {
		if (currentMillis - readMillis >= sensorInterval) {
			requestMillis = currentMillis;

			sensors.requestTemperatures();
			requested = true;
		}
	}
}

void Temperature::readTemperature()
{
	temperature = sensors.getTempCByIndex(0);
}

float Temperature::getTemperature()
{
	return temperature;
}
