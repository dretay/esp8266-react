#include "VoiceThread.h"

using namespace std;

QueueHandle_t VOICE_QUEUE;
AudioOutputI2S* out;
static CThread thread;

static void convertSmallTimeToWord(int input, char* timeOut, int wordSize, bool isHour);
static void convertLargeTimeToWord(int input, char* timeOut, int wordSize);
static void convertTimeToWord(char* timeIn, char* timeOut);

static void convertTimeToWord(char* timeIn, char* timeOut, bool isHour) {
  int input = atoi(timeIn);
  static int wordSize = 15;
  Serial.printf_P(PSTR("convertTimeToWord: %s -> %d\n"), timeIn, input);
  if (input < 20) {
    convertSmallTimeToWord(input, timeOut, wordSize, isHour);
  } else {
    convertLargeTimeToWord(input, timeOut, wordSize);
  }
}
static void convertLargeTimeToWord(int input, char* timeOut, int wordSize) {
  int bigWord = input / 10;
  int littleInt;
  char littleWord[wordSize];
  Serial.printf_P(PSTR("convertLargeTimeToWord: %d\n"), input);
  switch (bigWord) {
    case 2: {
      strncpy(timeOut, "twenty.", wordSize);
      littleInt = input % 20;
      break;
    }
    case 3: {
      strncpy(timeOut, "thirty.", wordSize);
      littleInt = input % 30;
      break;
    }
    case 4: {
      strncpy(timeOut, "forty.", wordSize);
      littleInt = input % 40;
      break;
    }
    case 5: {
      strncpy(timeOut, "fifty.", wordSize);
      littleInt = input % 50;
      break;
    }
  }
  if (littleInt > 0 || bigWord == 0) {
    bool useOh = bigWord >= 2 ? true : false;
    convertSmallTimeToWord(littleInt, littleWord, wordSize, useOh);
    strncat(timeOut, littleWord, wordSize);
  }
}
static void convertSmallTimeToWord(int input, char* timeOut, int wordSize, bool isHour) {
  Serial.printf_P(PSTR("convertSmallTimeToWord: %d\n"), input);

  switch (input) {
    case 1: {
      if (!isHour) {
        strncpy(timeOut, "oh.one.", wordSize);

      } else {
        strncpy(timeOut, "one.", wordSize);
      }
      break;
    }
    case 2: {
      if (!isHour) {
        strncpy(timeOut, "oh.two.", wordSize);

      } else {
        strncpy(timeOut, "two.", wordSize);
      }
      break;
    }
    case 3: {
      if (!isHour) {
        strncpy(timeOut, "oh.three.", wordSize);

      } else {
        strncpy(timeOut, "three.", wordSize);
      }
      break;
    }
    case 4: {
      if (!isHour) {
        strncpy(timeOut, "oh.four.", wordSize);

      } else {
        strncpy(timeOut, "four.", wordSize);
      }
      break;
    }
    case 5: {
      if (!isHour) {
        strncpy(timeOut, "oh.five.", wordSize);

      } else {
        strncpy(timeOut, "five.", wordSize);
      }
      break;
    }
    case 6: {
      if (!isHour) {
        strncpy(timeOut, "oh.siz.", wordSize);

      } else {
        strncpy(timeOut, "six.", wordSize);
      }
      break;
    }
    case 7: {
      if (!isHour) {
        strncpy(timeOut, "oh.seven.", wordSize);

      } else {
        strncpy(timeOut, "seven.", wordSize);
      }
      break;
    }
    case 8: {
      if (!isHour) {
        strncpy(timeOut, "oh.eight.", wordSize);

      } else {
        strncpy(timeOut, "eight.", wordSize);
      }
      break;
    }
    case 9: {
      if (!isHour) {
        strncpy(timeOut, "oh.nine.", wordSize);

      } else {
        strncpy(timeOut, "nine.", wordSize);
      }
      break;
    }
    case 10: {
      strncpy(timeOut, "ten.", wordSize);
      break;
    }
    case 11: {
      strncpy(timeOut, "eleven.", wordSize);
      break;
    }
    case 12: {
      strncpy(timeOut, "twelve.", wordSize);
      break;
    }
    case 13: {
      strncpy(timeOut, "thirteen.", wordSize);
      break;
    }
    case 14: {
      strncpy(timeOut, "four teen.", wordSize);
      break;
    }
    case 15: {
      strncpy(timeOut, "fifteen.", wordSize);
      break;
    }
    case 16: {
      strncpy(timeOut, "six teen.", wordSize);
      break;
    }
    case 17: {
      strncpy(timeOut, "seven teen.", wordSize);
      break;
    }
    case 18: {
      strncpy(timeOut, "eight teen.", wordSize);
      break;
    }
    case 19: {
      strncpy(timeOut, "nine teen.", wordSize);
      break;
    }
    default: {
      strncpy(timeOut, "oh clock.", wordSize);
      break;
    }
  }
}
static void run(void* params) {
  int command = -1;
  Serial.printf_P(PSTR("VoiceThread running...\n"));
  char nowTsHours[3];
  char nowTsHoursLong[30];

  char nowTsMinutes[3];
  char nowTsMinutesLong[30];

  char nowTsAmPm[3];
  // char timeBuffer[8];
  for (;;) {
    xQueueReceive(VOICE_QUEUE, &command, portMAX_DELAY);
    switch (command) {
      case 0: {
        time_t currentTimestamp = time(nullptr);
        struct tm nowTs = *localtime(&currentTimestamp);
        strftime(nowTsHours, 3, "%I", &nowTs);
        convertTimeToWord(nowTsHours, nowTsHoursLong, true);

        strftime(nowTsMinutes, 3, "%M", &nowTs);
        // Serial.printf_P(PSTR("nowTsMinutes: %s\n"), nowTsMinutes);
        convertTimeToWord(nowTsMinutes, nowTsMinutesLong, false);

        strftime(nowTsAmPm, 3, "%p", &nowTs);

        // Serial.printf_P(PSTR("Time: %s %s %s\n"), nowTsHoursLong, nowTsMinutesLong, nowTsAmPm);

        out = new AudioOutputI2S(0, 1, 8, 0);
        out->begin();
        ESP8266SAM* sam = new ESP8266SAM;
        AnalogMux.setChannel(1);
        // 0 = mp3, 2=gnd, 1=speech
        sam->Say(out, nowTsHoursLong);
        delay(100);
        sam->Say(out, nowTsMinutesLong);
        delay(100);
        if (strncmp(nowTsAmPm, "AM", 2) == 0) {
          sam->Say(out, "a.");
        } else {
          sam->Say(out, "p.");
        }

        delay(100);
        sam->Say(out, "m.");
        // sam->Say(out, nowTsAmPm);
        delete sam;
        out->stop();
        AnalogMux.setChannel(2);
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

static CThread* initialize(u8 priority) {  
  // start threads
  VOICE_QUEUE = xQueueCreate(1, sizeof(int));
  if (VOICE_QUEUE == NULL) {
    Serial.printf_P(PSTR("Error creating the queue"));
  }
  AnalogMux.initialize();

  thread.run = run;
  return CThread_super(&thread, 2048, "voiceThread", (tskIDLE_PRIORITY+priority));
}
const struct voiceThread VoiceThread = {
    .initialize = initialize,
};