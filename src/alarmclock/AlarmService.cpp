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
void AlarmService::handleInterrupt() {
  portENTER_CRITICAL_ISR(&mux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&mux);
}
#define S0 14
#define S1 27

extern QueueHandle_t VOICE_QUEUE;
extern QueueHandle_t MP3_QUEUE;
void AlarmService::begin() {
  Serial.printf_P(PSTR("Begin()\n"));
  _fsPersistence.readFromFS();

  mux = portMUX_INITIALIZER_UNLOCKED;

  pinMode(S0, OUTPUT);  // s0
  pinMode(S1, OUTPUT);  // s1

  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  int command = 0;

  //voiceThread Usage (bytes) = 8308
  VoiceThread voiceThread{10000, 1, "voiceThread"};

  Mp3Thread Mp3Thread{10000, 1, "mp3Thread"};
  // xQueueSend(MP3_QUEUE, &command, portMAX_DELAY);

  KeypadThread KeypadThread{10000, 1, "keypadThread"};

  // // strip = 
  // // Serial.printf_P(PSTR("largest_free_block: %d\n"),heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  
  // NeopixelThread NeopixelThread{1024, 5, "neopixelThread"};
  
}



void AlarmService::loop() {    

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