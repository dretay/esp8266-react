#pragma once
#include <Arduino.h>

struct analogMux
{
	void(*initialize)(void);	
	bool(*setChannel)(int channel);	
};

extern const struct analogMux AnalogMux;