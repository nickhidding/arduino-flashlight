// Display.h

#ifndef _DISPLAY_h
#define _DISPLAY_h

#include <U8g2lib.h>
#include "Led.h"
#include "Fan.h"
#include "Pump.h"
#include "Power.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define SCREEN_MAIN 0
#define SCREEN_BATTERY 1

class Display
{
protected:
	uint8_t currentScreen;
	uint8_t lastScreen;
	U8G2* u8g2;
	Led* led;
	Fan* fan;
	Pump* pump;
	Power* power;
	float inputVoltage;
	float inputAmps;
	float temperature;
	bool enabled = true;
	int16_t encoderCount;
	int8_t encoderSteps;

	uint8_t encoderButtonPin = 19;
	uint8_t buttonState;
	uint8_t buttonPushed;
	uint8_t lastButtonState = LOW;
    unsigned long lastDebounceTime = 0;

	int16_t tempLowVoltage;

public:
	void init(U8G2 *u8g2, Led *led, Fan *fan, Pump *pump, Power *power);
	void updateScreen();
	void drawHeader();
	void setInputVoltage(float voltage);
	void setInputAmps(float amps);
	void setTemperature(float temperature);
	void setEnabled(bool enabled);
	void setEncoderCount(uint16_t count);
	void readInputs();

	// Screens
	void updateScreenMain();
	void updateScreenVoltage();
	void updateScreenFan();
};

#endif

