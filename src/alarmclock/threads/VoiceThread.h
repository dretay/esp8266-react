#pragma once
#include <Arduino.h>

#include "CThread.h"
#include <types.h>

#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceSPIRAMBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <SD.h>
#include <ESP8266SAM.h>
#include <time.h>

#include "../AnalogMux.h"

struct voiceThread
{
	CThread*(*initialize)(u8 priority);	
};

extern const struct voiceThread VoiceThread;