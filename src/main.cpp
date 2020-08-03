#include <ESP8266React.h>
#include <LightMqttSettingsService.h>
#include <LightStateService.h>
#include <alarmclock/AlarmService.h>
#include <FS.h>


#define SERIAL_BAUD_RATE 115200

AsyncWebServer server(80);
ESP8266React esp8266React(&server, &SPIFFS);
LightMqttSettingsService lightMqttSettingsService =
    LightMqttSettingsService(&server, &SPIFFS, esp8266React.getSecurityManager());
LightStateService lightStateService = LightStateService(&server,
                                                        esp8266React.getSecurityManager(),
                                                        esp8266React.getMqttClient(),
                                                        &lightMqttSettingsService);
AlarmService alarmService = AlarmService(&server, &SPIFFS, esp8266React.getSecurityManager());
HardwareSerial dfplayerUart(1);

float GetTaskHighWaterMarkPercent(TaskHandle_t task_handle, uint32_t stack_allotment) {
  UBaseType_t uxHighWaterMark;
  uint32_t diff;
  float result;

  uxHighWaterMark = uxTaskGetStackHighWaterMark(task_handle);

  diff = stack_allotment - uxHighWaterMark;

  result = ((float)diff / (float)stack_allotment) * 100.0;

  return result;
}

void setup() {

  
  // start serial and filesystem
  Serial.begin(SERIAL_BAUD_RATE);

  // start the file system (must be done before starting the framework)
#ifdef ESP32
  SPIFFS.begin(true);
#elif defined(ESP8266)
  SPIFFS.begin();
#endif
 
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
