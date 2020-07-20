#pragma once
#include <Arduino.h>

#include "CThread.h"
#include <types.h>

#include <Adafruit_NeoPixel.h>

struct neopixelThread
{
	CThread*(*initialize)(void);	
};

extern const struct neopixelThread NeopixelThread;