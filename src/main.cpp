#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
#include <alarmclock/AlarmService.h>
#include <FS.h>

#include "alarmclock/threads/VoiceThread.h"
#include "alarmclock/threads/Mp3Thread.h"
#include "alarmclock/threads/KeypadThread.h"
#include "alarmclock/threads/NeopixelThread.h"

#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server);
LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, esp8266React.getFS(), esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);
AlarmService alarmService = AlarmService(&server, &SPIFFS, esp8266React.getSecurityManager());
HardwareSerial dfplayerUart(1);
QueueHandle_t VOICE_QUEUE,MP3_QUEUE;

float GetTaskHighWaterMarkPercent(TaskHandle_t task_handle, uint32_t stack_allotment) {
  UBaseType_t uxHighWaterMark;
  uint32_t diff;
  float result;

  uxHighWaterMark = uxTaskGetStackHighWaterMark(task_handle);

  diff = stack_allotment - uxHighWaterMark;

  result = ((float)diff / (float)stack_allotment) * 100.0;

  return result;
}
// KeypadThread keypadThread;
// static uint keypadStackSize = 50000;

// VoiceThread voiceThread;
// static uint voiceStackSize = 1000;

// Mp3Thread mp3Thread;
// static uint mp3StackSize = 10000;

// NeopixelThread neopixelThread;
// static uint neopixelStackSize = 1024;
static CThread *voiceThread, *mp3Thread, *neopixelThread;
void setup() {
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif
  // start threads
  VOICE_QUEUE = xQueueCreate(10, sizeof(int));
  if (VOICE_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
  MP3_QUEUE = xQueueCreate(10, sizeof(int));
  if (MP3_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
  voiceThread = VoiceThread.initialize();
  mp3Thread = Mp3Thread.initialize();
  neopixelThread = NeopixelThread.initialize();

  // start the framework and demo project
  esp8266React.begin();

  // load the initial light settings
  lightStateService.begin();

  // load the initial alarm settings
  alarmService.begin();

  // start the light service
  lightMqttSettingsService.begin();


  // start the server
  server.begin();
}

void loop() {
  // VoiceThread voiceThread{50000, 1, "voiceThread"};
  // Mp3Thread mp3Thread{10000, 1, "mp3Thread"};
  // NeopixelThread neopixelThread{1024, 1, "neopixelThread"};
  
  // voiceThread Usage (bytes) = 3288
  xTaskCreate(voiceThread->run, "voiceThread", 5000, NULL, (tskIDLE_PRIORITY + 3), NULL);
  
  // Mp3Thread Usage (bytes) = 3620
  xTaskCreate(mp3Thread->run, "mp3Thread", 5000, NULL, (tskIDLE_PRIORITY), NULL);
  
  // neopixelThread Usage (bytes) = 3628
  xTaskCreate(neopixelThread->run, "neopixelThread", 5000, NULL, (tskIDLE_PRIORITY), NULL);

  for (;;) {
    esp8266React.loop();
    alarmService.loop();
  }
  // run the framework's loop function

  // static const unsigned long REFRESH_INTERVAL = 1000;  // ms
  // static unsigned long lastRefreshTime = 0;

  // if (millis() - lastRefreshTime >= REFRESH_INTERVAL) {
  //   lastRefreshTime += REFRESH_INTERVAL;
  //   Serial.printf_P(PSTR("VoiceThread max usage: %.0f%%\n"),
  //                   GetTaskHighWaterMarkPercent(voiceThread.getHandle(), voiceStackSize));
  //   Serial.printf_P(PSTR("Mp3Thread max usage: %.0f%%\n"),
  //                   GetTaskHighWaterMarkPercent(mp3Thread.getHandle(), mp3StackSize));
  //   Serial.printf_P(PSTR("KeypadThread max usage: %.0f%%\n"),
  //                   GetTaskHighWaterMarkPercent(keypadThread.getHandle(), keypadStackSize));
  //   Serial.printf_P(PSTR("NeopixelThread max usage: %.0f%%\n"),
  //                   GetTaskHighWaterMarkPercent(neopixelThread.getHandle(), neopixelStackSize));
  //   Serial.printf_P(PSTR("largest_free_block: %d\n"), heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  // }
}
