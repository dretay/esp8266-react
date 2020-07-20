#include "VoiceThread.h"

using namespace std;

#define VOICETHREAD_QUEUE_SIZE 10

extern QueueHandle_t VOICE_QUEUE;
AudioOutputI2S* out;
static CThread thread;

static void convertSmallTimeToWord(int input, char* timeOut);
static void convertLargeTimeToWord(int input, char* timeOut);
static void convertTimeToWord(char* timeIn, char* timeOut);

static void convertTimeToWord(char* timeIn, char* timeOut) {
  int input = atoi(timeIn);
  if (input < 20) {
    convertSmallTimeToWord(input, timeOut);
  } else {
    convertLargeTimeToWord(input, timeOut);
  }
}
static void convertLargeTimeToWord(int input, char* timeOut) {
  int bigWord = input / 10;
  int littleInt;
  char littleWord[10];
  switch (bigWord) {
    case 2: {
      strncpy(timeOut, "twenty.", 7);
      littleInt = input % 20;
      break;
    }
    case 3: {
      strncpy(timeOut, "thirty.", 7);
      littleInt = input % 30;
      break;
    }
    case 4: {
      strncpy(timeOut, "forty.", 7);
      littleInt = input % 40;
      break;
    }
    case 5: {
      strncpy(timeOut, "fifty.", 7);
      littleInt = input % 50;
      break;
    }
      convertSmallTimeToWord(littleInt, littleWord);
      strncat(timeOut, littleWord, 10);
  }
}
static void convertSmallTimeToWord(int input, char* timeOut) {
  switch (input) {
    case 1: {
      strncpy(timeOut, "one.", 10);
      break;
    }
    case 2: {
      strncpy(timeOut, "two.", 10);
      break;
    }
    case 3: {
      strncpy(timeOut, "three.", 10);
      break;
    }
    case 4: {
      strncpy(timeOut, "four.", 10);
      break;
    }
    case 5: {
      strncpy(timeOut, "five.", 10);
      break;
    }
    case 6: {
      strncpy(timeOut, "six.", 10);
      break;
    }
    case 7: {
      strncpy(timeOut, "seven.", 10);
      break;
    }
    case 8: {
      strncpy(timeOut, "eight.", 10);
      break;
    }
    case 9: {
      strncpy(timeOut, "nine.", 10);
      break;
    }
    case 10: {
      strncpy(timeOut, "ten.", 10);
      break;
    }
    case 11: {
      strncpy(timeOut, "eleven.", 10);
      break;
    }
    case 12: {
      strncpy(timeOut, "twelve.", 10);
      break;
    }
    case 13: {
      strncpy(timeOut, "thirteen.", 10);
      break;
    }
    case 14: {
      strncpy(timeOut, "fourteen.", 10);
      break;
    }
    case 15: {
      strncpy(timeOut, "fifteen.", 10);
      break;
    }
    case 16: {
      strncpy(timeOut, "sixteen.", 10);
      break;
    }
    case 17: {
      strncpy(timeOut, "seventeen.", 10);
      break;
    }
    case 18: {
      strncpy(timeOut, "eighteen.", 10);
      break;
    }
    case 19: {
      strncpy(timeOut, "nineteen.", 10);
      break;
    }
    default: {
      strncpy(timeOut, "", 10);
      break;
    }
  }
}
static void run(void* params) {
  int command = -1;
  Serial.printf_P(PSTR("VoiceThread running...\n"));
  char nowTsHours[3];
  char nowTsHoursLong[20];

  char nowTsMinutes[3];
  char nowTsMinutesLong[20];

  char nowTsAmPm[3];
  // char timeBuffer[8];
  for (;;) {
    xQueueReceive(VOICE_QUEUE, &command, portMAX_DELAY);
    Serial.printf_P(PSTR("VoiceThread speaking...\n"));

    int S0 = 14;
    int S1 = 27;
    switch (command) {
      case 0: {
        time_t currentTimestamp = time(nullptr);
        struct tm nowTs = *gmtime(&currentTimestamp);
        strftime(nowTsHours, 3, "%H", &nowTs);
        convertTimeToWord(nowTsHours, nowTsHoursLong);

        strftime(nowTsMinutes, 3, "%M", &nowTs);
        // Serial.printf_P(PSTR("nowTsMinutes: %s\n"), nowTsMinutes);
        convertTimeToWord(nowTsMinutes, nowTsMinutesLong);

        strftime(nowTsAmPm, 3, "%p", &nowTs);

        // Serial.printf_P(PSTR("Time: %s %s %s\n"), nowTsHoursLong, nowTsMinutesLong, nowTsAmPm);

        out = new AudioOutputI2S(0, 1, 8, 0);
        out->begin();
        ESP8266SAM* sam = new ESP8266SAM;
        digitalWrite(S0, HIGH);
        digitalWrite(S1, LOW);
        sam->Say(out, nowTsHoursLong);
        delay(100);
        sam->Say(out, nowTsMinutesLong);
        delay(100);
        if (strncmp(nowTsAmPm, "am", 2) == 0) {
          sam->Say(out, "a.");
        } else {
          sam->Say(out, "p.");
        }

        delay(100);
        sam->Say(out, "m.");
        // sam->Say(out, nowTsAmPm);
        delete sam;
        out->stop();
        Serial.printf("voiceThread Usage (bytes) = %d\n", uxTaskGetStackHighWaterMark(NULL));
        break;
      }
      default: {
        // whiteOverRainbow(75, 5);
      }
    }
    command = -1;
  }
  vTaskDelete(NULL);
}

static CThread* initialize() {
  thread.run = run;
  return &thread;
}
const struct voiceThread VoiceThread = {
    .initialize = initialize,
};