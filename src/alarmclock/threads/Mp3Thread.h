#pragma once
#include <Arduino.h>

#include "CThread.h"
#include <types.h>
#include "../AnalogMux.h"

#include<DFPlayerMini_Fast.h>

struct mp3Thread
{
	CThread*(*initialize)(u8 priority);	
};

extern const struct mp3Thread Mp3Thread;