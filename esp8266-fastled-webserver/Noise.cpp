/*
   ESP8266 + FastLED: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2015-2020 Jason Coon

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

#if 0 && defined(PRODUCT_1628_RINGS)
  // NOTE: these are entirely unused, and thus commented out entirely.
  //       however, they are retained in case they would be useful in future.

  // Polar coordinate mapping code for FastLED by Adam Haile, Maniacal Labs:
  // http://maniacallabs.com/2015/05/04/review-code-adafruit-dotstar-disk/
  // https://gist.github.com/adammhaile/a769f3ff87ff61f22ace

  const uint8_t ringCount { 20 };           // Total Number of Rings. AdaFruit Disk has 10
  const uint8_t lastRing { ringCount - 1 }; // for convenience

  //Map rings on disk to indicies.
  //Each represents one of the concentric rings.
  //TODO: change to structure with min/max (inclusive), to clarify meaning of indices
  const uint16_t rings[ringCount][2] {
    // { first pixel of the ring, last pixel of the ring } // INCLUSIVE indices
    {    0,    3 },
    {    4,   15 },
    {   16,   36 },
    {   37,   65 },
    {   66,  102 },
    {  103,  147 },
    {  148,  200 },
    {  201,  262 },
    {  263,  332 },
    {  333,  410 },
    {  411,  496 },
    {  497,  590 },
    {  591,  692 },
    {  693,  802 },
    {  803,  920 },
    {  921, 1045 },
    { 1046, 1180 },
    { 1181, 1322 },
    { 1323, 1467 },
    { 1468, 1627 },
  };
#endif

// drawNoise() function uses coordsX / coordsY
#if HAS_COORDINATE_MAP

// TODO: static assert that the values here correspond to the maximum storable value
//       used by the typeof(coordsX) and typeof(coordsY).
const uint8_t matrixWidth  = 255;
const uint8_t matrixHeight = 255;

#define MAX_DIMENSION ((matrixWidth > matrixHeight) ? matrixWidth : matrixHeight)

// The larger (16-bit) version of our coordinates
static uint16_t noisex;
static uint16_t noisey;
static uint16_t noisez;

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
static int noisespeedx = 0;
static int noisespeedy = 1;
static int noisespeedz = 0;

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise will be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
static uint16_t noisescale = 1; // scale is set dynamically once we've started up

static uint8_t colorLoop = 0;




static const CRGBPalette16 blackAndWhiteStripedPalette {
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black
};

static const CRGBPalette16 blackAndBlueStripedPalette {
  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,  CRGB::Blue,
  CRGB::Blue,  CRGB::Blue,  CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black,
  CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
};

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.
CRGB noiseXYZ(CRGBPalette16 palette, uint8_t hueReduce, int x, int y, int z)
{
  uint8_t data = inoise8(x, y, z);
  // The range of the inoise8 function is roughly 16-238.
  // These two operations expand those values out to roughly 0..255
  // You can comment them out if you want the raw noise data.
  data = qsub8(data, 16);
  data = qadd8(data, scale8(data, 39));

  if (hueReduce > 0 && data >= hueReduce) {
    data -= hueReduce;
  }

  return ColorFromPalette(palette, data, 255, LINEARBLEND);
}

void drawNoise(CRGBPalette16 palette, uint8_t hueReduce = 0)
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t x = coordsX[i];
    uint8_t y = coordsY[i];

    int xoffset = noisescale * x;
    int yoffset = noisescale * y;

    leds[i] = noiseXYZ(palette, hueReduce, x + xoffset + noisex, y + yoffset + noisey, noisez);
  }

  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}

#if HAS_POLAR_COORDS // change to "HAS_CONCENTRIC_RINGS" ?

// drawPolarNoise() uses angles[] and radii[]
void drawPolarNoise(CRGBPalette16 palette, uint8_t hueReduce = 0)
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t x = angles[i];
    uint8_t y = radii[i] / 2u; // divide by 2 to change range of values from [0..255] to [0..127]

    int xoffset = noisescale * x;
    int yoffset = noisescale * y;
    leds[i] = noiseXYZ(palette, hueReduce, x + xoffset + noisex, y + yoffset + noisey, noisez);
  }
  noisex += noisespeedx;
  noisey += noisespeedy;
  noisez += noisespeedz;
}
#endif // HAS_POLAR_COORDS

#if HAS_POLAR_COORDS //  gradientPalettePolarNoise() uses drawPolarNoise()
// TODO: Check if gradientPalettePolarNoise() is equivalent to angleGradientPalette()?
void gradientPalettePolarNoise() {
  noisespeedx = 4;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(gCurrentPalette);
}
#endif
#if HAS_POLAR_COORDS // palettePolarNoise() uses drawPolarNoise()
void palettePolarNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(palettes[currentPaletteIndex]);
}
#endif

void rainbowNoise() {
  noisespeedx = 0;
  noisespeedy = -1;
  noisespeedz = 0;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(RainbowColors_p);
}

#if HAS_POLAR_COORDS // rainbowPolarNoise() uses drawPolarNoise()
void rainbowPolarNoise() {
  noisespeedx = 0;
  noisespeedy = 2;
  noisespeedz = 0;
  noisescale = 0;
  colorLoop = 0;
  drawPolarNoise(RainbowColors_p);
}
#endif // HAS_POLAR_COORDS

void rainbowStripeNoise() {
  noisespeedx = 0;
  noisespeedy = -2;
  noisespeedz = 0;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(RainbowStripeColors_p);
}

#if HAS_POLAR_COORDS // rainbowStripePolarNoise() uses drawPolarNoise()
void rainbowStripePolarNoise() {
  noisespeedx = 0;
  noisespeedy = 2;
  noisespeedz = 0;
  noisescale = 0;
  colorLoop = 0;
  drawPolarNoise(RainbowStripeColors_p);
}
#endif

void partyNoise() {
  noisespeedx = -9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 8;
  colorLoop = 0;
  drawNoise(PartyColors_p);
}

#if HAS_POLAR_COORDS // partyPolarNoise() uses drawPolarNoise()
void partyPolarNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(PartyColors_p);
}
#endif

void forestNoise() {
  noisespeedx = -9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 8;
  colorLoop = 0;
  drawNoise(ForestColors_p);
}

#if HAS_POLAR_COORDS // forestPolarNoise() uses drawPolarNoise()
void forestPolarNoise() {
  noisespeedx = 9;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(ForestColors_p);
}
#endif

void cloudNoise() {
  noisespeedx = -2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(CloudColors_p);
}

#if HAS_POLAR_COORDS // cloudPolarNoise() uses drawPolarNoise()
void cloudPolarNoise() {
  noisespeedx = 2;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 0;
  colorLoop = 0;
  drawPolarNoise(CloudColors_p);
}
#endif

void fireNoise() {
  noisespeedx = 0; // 24;
  noisespeedy = -32;
  noisespeedz = 0;
  noisescale = 16;
  colorLoop = 0;
  drawNoise(HeatColors_p, 60);
}

#if HAS_POLAR_COORDS // firePolarNoise() uses drawPolarNoise()
void firePolarNoise() {
//  noisespeedx = 0; // 24;
//  noisespeedy = -24;
//  noisespeedz = 0;
//  noisescale = 4;
//  colorLoop = 0;
  noisespeedx = 0; // 24;
  noisespeedy = -32;
  noisespeedz = 0;
  noisescale = 4;
  colorLoop = 0;
  drawPolarNoise(HeatColors_p, 60);
}
#endif

void fireNoise2() {
  noisespeedx = 0;
  noisespeedy = -8;
  noisespeedz = 3;
  noisescale = 8;
  colorLoop = 0;
  drawNoise(HeatColors_p);
}

#if HAS_POLAR_COORDS // firePolarNoise2() uses drawPolarNoise()
void firePolarNoise2() {
//  noisespeedx = 0;
//  noisespeedy = -8;
//  noisespeedz = 4;
//  noisescale = 1;
//  colorLoop = 0;
  noisespeedx = 0;
  noisespeedy = -4;
  noisespeedz = 4;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(HeatColors_p);
}
#endif

void lavaNoise() {
  noisespeedx = 0;
  noisespeedy = -1;
  noisespeedz = 1;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(LavaColors_p);
}

#if HAS_POLAR_COORDS // lavaPolarNoise() uses drawPolarNoise()
void lavaPolarNoise() {
  noisespeedx = 0;
  noisespeedy = -1;
  noisespeedz = 1;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(LavaColors_p);
}
#endif

void oceanNoise() {
  noisespeedx = -2;
  noisespeedy = 0;
  noisespeedz = 4;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(OceanColors_p);
}

#if HAS_POLAR_COORDS // oceanPolarNoise() uses drawPolarNoise()
void oceanPolarNoise() {
  noisespeedx = -1; // beatsin8(6, 0, 2) - 1;
  noisespeedy = 0;
  noisespeedz = 1;
  noisescale = 0;
  colorLoop = 0;
  drawPolarNoise(OceanColors_p);
}
#endif

void blackAndWhiteNoise() {
  noisespeedx = -12;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 6;
  colorLoop = 0;
  drawNoise(blackAndWhiteStripedPalette);
}

#if HAS_POLAR_COORDS // blackAndWhitePolarNoise() uses drawPolarNoise()
void blackAndWhitePolarNoise() {
  noisespeedx = -4; // beatsin8(8, 0, 9) - 4;
  noisespeedy = 0;
  noisespeedz = 0;
  noisescale = 0;
  colorLoop = 0;
  drawPolarNoise(blackAndWhiteStripedPalette);
}
#endif

void blackAndBlueNoise() {
  noisespeedx = 0;
  noisespeedy = 8;
  noisespeedz = 0;
  noisescale = 8;
  colorLoop = 0;
  drawNoise(blackAndBlueStripedPalette);
}

#if HAS_POLAR_COORDS // blackAndBluePolarNoise() uses drawPolarNoise()
void blackAndBluePolarNoise() {
  noisespeedx = 0;
  noisespeedy = -8; // beatsin8(8, 0, 16) - 8;
  noisespeedz = 0;
  noisescale = 1;
  colorLoop = 0;
  drawPolarNoise(blackAndBlueStripedPalette);
}
#endif

#endif // HAS_COORDINATE_MAP