// 
// 
// 

#include "Display.h"

void Display::init(U8G2 *u8g2, Led *led, Fan *fan, Pump *pump, Power *power)
{
	this->u8g2 = u8g2;
	this->led = led;
	this->fan = fan;
	this->pump = pump;
	this->power = power;
	this->u8g2->begin();
	currentScreen = SCREEN_MAIN;
}

void Display::updateScreen()
{
	switch (currentScreen)
	{
	case 0:

		if (lastScreen != 0) {
			lastScreen = 0;
		}

		if (buttonPushed) {
			this->currentScreen = 1;
			buttonPushed = 0;
		}

		updateScreenMain();
		break;
	case 1:

		if (lastScreen != 1) {
			this->tempLowVoltage = power->getLowVoltage();
			lastScreen = 1;
		}

		if (buttonPushed) {
			if (this->tempLowVoltage != this->power->getLowVoltage()) {
				this->power->setLowVoltage(this->tempLowVoltage);
			}
			this->currentScreen = 2;
			buttonPushed = 0;
		}

		// Update voltage
		if (this->tempLowVoltage + this->encoderSteps > 0) {
			this->tempLowVoltage += this->encoderSteps;
		}
		this->encoderSteps = 0;

		updateScreenVoltage();
		break;
	case 2:

		if (lastScreen != 2) {
			lastScreen = 2;
		}

		if (buttonPushed) {
			this->currentScreen = 0;
			buttonPushed = 0;
		}

		// Update voltage
		if (this->encoderSteps != 0) {
			this->fan->setSpeedPercentage(this->fan->getSpeedPercentage() + this->encoderSteps);
			this->encoderSteps = 0;
		}

		updateScreenFan();
		break;
	default:
		break;
	}
	
}

void Display::updateScreenMain()
{
	u8g2->setFont(u8g2_font_crox1h_tr);
	u8g2->firstPage();
	do {
		drawHeader();

		u8g2->setFont(u8g2_font_crox1hb_tr);

		char fanSpeed[10];
		sprintf(fanSpeed, "Fan %d%%", this->fan->getSpeedPercentage());
		u8g2->setCursor(1, 26);
		u8g2->print(fanSpeed);

		char pumpSpeed[11];
		char on[3] = { 'O', 'n', '\0' };
		char off[4] = { 'O', 'f', 'f', '\0' };
		sprintf(pumpSpeed, "Pump %s", (this->pump->getEnabled() ? on : off));
		u8g2->setCursor(1, 37);
		u8g2->print(pumpSpeed);

		char inputAmps[20];
		char str_temp[6];
		dtostrf(this->inputAmps, 3, 3, str_temp);
		sprintf(inputAmps, "Current %sA", str_temp);
		u8g2->setCursor(1, 47);
		u8g2->print(inputAmps);

		char temperature[20];
		dtostrf(this->temperature, 3, 2, str_temp);
		sprintf(temperature, "Temp %sC", str_temp);
		u8g2->setCursor(1, 57);
		u8g2->print(temperature);

	} while (u8g2->nextPage());
}

void Display::drawHeader()
{
	u8g2->setFont(u8g2_font_crox1h_tr);
	u8g2->setCursor(1, 11);
	char ledBrightness[10];
	sprintf(ledBrightness, "Fan %d%%", this->fan->getSpeedPercentage());
	u8g2->print(ledBrightness);

	char batteryVoltage[20];
	char str_temp[6];
	dtostrf(this->inputVoltage, 3, 2, str_temp);
	sprintf(batteryVoltage, "Voltage %sV", str_temp);
	u8g2->setCursor(50, 11);
	u8g2->print(batteryVoltage);

	u8g2->drawHLine(0, 12, 128);
}

void Display::updateScreenVoltage()
{
	u8g2->setFont(u8g2_font_crox1h_tr);
	u8g2->firstPage();
	do {
		drawHeader();

		u8g2->setFont(u8g2_font_crox1hb_tr);
		u8g2->setCursor(1, 26);
		char text[23];
		sprintf(text, "Minimum input voltage");
		u8g2->print(text);

		u8g2->setFont(u8g2_font_crox3hb_tr);
		char voltage[20];
		sprintf(voltage, "%d.%dV", this->tempLowVoltage / 10, this->tempLowVoltage % 10);
		u8g2->setCursor(20, 50);
		u8g2->print(voltage);

	} while (u8g2->nextPage());
}

void Display::updateScreenFan()
{
	u8g2->setFont(u8g2_font_crox1h_tr);
	u8g2->firstPage();
	do {
		drawHeader();

		u8g2->setFont(u8g2_font_crox1hb_tr);
		u8g2->setCursor(1, 26);
		char text[23];
		sprintf(text, "Fan speed");
		u8g2->print(text);

		u8g2->setFont(u8g2_font_crox3hb_tr);
		sprintf(text, "%d%%", this->fan->getSpeedPercentage());
		u8g2->setCursor(20, 50);
		u8g2->print(text);

	} while (u8g2->nextPage());
}

void Display::setInputVoltage(float voltage)
{
	this->inputVoltage = voltage;
}

void Display::setInputAmps(float amps)
{
	this->inputAmps = amps;
}

void Display::setTemperature(float temperature)
{
	this->temperature = temperature;
}

void Display::setEnabled(bool enabled)
{
	this->enabled = enabled;
	if (enabled) {
		this->u8g2->setPowerSave(0);
	}
	else {
		this->u8g2->setPowerSave(1);
	}
}

void Display::setEncoderCount(uint16_t count)
{
	this->encoderSteps = count - this->encoderCount;
	this->encoderCount = count;
}

void Display::readInputs()
{
	int reading = digitalRead(this->encoderButtonPin);

	if (reading != lastButtonState) {
		this->lastDebounceTime = millis();
	}

	if ((millis() - this->lastDebounceTime) > 50) {
		if (reading != buttonState) {
			buttonState = reading;
			if (buttonState == LOW) {
				buttonPushed = 1;
			}
		}
	}
    
	lastButtonState = reading;
}
