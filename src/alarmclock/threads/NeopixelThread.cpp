#include "NeopixelThread.h"

using namespace std;

#define NEOPIXEL_QUEUE_SIZE 10
#define LED_PIN 32
#define LED_COUNT 12
#define BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

static CThread thread;

static void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
    strip.setPixelColor(i, color);               //  Set pixel's color (in RAM)
    strip.show();                                //  Update strip to match
    delay(wait);                                 //  Pause for a moment
  }
}

static void run(void* params) {
  for (;;) {
    colorWipe(strip.Color(255, 0, 0), 50);     // Red
    colorWipe(strip.Color(0, 255, 0), 50);     // Green
    colorWipe(strip.Color(0, 0, 255), 50);     // Blue
    colorWipe(strip.Color(0, 0, 0, 255), 50);  // True white (not RGB white)
    
  }
  vTaskDelete(NULL);
}

static CThread* initialize(u8 priority) {  
  strip.begin();            // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();             // Turn OFF all pixels ASAP
  strip.setBrightness(10);  // Set BRIGHTNESS to about 1/5 (max = 255)

  thread.run = run;
  return CThread_super(&thread, configMINIMAL_STACK_SIZE, "neopixelThread", (tskIDLE_PRIORITY+priority));
}
const struct neopixelThread NeopixelThread = {
    .initialize = initialize,
};