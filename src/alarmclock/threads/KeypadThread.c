#include "KeypadThread.h"
#include <Arduino.h>

static CThread thread;

static int MUX_S0 = 18;
static int MUX_S1 = 5;
static int MUX_S2 = 21;
static int MUX_S3 = 19;
static int MUX_IN = 34;

QueueHandle_t keypadQueue = NULL;
EventGroupHandle_t keypadEventGroup = NULL;

static uint16_t keypadSerialize(void);
static int readKey(int key);

static void keypadDummyTimerCallback(TimerHandle_t xTimer) {
}
// Struct to keep the variables of the keypad module.
typedef struct {
  //-- Key events.
  uint16_t keysPressed;   // PORT POLLING register
  uint16_t keysDown;      //
  uint16_t keysReleased;  //
  uint8_t counterBoost;
  TimerHandle_t timerDebounce;
  TimerHandle_t timerLong;
} tKeypad;

// Initialize the variables.
static tKeypad keypad = {
    .keysPressed = 0,
    .keysDown = 0,
    .keysReleased = 0,
    .counterBoost = 0,
};

static void run(void* params) {
  for (;;) {
    keypad.keysReleased &= KEYLONG;  // Release=0
    // Read all the keys to variable.
    keypad.keysPressed = keypadSerialize();
    if (keypad.keysPressed) {
      //-- If pressed and seen for the first time.
      if (keypad.keysPressed != keypad.keysDown) {
        keypad.keysReleased &= ~KEYLONG;  // Long=0
        keypad.counterBoost = 0;

        // Set periods and reset timers.
        xTimerChangePeriod(keypad.timerDebounce, (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
        xTimerReset(keypad.timerDebounce, portMAX_DELAY);
        xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
        xTimerReset(keypad.timerLong, portMAX_DELAY);
      }

      //-- Remember what is just seen.
      keypad.keysDown |= keypad.keysPressed;

      //-- Longpress timeout check.
      //-- If this is the first Longpress event or REPEAT is enabled (KEY_LONGPRESS_REPEAT_TIME > 0).
      if (!xTimerIsTimerActive(keypad.timerLong) &&
          (((keypad.keysReleased & KEYLONG) == 0) || KEY_LONGPRESS_REPEAT_TIME)) {
        keypad.keysReleased = keypad.keysDown | KEYLONG;  // Release=Down, Long=1

        // If number of REPEAT events are greater than KEY_LONGPRESS_BOOST_THRESHOLD and BOOST is enabled.
        if (KEY_LONGPRESS_BOOST_THRESHOLD && (++keypad.counterBoost > KEY_LONGPRESS_BOOST_THRESHOLD)) {
          keypad.counterBoost = KEY_LONGPRESS_BOOST_THRESHOLD;
          xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_BOOST_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
          xTimerReset(keypad.timerLong, portMAX_DELAY);
        } else {
          xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_REPEAT_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
          xTimerReset(keypad.timerLong, portMAX_DELAY);
        }
      }
    } else {
      //   --Keypress timeout check.
      if (!xTimerIsTimerActive(keypad.timerDebounce)) {
        if ((keypad.keysReleased & KEYLONG) == 0) {
          keypad.keysReleased = keypad.keysDown;  // Release=Down
        }
      }

      //   -- Reset all.
      keypad.keysDown = 0;
      keypad.keysReleased &= ~KEYLONG;  // Long=0
      keypad.counterBoost = 0;

      //   -- Set periods and reset timers.
      xTimerChangePeriod(keypad.timerDebounce, (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
      xTimerReset(keypad.timerDebounce, portMAX_DELAY);
      xTimerChangePeriod(keypad.timerLong, (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), portMAX_DELAY);
      xTimerReset(keypad.timerLong, portMAX_DELAY);
    }

    // Broadcast event to notify any waiters.
    xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);
    xEventGroupSetBits(keypadEventGroup, (keypad.keysReleased & KEY_EVENTBITMASK));

    // Send to queue to notify any receivers.
    xQueueOverwrite(keypadQueue, &keypad.keysReleased);

    // Delay.
    // vTaskDelay(KEY_TASKDELAY_TIME / portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}
static CThread* initialize() {
  thread.run = run;

  //   Serial.printf_P(PSTR("Starting KeypadThread...\n"));

  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  pinMode(MUX_IN, INPUT);

  // Create timers.
  keypad.timerDebounce =
      xTimerCreate("Debounce", (KEY_DEBOUNCE_TIME / portTICK_PERIOD_MS), pdFALSE, (void*)0, keypadDummyTimerCallback);
  keypad.timerLong =
      xTimerCreate("LongPress", (KEY_LONGPRESS_TIME / portTICK_PERIOD_MS), pdFALSE, (void*)0, keypadDummyTimerCallback);

  // Create Event Group.
  keypadEventGroup = xEventGroupCreate();
  xEventGroupClearBits(keypadEventGroup, KEY_EVENTBITMASK);

  // Create Queue.
  keypadQueue = xQueueCreate(1, sizeof(uint32_t));

  return &thread;
}
static uint16_t keypadSerialize(void) {
  uint16_t result = 0;

  result |= (readKey(0) == HIGH) ? KEY00 : 0;
  result |= (readKey(1) == HIGH) ? KEY01 : 0;
  result |= (readKey(2) == HIGH) ? KEY02 : 0;
  result |= (readKey(3) == HIGH) ? KEY03 : 0;
  result |= (readKey(4) == HIGH) ? KEY04 : 0;
  result |= (readKey(5) == HIGH) ? KEY05 : 0;
  result |= (readKey(6) == HIGH) ? KEY06 : 0;
  result |= (readKey(7) == HIGH) ? KEY07 : 0;
  result |= (readKey(8) == HIGH) ? KEY08 : 0;
  result |= (readKey(9) == HIGH) ? KEY09 : 0;
  result |= (readKey(10) == HIGH) ? KEY10 : 0;
  result |= (readKey(11) == HIGH) ? KEY11 : 0;
  result |= (readKey(12) == HIGH) ? KEY12 : 0;
  result |= (readKey(13) == HIGH) ? KEY13 : 0;
  result |= (readKey(14) == HIGH) ? KEY14 : 0;

  return result;
}
static int readKey(int key) {
  digitalWrite(MUX_S0, bitRead(key, 0));
  digitalWrite(MUX_S1, bitRead(key, 1));
  digitalWrite(MUX_S2, bitRead(key, 2));
  digitalWrite(MUX_S3, bitRead(key, 3));
  vTaskDelay(pdMS_TO_TICKS(10));
  return digitalRead(MUX_IN);
}
const struct keypadThread KeypadThread = {
    .initialize = initialize,
};