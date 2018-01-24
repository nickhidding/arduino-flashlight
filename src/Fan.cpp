// 
// 
// 

#include "Fan.h"

void Fan::init()
{
	this->speed = 0;
}

void Fan::setSpeed(uint8_t speed)
{
	if (speed <= 255 && speed >= 0)
		this->speed = speed;
}

uint8_t Fan::getSpeed()
{
	if (this->speed == 0) return 0;
	return map(this->speed, 0, 100, this->minSpeed, 255);
	//return map(this->speed, 0, 255, this->minSpeed, 255);
}

uint8_t Fan::getSpeedPercentage()
{
	return this->speed;
	//return map(this->speed, 0, 255, 0, 100);
}

void Fan::setSpeedPercentage(uint8_t speed)
{
	if (speed >= 0 && speed <= 100) {
		this->speed = speed;
	}
}
