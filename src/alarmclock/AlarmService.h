#ifndef AlarmService_h
#define AlarmService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>
#include <ESPUtils.h>

#include <time.h>
#include <HardwareSerial.h>

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif


#include "threads/CThread.h"
#include "threads/ButtonThread.h"
#include "threads/Mp3Thread.h"
#include "threads/NeopixelThread.h"
#include "threads/VoiceThread.h"
#include "threads/ClockThread.h"


#include "AnalogMux.h"


#define ALARM_SETTINGS_FILE "/config/alarmSettings.json"
#define ALARM_SETTINGS_PATH "/rest/alarmState"

class AlarmSettings {
 public:
  class AlarmSettingEntry {
   public:
    int song;
    time_t time;
    String daysOfWeek;
    bool enabled;
  };
  AlarmSettingEntry entries[5];

  // take the data out of settings and put it into root
  static void read(AlarmSettings& settings, JsonObject& root) {
    // Serial.println(F("In Read Handler!"));
    JsonArray entries = root.createNestedArray("entries");
    JsonObject entry = entries.createNestedObject();
    entry["song"] = settings.entries[0].song;
    entry["time"] = settings.entries[0].time;
    entry["daysOfWeek"] = settings.entries[0].daysOfWeek;
    entry["enabled"] = settings.entries[0].enabled;

   
  }

  // take the data out of root and put it into settings
  static StateUpdateResult update(JsonObject& root, AlarmSettings& settings) {
    // Serial.println(F("In Update Handler!"));
    // serializeJsonPretty(root,Serial);
    settings.entries[0].song = root["entries"][0]["song"];
    settings.entries[0].time = root["entries"][0]["time"];
    settings.entries[0].daysOfWeek = root["entries"][0]["daysOfWeek"].as<String>();
    settings.entries[0].enabled = root["entries"][0]["enabled"];

    return StateUpdateResult::CHANGED;
  }
};

class AlarmService : public StatefulService<AlarmSettings> {
 public:
  AlarmService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();
  void loop();

 private:
  HttpEndpoint<AlarmSettings> _httpEndpoint;
  FSPersistence<AlarmSettings> _fsPersistence;
  bool alarmTriggered;
  void colorWipe(uint32_t color, int wait);
};

#endif  // end AlarmService_h
