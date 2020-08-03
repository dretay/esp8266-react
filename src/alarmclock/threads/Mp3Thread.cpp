#include "Mp3Thread.h"

using namespace std;

QueueHandle_t MP3_QUEUE;
DFPlayerMini_Fast myMP3;
extern HardwareSerial dfplayerUart;
static CThread thread;

static void run(void* params) {
  int command = -1;
  Serial.printf_P(PSTR("Mp3Thread running...\n"));

  for (;;) {
    xQueueReceive(MP3_QUEUE, &command, portMAX_DELAY);
    int S0 = 14;
    int S1 = 27;
    switch (command) {
      case 0: {
        AnalogMux.setChannel(0);
        // 0 = mp3, 2=gnd, 1=speech
        // Serial.printf_P(PSTR("Playing %d\n"), _state.entries[0].song);
        // myMP3.playFromMP3Folder(_state.entries[0].song);
        myMP3.playFromMP3Folder(1);
        myMP3.startRepeat();
        break;
      }
      case 1: {
        myMP3.stop();
        AnalogMux.setChannel(2);
        break;
      }
      default: {
      }
    }
    command = -1;
  }
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {
  dfplayerUart = HardwareSerial(1);
  dfplayerUart.begin(9600, SERIAL_8N1, 16, 17);
  myMP3.begin(dfplayerUart);
  myMP3.volume(30);

  MP3_QUEUE = xQueueCreate(1, sizeof(int));
  if (MP3_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }

  AnalogMux.initialize();
  thread.run = run;
  return CThread_super(&thread, 2048, "mp3Thread", (tskIDLE_PRIORITY+priority));
}
const struct mp3Thread Mp3Thread = {
    .initialize = initialize,
};
