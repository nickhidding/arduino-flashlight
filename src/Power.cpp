// 
// 
// 

#include "Power.h"

void Power::init(uint8_t relayPin, uint8_t relayFeedbackPin)
{
	this->relayPin = relayPin;
	this->relayFeedbackPin = relayFeedbackPin;
	pinMode(relayPin, OUTPUT);
	digitalWrite(relayPin, HIGH);

	// Get minimum voltage from eeprom
	uint8_t two = EEPROM.read(0);
	uint8_t one = EEPROM.read(1);
    this->lowVoltage = ((two << 0) & 0xFF) + ((one << 8) & 0xFFFF);
}

void Power::update()
{
	uint16_t value = analogRead(relayFeedbackPin);

	// Check if switch is on || relay is off
	if (value > 100) {
		if (status == On) {
			status = turningOff;
			digitalWrite(relayPin, LOW);
		}
	}
	else {
		if (status == turningOn) {
			status = On;
			digitalWrite(relayPin, HIGH);
		}
	}

	// Check if voltage is to low
	this->voltageLowCheck();
}

void Power::voltageLowCheck()
{
	if ((this->inputVoltage * 10) < lowVoltage) {
		lowVoltageReadings++;

		if (lowVoltageReadings > 50 && lowVoltagePowerOff) {
			this->shutdown();
		}
	}
	else {
		lowVoltageReadings = 0;
	}
}

uint16_t Power::getLowVoltage()
{
	return lowVoltage;
}

void Power::setLowVoltage(uint16_t voltage)
{
	this->lowVoltage = voltage;

	byte two = (voltage & 0xFF);
	byte one = ((voltage >> 8) & 0xFF);

	EEPROM.write(0, two);
	EEPROM.write(1, one);
}

Power::PowerStatus Power::getStatus()
{
	return this->status;
}

void Power::readVoltage()
{
	// Take voltage sample
	analogVoltageSum += analogRead(inputVoltagePin);
	analogVoltageSampleCount++;
	if (analogVoltageSampleCount >= 5) {
		// calculate the voltage
		// use 5.0 for a 5.0V ADC reference voltage
		// 5.015V is the calibrated reference voltage
		/*Serial.print("(");
		Serial.print(analogVoltageSum);
		Serial.print(" / ");
		Serial.print(analogVoltageSampleCount);
		Serial.print(" * ");
		Serial.print(Vcc);
		Serial.print(") / 1024.0 = ");*/
		inputVoltage = ((float)analogVoltageSum / (float)analogVoltageSampleCount * Vcc) / 1024.0;
		/*Serial.println(inputVoltage * 11.900);*/
		// send voltage for display on Serial Monitor
		// voltage multiplied by 11 when using voltage divider that
		// divides by 11. 10.860 is the calibrated voltage divide
		// value
		inputVoltage = inputVoltage * 11.900;
		// Reset
		analogVoltageSampleCount = 0;
		analogVoltageSum = 0;

		Vcc = readVcc() / 1000.0;
		//Serial.println(Vcc);
		//Vcc = 4.67;
	}
}

long Power::readVcc() {
	long result;
	// Read 1.1V reference against AVcc
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	delay(20); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Convert
	while (bit_is_set(ADCSRA, ADSC));
	result = ADCL;
	result |= ADCH << 8;
	result = 1113500L / result; // Back-calculate AVcc in mV
	return result;
}

float Power::getVoltage()
{
	return inputVoltage;
}

/*
Cut power to the system
*/
void Power::shutdown() {
	digitalWrite(this->relayPin, LOW);
	delay(1000);
}
