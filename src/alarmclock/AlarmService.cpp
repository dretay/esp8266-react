#include "AlarmService.h"

AlarmService::AlarmService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager) :
    _httpEndpoint(AlarmSettings::read,
                  AlarmSettings::update,
                  this,
                  server,
                  ALARM_SETTINGS_PATH,
                  securityManager,
                  AuthenticationPredicates::IS_AUTHENTICATED),
    _fsPersistence(AlarmSettings::read, AlarmSettings::update, this, fs, ALARM_SETTINGS_FILE) {
  alarmTriggered = false;
}

#define S0 14
#define S1 27

extern QueueHandle_t VOICE_QUEUE;
extern QueueHandle_t MP3_QUEUE;

static CThread* keypadThread;

static void toggleVoiceThread(void* param) {
  uint32_t keypadValue;
  while (true) {
    if (xQueueReceive(keypadQueue, &keypadValue, portMAX_DELAY) && (keypadValue == KEY00)) {
      Serial.println("Sending voice command...");

      int command = 0;
      xQueueSend(VOICE_QUEUE, &command, portMAX_DELAY);
    }
  }
}
static int mp3_command = 0;
static void toggleMp3Thread(void* param) {
  uint32_t keypadValue;
  while (true) {
    if (xQueueReceive(keypadQueue, &keypadValue, portMAX_DELAY) && (keypadValue == KEY01)) {
      Serial.println("Sending mp3 command...");
      xQueueSend(MP3_QUEUE, &mp3_command, portMAX_DELAY);
      mp3_command++;
    }
  }
}

void AlarmService::begin() {
  Serial.printf_P(PSTR("Begin()\n"));
  _fsPersistence.readFromFS();

  pinMode(S0, OUTPUT);  // s0
  pinMode(S1, OUTPUT);  // s1

  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  keypadThread = KeypadThread.initialize();

  xTaskCreate(toggleVoiceThread, "toggleVoiceThread", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL);
  xTaskCreate(toggleMp3Thread, "toggleMp3Thread", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1), NULL);
  xTaskCreate(keypadThread->run, "keypadThread", configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 3), NULL);

  // VoiceThread voiceThread{50000, 1, "voiceThread"};
  // Mp3Thread mp3Thread{10000, 1, "mp3Thread"};
  // NeopixelThread neopixelThread{1024, 1, "neopixelThread"};
}
// typedef enum {
//   TOGGLE_LED_Blue_KEY = (KEY00),
//   TOGGLE_LED_Green_KEY = (KEY00 | KEYLONG),
// } tToggleKeys;
void AlarmService::loop() {
  // delay(1000);
  // xEventGroupWaitBits(keypadEventGroup, TOGGLE_LED_Green_KEY, pdTRUE, pdTRUE, 0);
  // Serial.printf_P(PSTR("Key 0 Pressed"));

  // char storedTsHours[3];
  // char storedTsMinutes[3];
  // time_t storedTimestamp = _state.entries[0].time;
  // struct tm storedTs = *gmtime(&storedTimestamp);
  // strftime(storedTsHours, 3, "%H", &storedTs);
  // strftime(storedTsMinutes, 3, "%M", &storedTs);
  // int storedHours = atoi(storedTsHours);
  // int storedMinutes = atoi(storedTsMinutes);

  // char nowTsHours[3];
  // char nowTsMinutes[3];
  // time_t currentTimestamp = time(nullptr);
  // struct tm nowTs = *gmtime(&currentTimestamp);
  // strftime(nowTsHours, 3, "%H", &nowTs);
  // strftime(nowTsMinutes, 3, "%M", &nowTs);
  // int nowHours = atoi(nowTsHours);
  // int nowMinutes = atoi(nowTsMinutes);

  // if (storedHours == nowHours && storedMinutes == nowMinutes) {
  //   if (alarmTriggered == false) {
  //     alarmTriggered = true;
  //     int speak = 0;
  //     int play = 1;
  //     // xQueueSend(queue, &speak, portMAX_DELAY);
  //     // xQueueSend(queue, &play, portMAX_DELAY);
  //   } else {
  //     // this is 2
  //   }
  // } else {
  //   // digitalWrite(S0, LOW);
  //   // digitalWrite(S1, HIGH);
  //   alarmTriggered = false;
  // }
  // whiteOverRainbow(75, 5);
  // // neopixels->whiteOverRainbow(75, 5);

  // if (interruptCounter > 0) {
  //   portENTER_CRITICAL(&mux);
  //   interruptCounter--;
  //   portEXIT_CRITICAL(&mux);

  //   numberOfInterrupts++;
  //   if (alarmTriggered) {
  //     // myMP3.stop();
  //   }
  // }
}