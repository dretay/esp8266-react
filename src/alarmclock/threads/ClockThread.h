#pragma once 

#include "CThread.h"
#include <Arduino.h>
#include <types.h>
#include <ugfx-config-uc1698.h>

struct clockThread
{
	CThread*(*initialize)(u8 priority);	
};

extern const struct clockThread ClockThread;