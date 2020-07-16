#include "Mp3Thread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10

QueueHandle_t MP3_QUEUE;
DFPlayerMini_Fast myMP3;
extern HardwareSerial dfplayerUart;

void Mp3Thread::run() {
  int command = -1;
  int S0 = 14;
  int S1 = 27;
  for (;;) {
    xQueueReceive(MP3_QUEUE, &command, portMAX_DELAY);
    int S0 = 14;
    int S1 = 27;
    switch (command) {
      case 0: {
        digitalWrite(S0, LOW);
        digitalWrite(S1, LOW);
        // Serial.printf_P(PSTR("Playing %d\n"), _state.entries[0].song);
        // myMP3.playFromMP3Folder(_state.entries[0].song);
        myMP3.playFromMP3Folder(1);
        myMP3.startRepeat();
        Serial.printf("Mp3Thread Usage (bytes) = %d\n",uxTaskGetStackHighWaterMark(NULL));

        break;
      }
      default: {
      }
    }
    command = -1;
  }
}

Mp3Thread::Mp3Thread(const uint32_t _stackDepth, UBaseType_t _priority, const char* const _name) :
    // this class arguments
    Thread{_stackDepth, _priority, _name} {
  MP3_QUEUE = xQueueCreate(NEOPIXEL_QUEUE_SIZE, sizeof(int));
  if (MP3_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
  dfplayerUart = HardwareSerial(1);
  dfplayerUart.begin(9600, SERIAL_8N1, 16, 17);
  myMP3.begin(dfplayerUart);
  myMP3.volume(30);
}
