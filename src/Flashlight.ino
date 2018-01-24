/*

Flashlight.ino

Digital Pins:
	13 = OLED CLK
	12 = OLED MISO
	11 = OLED MOSI
	10 = OLED CS
	9 = -
	8 = OLED RESET
	7 = OLED DC
	6 = Led
	5 = Pump
	4 = Power relay
	3 = Fan
	2 = Temperature sensor
	1 = TX
	0 = RX

Analog Pins:
	A0 = Voltage divider for battery voltage
	A1 = ACS712 Current sensor
	A2 = Relay feedback
	A3 = Encoder A
	A4 = Encoder B
	A5 = Encoder Push

*/

#include "Power.h"
#include "Temperature.h"
#include "Pump.h"
#include <U8g2lib.h>
#include "Display.h"
#include "Fan.h"
#include "Led.h"
#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Relay
Power power;
uint8_t powerrelay = 4;
uint8_t relayfeedback = A2;

// Display
U8G2_SH1106_128X64_NONAME_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 7, /* reset=*/ 8);
Display display;

// Led
Led led;
uint8_t ledPin = 6;

// Fan
Fan fan;
uint8_t fanPin = 3;

// Pump
Pump pump;
uint8_t pumpPin = 5;

// Temperature sensor
Temperature reservoirSensor;

// Measure amps with ACS712
uint8_t inputAmpsPin = A1;
uint8_t ACS712mVperAmp = 66;			// use 100 for 20A Module and 66 for 30A Module
double ACSAmps = 0;
float inputAmps = 0.0;
uint8_t inputAmpsSampleCount = 0;		// Keep track of the sample count

// Encoder
int encoderPinA = 17;
int encoderPinB = 18;
int encoderPinButton = 19;
const int8_t enctable[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };
const uint8_t detents = 3;

int8_t encoderOldstate = 3;
int encoder = 0;
int encoderVal = 0;
int encoderOld = 0;
int encoderCounter = 0;

ISR(PCINT1_vect)
{
	boolean chanA = bit_is_set(PINC, PINC3);
	boolean chanB = bit_is_set(PINC, PINC4);
	int8_t state = chanA | (chanB << 1);
	if (encoderOldstate != state) {
		encoder += enctable[state | (encoderOldstate << 2)];
		if (state == detents) {
			encoderVal = encoder >> 2;
		}
		encoderOldstate = state;
	}
}

void setup() {
	led.init();
	power.init(powerrelay, relayfeedback);
	display.init(&u8g2, &led, &fan, &pump, &power);
	display.setEnabled(true);
	reservoirSensor.init();
	Serial.begin(115200);
	pinMode(fanPin, OUTPUT);
	pinMode(pumpPin, OUTPUT);
	pinMode(15, INPUT);
	analogReference(DEFAULT);

	pinMode(A0, INPUT);

	pinMode(encoderPinA, INPUT_PULLUP);
	pinMode(encoderPinB, INPUT_PULLUP);
	pinMode(encoderPinButton, INPUT_PULLUP);

	// Interrupt for encoder pins
	PCICR |= (1 << PCIE1);		// set pin change interrupt for port C
	PCMSK1 |= (1 << PCINT11);	// interrupt 11 = analog pin 3
	PCMSK1 |= (1 << PCINT12);	// interrupt 12 = analog pin 4
	sei();						// global interrupt enable

	// Faster timer mode for pwm
	TCCR2B = (TCCR2B & 0b11111000) | 0x01;
	//TCCR1B = (TCCR0B & 0b11111000) | 0x01;
	//TCCR0B = (TCCR0B & 0b11111000) | 0x01;
}

void loop() {
	// Manage power
	power.update();
	delay(5);
	power.readVoltage();
	display.setInputVoltage(power.getVoltage());

	// Measure amps
	delay(5);
	ACSAmps += (((analogRead(inputAmpsPin) / 1024.0) * 5000 - (5000 / 2)) / ACS712mVperAmp);
	inputAmpsSampleCount++;
	if (inputAmpsSampleCount >= 3) {
		inputAmps = (ACSAmps / inputAmpsSampleCount);
		display.setInputAmps(inputAmps);
		//Serial.println(inputAmps);
		// Reset
		inputAmpsSampleCount = 0;
		ACSAmps = 0;
	}

	// Read temperature
	reservoirSensor.requestTemperature();
	display.setTemperature(reservoirSensor.getTemperature());

	if (power.getStatus() == Power::PowerStatus::On) {
		// Read led potentiometer and set brightness
		//led.readBrightnessPot(potentiometer);
		//analogWrite(ledPin, led.getBrightness());
		digitalWrite(ledPin, HIGH);

		// Write fan speed
		analogWrite(fanPin, fan.getSpeed());

		// Enable the pump if led is on
		pump.setEnabled(true);
		digitalWrite(pumpPin, pump.getEnabled());
	}
	else {
		analogWrite(ledPin, 0);
		analogWrite(fanPin, 0);
		digitalWrite(pumpPin, 0);
	}

	// Read encoder
	if (encoderVal > encoderOld) {
		encoderCounter++;
		display.setEncoderCount(encoderVal);
		encoderOld = encoderVal;
	}
	else if (encoderVal < encoderOld) {
		encoderCounter--;
		display.setEncoderCount(encoderVal);
		encoderOld = encoderVal;
	}

	// Refresh the screen
	display.readInputs();
	display.updateScreen();
}
