// 
// 
// 

#include "Pump.h"

void Pump::init()
{
	this->enabled = false;
}

void Pump::setEnabled(bool enabled)
{
	this->enabled = enabled;
}

bool Pump::getEnabled()
{
	return this->enabled;
}


