#include <Arduino.h>

#include "Thread.h"
#include <types.h>

#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceSPIRAMBuffer.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <SD.h>
#include <ESP8266SAM.h>

class VoiceThread : public Thread {
 public:
  VoiceThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
  void run();  

 private:
};
