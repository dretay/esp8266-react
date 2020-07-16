#include <Arduino.h>

#include "Thread.h"
#include <types.h>

#include<DFPlayerMini_Fast.h>


class Mp3Thread : public Thread {
 public:
  Mp3Thread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
  void run();  

 private:
  // static void whiteOverRainbow(int whiteSpeed, int whiteLength);
};
