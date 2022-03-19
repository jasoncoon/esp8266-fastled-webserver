/*
   ESP8266 + FastLED Web Server: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2016 Jason Coon

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

#if IS_FIBONACCI

// Fibonacci Stars pattern by Jason Coon
// Draws shooting stars radiating outward from the center, along Fibonacci spiral lines.
void fibonacciStarsWithOffset(uint16_t stars[], uint8_t starCount, uint8_t offset = 21, bool setup = false, bool move = false)
{
  // use a number from the Fibonacci sequence for offset to follow a spiral out from the center

  for (uint8_t i = 0; i < starCount; i++)
  {
    if (setup || stars[i] >= NUM_PIXELS)
    {
      // reset star
      stars[i] = random8(offset - 1);
    }

    uint16_t index = fibonacciToPhysical[stars[i]];

    // draw the star
    leds[index] = ColorFromPalette(gCurrentPalette, stars[i] - gHue); // i * (240 / starCount)
  }

  // move the stars
  if (move)
  {
    for (uint8_t i = 0; i < starCount; i++)
    {
      stars[i] = stars[i] + offset;
    }
  }
}

const uint8_t starCount = NUM_PIXELS >= 256 ? 4 : 2;

void fibonacciStars8(bool setup = false, bool move = false)
{
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 8, setup, move);
}

void fibonacciStars13(bool setup = false, bool move = false)
{
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 13, setup, move);
}

void fibonacciStars21(bool setup = false, bool move = false)
{
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 21, setup, move);
}

void fibonacciStars34(bool setup = false, bool move = false)
{
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 34, setup, move);
}

void fibonacciStars55(bool setup = false, bool move = false) {
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 55, setup, move);
}

void fibonacciStars89(bool setup = false, bool move = false) {
  static uint16_t stars[starCount];
  fibonacciStarsWithOffset(stars, starCount, 89, setup, move);
}

// called from Arduino loop
void fibonacciStars()
{
  bool move = false;
  static bool setup = true;
  fadeToBlackBy(leds, NUM_PIXELS, 8);

  EVERY_N_MILLIS(60)
  {
    move = true;
  }

  if (NUM_PIXELS >= 128) {
    fibonacciStars8(setup, move);
    fibonacciStars13(setup, move);
  } else if (NUM_PIXELS >= 256) {
    fibonacciStars13(setup, move);
    fibonacciStars21(setup, move);
    fibonacciStars34(setup, move);
  } else if (NUM_PIXELS >= 1024) {
    fibonacciStars34(setup, move);
    fibonacciStars55(setup, move);
    fibonacciStars89(setup, move);
  }
  setup = false;
}

#endif