#include "AnalogMux.h"

static int S0=14;
static int S1=27;

static bool setChannel(int channel) {
  if (channel > 3 || channel < 0) {
    return false;
  }
  switch (channel) {
    case 0: {
      digitalWrite(S0, LOW);
      digitalWrite(S1, LOW);
      break;
    }
    case 1: {
      digitalWrite(S0, HIGH);
      digitalWrite(S1, LOW);
      break;
    }
    case 2: {
      digitalWrite(S0, LOW);
      digitalWrite(S1, HIGH);
      break;
    }
    case 3: {
      digitalWrite(S0, HIGH);
      digitalWrite(S1, HIGH);
      break;
    }
  }
  return true;
}
static void initialize(void) {
  pinMode(S0, OUTPUT);  // s0
  pinMode(S1, OUTPUT);  // s1
}
const struct analogMux AnalogMux = {
    .initialize = initialize,
    .setChannel = setChannel,
};
