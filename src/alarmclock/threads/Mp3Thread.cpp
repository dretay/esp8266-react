#include "Mp3Thread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10

extern QueueHandle_t MP3_QUEUE;
DFPlayerMini_Fast myMP3;
extern HardwareSerial dfplayerUart;
static CThread thread;

static void run(void* params) {
  int command = -1;
  int S0 = 14;
  int S1 = 27;
  Serial.printf_P(PSTR("Mp3Thread running...\n"));

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
        Serial.printf("Mp3Thread Usage (bytes) = %d\n", uxTaskGetStackHighWaterMark(NULL));

        break;
      }
      case 1: {        
        myMP3.stop();
        Serial.printf("Mp3Thread Usage (bytes) = %d\n", uxTaskGetStackHighWaterMark(NULL));
        break;
      }
      default: {
      }
    }
    command = -1;
  }
  vTaskDelete(NULL);
}

static CThread* initialize() {
  Serial.printf_P(PSTR("Starting Mp3Thread...\n"));
  thread.run = run;

  
  dfplayerUart = HardwareSerial(1);
  dfplayerUart.begin(9600, SERIAL_8N1, 16, 17);
  myMP3.begin(dfplayerUart);
  myMP3.volume(30);
  return &thread;
}
const struct mp3Thread Mp3Thread = {
    .initialize = initialize,
};
