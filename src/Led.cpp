// 
// Led control
// 

#include "Led.h"

void Led::init()
{
	this->brightness = 0;

	for (int thisReading = 0; thisReading < 10; thisReading++) {
		readings[thisReading] = 0;
	}
}

void Led::setBrightness(uint8_t brightness)
{
	// Check boundaries
	if (brightness < 0 && brightness > 255)
		return;

	// Smoothing
	//if (brightness == this->brightness + 1 || brightness == this->brightness - 1)
	//	return;

	this->brightness = brightness;
}

uint8_t Led::getBrightness()
{
	return this->brightness;
}

uint8_t Led::getBrightnessPercentage()
{
	return map(this->brightness, 0, 255, 0, 100);
}

void Led::readBrightnessPot(uint8_t potPin)
{
	total = total - readings[readIndex];
	readings[readIndex] = analogRead(potPin);
	total = total + readings[readIndex];
	readIndex = readIndex + 1;
	if (readIndex >= 10) {
		readIndex = 0;
	}
	average = total / 10;

	/*String string = String("|");

	int value = map(average, 0, 1023, 0, 50);
	for (int i = 0; i < 50; i++) {
		if (value > i) {
			string = string + String("!");
		}
		else {
			string = string + String(" ");
		}
	}

	string = string + String("|");
	Serial.println(string);*/


	if (average < 50)
	{
		this->brightness = 0;
		return;
	}
	if (average > 950)
	{
		this->brightness = 255;
		return;
	}
	this->brightness = map(average, 50, 950, 0, 255);
}

