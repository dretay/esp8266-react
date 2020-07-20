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

// class VoiceThread : public Thread {
//  public:
//   VoiceThread();
//   VoiceThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
//   void run();  

//  private:
// };

struct voiceThread
{
	CThread*(*initialize)(void);	
};

extern const struct voiceThread VoiceThread;