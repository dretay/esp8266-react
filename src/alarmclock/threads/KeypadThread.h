#pragma once 

#include "CThread.h"
#include <Arduino.h>
#include <types.h>

#define KEY_TASKDELAY_TIME 5UL
#define KEY_DEBOUNCE_TIME 50UL
#define KEY_LONGPRESS_TIME 1000UL
#define KEY_LONGPRESS_REPEAT_TIME 1000UL
#define KEY_LONGPRESS_BOOST_TIME 50UL
#define KEY_LONGPRESS_BOOST_THRESHOLD 8

#if configUSE_16_BIT_TICKS == 1
#define KEY_EVENTBITMASK 0x00FFU
#define KEY00 0x0001
#define KEY01 0x0002
#define KEY02 0x0004
#define KEY03 0x0008
#define KEY04 0x0010
#define KEY05 0x0020
#define KEY06 0x0040
#define KEYLONG 0x0080
#else
#define KEY_EVENTBITMASK 0x00FFFFFFU
#define KEY00 0x0001
#define KEY01 0x0002
#define KEY02 0x0004
#define KEY03 0x0008
#define KEY04 0x0010
#define KEY05 0x0020
#define KEY06 0x0040
#define KEY07 0x0080
#define KEY08 0x0100
#define KEY09 0x0200
#define KEY10 0x0400
#define KEY11 0x0800
#define KEY12 0x1000
#define KEY13 0x2000
#define KEY14 0x4000
#define KEYLONG 0x8000
#endif


//-- Extern.
extern EventGroupHandle_t keypadEventGroup;
extern QueueHandle_t keypadQueue;

struct keypadThread
{
	CThread*(*initialize)(void);	
};

extern const struct keypadThread KeypadThread;