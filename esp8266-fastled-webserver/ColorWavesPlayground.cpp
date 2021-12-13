#include "common.h"

// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.

// Modified by Jason Coon to replace "magic numbers" with customizable inputs via sliders in the web app.

void colorwavesPlayground( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette, bool useFibonacciOrder)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  // uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88(brightDepthBpm * 256, brightDepthMin, brightDepthMax);
  uint16_t brightnessthetainc16 = beatsin88(brightThetaIncBpm, (brightThetaIncMin * 256), (brightThetaIncMax * 256));

  uint8_t msmultiplier = beatsin88(msMultiplierBpm, msMultiplierMin, msMultiplierMax);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(hueIncBpm, hueIncMin, hueIncMax * 256);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(sHueBpm * 256, sHueMin, sHueMax);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;

#if IS_FIBONACCI
    if (useFibonacciOrder) pixelnumber = fibonacciToPhysical[i];
#else
    (void)useFibonacciOrder;
#endif

    pixelnumber = (numleds - 1) - pixelnumber;

    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}

void colorWavesPlayground()
{
  colorwavesPlayground(leds, NUM_PIXELS, gCurrentPalette, false);
}


#if IS_FIBONACCI
void colorWavesPlaygroundFibonacci()
{
  colorwavesPlayground(leds, NUM_PIXELS, gCurrentPalette, true);
}
#endif

