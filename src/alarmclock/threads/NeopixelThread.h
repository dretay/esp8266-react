#include <Arduino.h>

#include "Thread.h"
#include <types.h>

#include <Adafruit_NeoPixel.h>


class NeopixelThread : public Thread {
 public:
  NeopixelThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
  void run();  

 private:
  void colorWipe(uint32_t color, int wait);
};
