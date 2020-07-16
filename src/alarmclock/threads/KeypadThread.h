#pragma once
// look at http://www.atakansarioglu.com/pushme-keypad-reader-for-freertos/

#include <Arduino.h>
#include "Thread.h"
#include <types.h>

#define KEY_TASKDELAY_TIME 5UL
#define KEY_DEBOUNCE_TIME 50UL
#define KEY_LONGPRESS_TIME 1000UL
#define KEY_LONGPRESS_REPEAT_TIME 1000UL
#define KEY_LONGPRESS_BOOST_TIME 50UL
#define KEY_LONGPRESS_BOOST_THRESHOLD 8

//-- Definitions (DONT TOUCH)
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

class KeypadThread : public Thread {
 public:
  class Keypad {
   public:
    uint16_t keysPressed = 0;   // PORT POLLING register
    uint16_t keysDown = 0;      //
    uint16_t keysReleased = 0;  //
    TimerHandle_t timerDebounce;
    TimerHandle_t timerLong;
    uint8_t counterBoost = 0;
  };
  Keypad keypad;
  KeypadThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name);
  void run();
  void setChannel(int channel);

 private:
  uint16_t keypadSerialize(void);
  static void keypadDummyTimerCallback(TimerHandle_t xTimer);
  int currentChannel;
};
