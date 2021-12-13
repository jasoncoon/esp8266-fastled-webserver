#include "common.h"

// Pride2015 by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.

// Modified by Jason Coon to replace "magic numbers" with customizable inputs via sliders in the web app.

uint8_t saturationBpm = 87;
uint8_t saturationMin = 220;
uint8_t saturationMax = 250;

uint8_t brightDepthBpm = 1;
uint8_t brightDepthMin = 96;
uint8_t brightDepthMax = 224;

uint8_t brightThetaIncBpm = 203;
uint8_t brightThetaIncMin = 25;
uint8_t brightThetaIncMax = 40;

uint8_t msMultiplierBpm = 147;
uint8_t msMultiplierMin = 23;
uint8_t msMultiplierMax = 60;

uint8_t hueIncBpm = 113;
uint8_t hueIncMin = 1;
uint8_t hueIncMax = 12;

uint8_t sHueBpm = 2;
uint8_t sHueMin = 5;
uint8_t sHueMax = 9;

void fillWithPridePlayground(bool useFibonacciOrder)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(saturationBpm, saturationMin, saturationMax);

  uint8_t brightdepth = beatsin88(brightDepthBpm * 256, brightDepthMin, brightDepthMax);
  uint16_t brightnessthetainc16 = beatsin88(brightThetaIncBpm, (brightThetaIncMin * 256), (brightThetaIncMax * 256));
  
  uint8_t msmultiplier = beatsin88(msMultiplierBpm, msMultiplierMin, msMultiplierMax);

  uint16_t hue16 = sHue16; //gHue * 256;
  uint16_t hueinc16 = beatsin88(hueIncBpm, hueIncMin, hueIncMax * 256);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(sHueBpm * 256, sHueMin, sHueMax);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_PIXELS; i++)
  {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
#if IS_FIBONACCI
    if (useFibonacciOrder) pixelnumber = fibonacciToPhysical[i];
#else
    (void)useFibonacciOrder;
#endif

    pixelnumber = (NUM_PIXELS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void pridePlayground() {
  fillWithPridePlayground(false);
}

#if IS_FIBONACCI
void pridePlaygroundFibonacci() {
  fillWithPridePlayground(true);
}
#endif
