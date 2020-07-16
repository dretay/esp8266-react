#include "KeypadThread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10

static int MUX_S0 = 18;
static int MUX_S1 = 5;
static int MUX_S2 = 21;
static int MUX_S3 = 19;

static int MUX_IN = 34;

QueueHandle_t keypadQueue = NULL;
EventGroupHandle_t keypadEventGroup;

void KeypadThread::keypadDummyTimerCallback(TimerHandle_t xTimer) {
}

KeypadThread::KeypadThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name) :
    // this class arguments
    Thread{_stackDepth, _priority, _name} {
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  pinMode(MUX_IN, INPUT);
  Serial.printf_P(PSTR("Keypad setup complete\n"));
}
void KeypadThread::setChannel(int channel) {
  if (currentChannel != channel) {
    digitalWrite(MUX_S0, bitRead(channel, 0));
    digitalWrite(MUX_S1, bitRead(channel, 1));
    digitalWrite(MUX_S2, bitRead(channel, 2));
    digitalWrite(MUX_S3, bitRead(channel, 3));
    currentChannel = channel;
    // Serial.printf_P(PSTR("Polling channel %d\n"),channel);
  }
}
void KeypadThread::run() {
  int pinValues[16] = {0};
  Serial.printf_P(PSTR("Beginning poll loop\n"));
  for (;;) {
    for (int i = 0; i < 15; i++) {
      setChannel(i);
      vTaskDelay(pdMS_TO_TICKS(10));
      int readValue = digitalRead(MUX_IN);
      if (readValue != pinValues[i]) {
        pinValues[i] = readValue;
        Serial.printf_P(PSTR("New value for %d: %d\n"), i, readValue);
      }
    }
  }
  vTaskDelete(NULL);
}
// void KeypadThread::run() {
//   // create timers
//   keypad.timerDebounce =
//       xTimerCreate("Debounce", (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), pdFALSE, (void*)0,
//       keypadDummyTimerCallback);
//   keypad.timerLong =
//       xTimerCreate("LongPress", (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), pdFALSE, (void*)0,
//       keypadDummyTimerCallback);

//   // Create Event Group.
//   keypadEventGroup = xEventGroupCreate();
//   xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);

//   // Create Queue.
//   keypadQueue = xQueueCreate(1, sizeof(uint32_t));

//   while (true) {
//     //-- All release events processed in the last cycle are cleared.
//     keypad.keysReleased &= KEYLONG;  // Release=0

//     // Read all the keys to variable.
//     keypad.keysPressed = keypadSerialize();

//     //-- If pressed.
//     if (keypad.keysPressed) {
//       //-- If pressed and seen for the first time.
//       if (keypad.keysPressed != keypad.keysDown) {
//         keypad.keysReleased &= ~KEYLONG;  // Long=0
//         keypad.counterBoost = 0;

//         // Set periods and reset timers.
//         xTimerChangePeriod(keypad.timerDebounce, (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//         xTimerReset(keypad.timerDebounce, portMAX_DELAY);
//         xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//         xTimerReset(keypad.timerLong, portMAX_DELAY);
//       }

//       //-- Remember what is just seen.
//       keypad.keysDown |= keypad.keysPressed;

//       //-- Longpress timeout check.
//       //-- If this is the first Longpress event or REPEAT is enabled (KEY_LONGPRESS_REPEAT_TIME > 0).
//       if (!xTimerIsTimerActive(keypad.timerLong) &&
//           (((keypad.keysReleased & KEYLONG) == 0) || KEY_LONGPRESS_REPEAT_TIME)) {
//         keypad.keysReleased = keypad.keysDown | KEYLONG;  // Release=Down, Long=1

//         // If number of REPEAT events are greater than KEY_LONGPRESS_BOOST_THRESHOLD and BOOST is enabled.
//         if (KEY_LONGPRESS_BOOST_THRESHOLD && (++keypad.counterBoost > KEY_LONGPRESS_BOOST_THRESHOLD)) {
//           keypad.counterBoost = KEY_LONGPRESS_BOOST_THRESHOLD;
//           xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_BOOST_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//           xTimerReset(keypad.timerLong, portMAX_DELAY);
//         } else {
//           xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_REPEAT_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//           xTimerReset(keypad.timerLong, portMAX_DELAY);
//         }
//       }
//     } else {
//       //-- Keypress timeout check.
//       if (!xTimerIsTimerActive(keypad.timerDebounce)) {
//         if ((keypad.keysReleased & KEYLONG) == 0) {
//           keypad.keysReleased = keypad.keysDown;  // Release=Down
//         }
//       }

//       //-- Reset all.
//       keypad.keysDown = 0;
//       keypad.keysReleased &= ~KEYLONG;  // Long=0
//       keypad.counterBoost = 0;

//       //-- Set periods and reset timers.
//       xTimerChangePeriod(keypad.timerDebounce, (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//       xTimerReset(keypad.timerDebounce, portMAX_DELAY);
//       xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
//       xTimerReset(keypad.timerLong, portMAX_DELAY);
//     }

//     // Broadcast event to notify any waiters.
//     xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);
//     xEventGroupSetBits(keypadEventGroup, (keypad.keysReleased & KEY_EVENTBITMASK));

//     // Send to queue to notify any receivers.
//     xQueueOverwrite(keypadQueue, &keypad.keysReleased);

//     // Delay.
//     vTaskDelay(KEY_TASKDELAY_TIME / portTICK_PERIOD_MS);
//   }
// }

uint16_t KeypadThread::keypadSerialize(void) {
  uint16_t result = 0;
#ifdef KEY00_pin
  result |= (mPinRead(KEY00_pin) == KEY00_active) ? KEY00 : 0;
#endif
#ifdef KEY01_pin
  result |= (mPinRead(KEY01_pin) == KEY01_active) ? KEY01 : 0;
#endif
#ifdef KEY02_pin
  result |= (mPinRead(KEY02_pin) == KEY02_active) ? KEY02 : 0;
#endif
#ifdef KEY03_pin
  result |= (mPinRead(KEY03_pin) == KEY03_active) ? KEY03 : 0;
#endif
#ifdef KEY04_pin
  result |= (mPinRead(KEY04_pin) == KEY04_active) ? KEY04 : 0;
#endif
#ifdef KEY05_pin
  result |= (mPinRead(KEY05_pin) == KEY05_active) ? KEY05 : 0;
#endif
#ifdef KEY06_pin
  result |= (mPinRead(KEY06_pin) == KEY06_active) ? KEY06 : 0;
#endif
#ifdef KEY07_pin
  result |= (mPinRead(KEY07_pin) == KEY07_active) ? KEY07 : 0;
#endif
#ifdef KEY08_pin
  result |= (mPinRead(KEY08_pin) == KEY08_active) ? KEY08 : 0;
#endif
#ifdef KEY09_pin
  result |= (mPinRead(KEY09_pin) == KEY09_active) ? KEY09 : 0;
#endif
#ifdef KEY10_pin
  result |= (mPinRead(KEY10_pin) == KEY10_active) ? KEY10 : 0;
#endif
#ifdef KEY11_pin
  result |= (mPinRead(KEY11_pin) == KEY11_active) ? KEY11 : 0;
#endif
#ifdef KEY12_pin
  result |= (mPinRead(KEY12_pin) == KEY12_active) ? KEY12 : 0;
#endif
#ifdef KEY13_pin
  result |= (mPinRead(KEY13_pin) == KEY13_active) ? KEY13 : 0;
#endif
#ifdef KEY14_pin
  result |= (mPinRead(KEY14_pin) == KEY14_active) ? KEY14 : 0;
#endif
  return result;
}