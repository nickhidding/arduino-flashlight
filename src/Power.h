// Power.h

#ifndef _POWER_h
#define _POWER_h

#include <EEPROM.h>

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class Power
{
public:
	typedef enum PowerStatus {
		turningOn,
		On,
		turningOff,
		Off
	};

	typedef enum PowerType {
		LeadAcid,
		Lion,
		LiPo,
		Adapter
	};

protected:
	uint8_t relayPin;
	uint8_t relayFeedbackPin;
	PowerStatus status = turningOn;
	PowerType type = LeadAcid;
	double Vcc;
	uint16_t lowVoltage = 110;
	uint8_t lowVoltageReadings = 0;
	uint8_t lowVoltagePowerOff = true;

	uint8_t inputVoltagePin = A0;
	uint16_t analogVoltageSum = 0;			// Sum of all ADC samples
	uint8_t analogVoltageSampleCount = 0;	// Keep track of the sample count
	float inputVoltage = 0.0;				// calculated voltage

public:
	void init(uint8_t relayPin, uint8_t relayFeedbackPin);
	void shutdown();
	void update();
	PowerStatus getStatus();
	void readVoltage();
	long readVcc();
	float getVoltage();
	void voltageLowCheck();
	uint16_t getLowVoltage();
	void setLowVoltage(uint16_t voltage);
};

#endif

