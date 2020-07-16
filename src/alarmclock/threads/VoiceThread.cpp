#include "VoiceThread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10

QueueHandle_t VOICE_QUEUE;
AudioOutputI2S* out;

void VoiceThread::run() {
  int command = -1;

  for (;;) {
    xQueueReceive(VOICE_QUEUE, &command, portMAX_DELAY);
    int S0 = 14;
    int S1 = 27;
    switch (command) {
      case 0: {
        digitalWrite(S0, HIGH);
        digitalWrite(S1, LOW);
        out = new AudioOutputI2S(0, 1, 8, 0);
        out->begin();
        ESP8266SAM* sam = new ESP8266SAM;
        sam->Say(out, "time.");
        delay(150);
        sam->Say(out, "to.");
        delay(150);
        sam->Say(out, "wake.");
        delay(100);
        sam->Say(out, "up.");
        delay(100);
        sam->Say(out, "Alex.");
        delete sam;
        out->stop();
        // Serial.printf("voiceThread Usage (bytes) = %d\n",uxTaskGetStackHighWaterMark(NULL));
        break;
      }
      default: {
        // whiteOverRainbow(75, 5);
      }
    }
    command = -1;
  }
}

VoiceThread::VoiceThread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name) :
    // this class arguments
    Thread{_stackDepth, _priority, _name} {
  VOICE_QUEUE = xQueueCreate(NEOPIXEL_QUEUE_SIZE, sizeof(int));
  if (VOICE_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
}
