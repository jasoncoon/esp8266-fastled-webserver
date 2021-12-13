/*
   ESP8266 FastLED WebServer: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) Jason Coon

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

#if !defined(ESP8266_FASTLED_WEBSERVER_COMMON_H)
#define ESP8266_FASTLED_WEBSERVER_COMMON_H

#include "Arduino.h"

// config.h must be included before FastLED, as it may define FastLED options
// such as whether to disable interrupts, etc.
// config.h defines pre-processor symbols, but does not itself define functions, structs, etc.
#include "config.h"

#if 1 // external libraries
  #define ARDUINOJSON_DECODE_UNICODE 0 // don't need to decode Unicode to UTF-8
  // storing uint32_t requires this flag
  #define ARDUINOJSON_USE_LONG_LONG 1
  #include "ArduinoJson.h"

  #define FASTLED_INTERNAL // no other way to suppress build warnings
  #include <FastLED.h>
  FASTLED_USING_NAMESPACE

  #include <LittleFS.h>
  #define MYFS LittleFS

  #include <NTPClient.h>
  #include <ESP8266WiFi.h>
  #include <WiFiUdp.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPUpdateServer.h>
  #include <ESP8266HTTPClient.h>
  #include <EEPROM.h>
  #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager/tree/development

  #include "./include/simplehacks/static_eval.h"
  #include "./include/simplehacks/constexpr_strlen.h"
  #include "./include/simplehacks/array_size2.h"
#endif // 1

void dimAll(byte value);
// info.cpp
String WiFi_SSID(bool persistent);
String getInfoJson();


// Ugly macro-like constexpr, used for FastLED template arguments
template <size_t ONE_BASED_OUTPUT_CHANNEL>
constexpr int LedOffset() {
  static_assert(ONE_BASED_OUTPUT_CHANNEL <= PARALLEL_OUTPUT_CHANNELS, "");
  static_assert(ONE_BASED_OUTPUT_CHANNEL >= 1, "");
  return 0 // this would be much simpler with C++14
  #if PARALLEL_OUTPUT_CHANNELS >= 2
    + ((ONE_BASED_OUTPUT_CHANNEL >= 2) ? PIXELS_ON_DATA_PIN_1 : 0)
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 3
    + ((ONE_BASED_OUTPUT_CHANNEL >= 3) ? PIXELS_ON_DATA_PIN_2 : 0)
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 4
    + ((ONE_BASED_OUTPUT_CHANNEL >= 4) ? PIXELS_ON_DATA_PIN_3 : 0)
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 5
    + ((ONE_BASED_OUTPUT_CHANNEL >= 5) ? PIXELS_ON_DATA_PIN_4 : 0)
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 6
    + ((ONE_BASED_OUTPUT_CHANNEL >= 6) ? PIXELS_ON_DATA_PIN_5 : 0)
  #endif
  ;
}
template <size_t ONE_BASED_OUTPUT_CHANNEL>
constexpr int LedCount() {
  static_assert(ONE_BASED_OUTPUT_CHANNEL <= PARALLEL_OUTPUT_CHANNELS, "");
  static_assert(ONE_BASED_OUTPUT_CHANNEL >= 1, "");
  #if PARALLEL_OUTPUT_CHANNELS == 1
    return NUM_PIXELS;
  #else
    return // this would be much simpler with C++14
    #if PARALLEL_OUTPUT_CHANNELS >= 6
      (ONE_BASED_OUTPUT_CHANNEL == 6) ? PIXELS_ON_DATA_PIN_6 :
    #endif
    #if PARALLEL_OUTPUT_CHANNELS >= 5
      (ONE_BASED_OUTPUT_CHANNEL == 5) ? PIXELS_ON_DATA_PIN_5 :
    #endif
    #if PARALLEL_OUTPUT_CHANNELS >= 4
      (ONE_BASED_OUTPUT_CHANNEL == 4) ? PIXELS_ON_DATA_PIN_4 :
    #endif
    #if PARALLEL_OUTPUT_CHANNELS >= 3
      (ONE_BASED_OUTPUT_CHANNEL == 3) ? PIXELS_ON_DATA_PIN_3 :
    #endif
    #if PARALLEL_OUTPUT_CHANNELS >= 2
      (ONE_BASED_OUTPUT_CHANNEL == 2) ? PIXELS_ON_DATA_PIN_2 :
    #endif
    #if PARALLEL_OUTPUT_CHANNELS >= 1
      (ONE_BASED_OUTPUT_CHANNEL == 1) ? PIXELS_ON_DATA_PIN_1 :
    #endif
      0;
  #endif
}

void writeAndCommitSettings();

#if defined(ESP32) || defined(ESP8266)
  // Optional: (LGPL) https://github.com/sinricpro/ESPTrueRandom.git#ed198f459da6d7af65dd13317a4fdc97b23991b4
  // #include "ESPTrueRandom.h"
  // Then:
  //     ESPTrueRandom.useRNG = true;
  //     int32_t r = ESPTrueRandom.memfill((void*)&r, sizeof(r));
#else
  #error "Currently only ESP32 and ESP8266 are supported"
#endif


// Structures
typedef void (*Pattern)();
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;

typedef struct {
  CRGBPalette16 palette;
  String name;
} PaletteAndName;


// forward-declarations

// TODO: cleanup to declare minimum necessary variables
// extern const uint8_t brightnessCount;
extern const uint8_t brightnessMap[];
extern uint8_t brightnessIndex;
extern uint8_t currentPatternIndex;
extern const uint8_t patternCount;
extern const PatternAndName patterns[];

extern uint8_t currentPaletteIndex;
extern uint8_t autoplay;
extern uint8_t autoplayDuration;
extern uint8_t showClock;
extern uint8_t clockBackgroundFade;
extern CRGB    solidColor;
extern uint8_t cooling;
extern uint8_t sparking;
extern uint8_t speed;
extern uint8_t twinkleSpeed;
extern uint8_t twinkleDensity;
extern uint8_t coolLikeIncandescent;
extern uint8_t saturationBpm;
extern uint8_t saturationMin;
extern uint8_t saturationMax;
extern uint8_t brightDepthBpm;
extern uint8_t brightDepthMin;
extern uint8_t brightDepthMax;
extern uint8_t brightThetaIncBpm;
extern uint8_t brightThetaIncMin;
extern uint8_t brightThetaIncMax;
extern uint8_t msMultiplierBpm;
extern uint8_t msMultiplierMin;
extern uint8_t msMultiplierMax;
extern uint8_t hueIncBpm;
extern uint8_t hueIncMin;
extern uint8_t hueIncMax;
extern uint8_t sHueBpm;
extern uint8_t sHueMin;
extern uint8_t sHueMax;



extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;
extern const CRGBPalette16 palettes[];
extern const uint8_t paletteCount;
extern const String paletteNames[];

extern CRGBPalette16 gCurrentPalette;
extern CRGBPalette16 gTargetPalette;
extern uint8_t gCurrentPaletteNumber;

extern WiFiManager wifiManager;
extern ESP8266WebServer webServer;
extern NTPClient timeClient;
extern String nameString;
extern int utcOffsetInSeconds;
extern uint8_t utcOffsetIndex;

extern CRGB leds[NUM_PIXELS];

#if IS_FIBONACCI // actual data in map.h
  #if NUM_PIXELS > 256 // when more than 256 pixels, cannot store index in uint8_t....
    extern const uint16_t physicalToFibonacci [NUM_PIXELS];
    extern const uint16_t fibonacciToPhysical [NUM_PIXELS];
  #else
    extern const uint8_t physicalToFibonacci [NUM_PIXELS];
    extern const uint8_t fibonacciToPhysical [NUM_PIXELS];
  #endif
#elif defined(PRODUCT_KRAKEN64)
  extern const uint8_t  body    [NUM_PIXELS];
#endif

#if HAS_COORDINATE_MAP
  extern const uint8_t  coordsX [NUM_PIXELS];
  extern const uint8_t  coordsY [NUM_PIXELS];
  extern const uint8_t  angles  [NUM_PIXELS];
#endif

#if HAS_POLAR_COORDS
  extern const uint8_t  radii[NUM_PIXELS];  // needed in noise.cpp
#endif

#include "include/GradientPalettes.hpp"
#include "include/Fields.hpp"
#include "include/FSBrowser.hpp"


// IR (commands.cpp)
#if ENABLE_IR
  void InitializeIR(void);
  void handleIrInput();
#else
  inline void handleIrInput() {}
#endif // ENABLE_IR

// TODO: move these functions out of main .INO file
// they are currently used by IR & main .INO file
#if ENABLE_IR
  void setPower(uint8_t value);
  void setAutoplay(uint8_t value);
  //void setAutoplayDuration(uint8_t value);
  void setSolidColor(CRGB color);
  //void setSolidColor(uint8_t r, uint8_t g, uint8_t b);
  void adjustPattern(bool up);
  void setPattern(uint8_t value);
  //void setPatternName(String name);
  void adjustBrightness(bool up);
#endif // ENABLE_IR


#if ENABLE_WEBSOCKETS
  void InitializeWebSocketServer(void);
  void broadcastInt(String name, uint8_t value);
  void broadcastString(String name, String value);
#else
  // these are put here for convenience
  inline void broadcastInt(String, uint8_t) {};
  inline void broadcastString(String, String) {};
#endif

// ping.cpp
void checkPingTimer();

// effects
// twinkles.cpp
void currentPaletteTwinkles();
void cloudTwinkles();
void rainbowTwinkles();
void snowTwinkles();
void incandescentTwinkles();
// twinkleFox.cpp
void redGreenWhiteTwinkles();
void hollyTwinkles();
void redWhiteTwinkles();
void blueWhiteTwinkles();
void fairyLightTwinkles();
void snow2Twinkles();
void iceTwinkles();
void retroC9Twinkles();
void partyTwinkles();
void forestTwinkles();
void lavaTwinkles();
void fireTwinkles();
void cloud2Twinkles();
void oceanTwinkles();
// map.h -- only when product defines HAS_COORDINATE_MAP to be true
#if HAS_COORDINATE_MAP
void anglePalette();
void xPalette();
void yPalette();
void xyPalette();
void xGradientPalette();
void yGradientPalette();
void xyGradientPalette();
void radarSweepPalette();
void radiusPalette();
void angleGradientPalette();
void radiusGradientPalette();
#endif
// map.h -- only when product defines IS_FIBONACCI to be true
#if IS_FIBONACCI
void drawAnalogClock();
void drawSpiralAnalogClock13();
void drawSpiralAnalogClock21();
void drawSpiralAnalogClock34();
void drawSpiralAnalogClock55();
void drawSpiralAnalogClock89();
void drawSpiralAnalogClock21and34();
void drawSpiralAnalogClock13_21_and_34();
void drawSpiralAnalogClock34_21_and_13();
void antialiasPixelAR(uint8_t angle, uint8_t dAngle, uint8_t startRadius, uint8_t endRadius, CRGB color, CRGB leds[] = leds, int _NUM_PIXELS = NUM_PIXELS);
#endif

// noise.h -- always defined
void rainbowNoise();
void rainbowStripeNoise();
void partyNoise();
void forestNoise();
void cloudNoise();
void fireNoise();
void fireNoise2();
void lavaNoise();
void oceanNoise();
void blackAndWhiteNoise();
void blackAndBlueNoise();

// noise.h -- only when product defines HAS_POLAR_COORDS to be true
#if HAS_POLAR_COORDS
void palettePolarNoise();
void gradientPalettePolarNoise();
void rainbowPolarNoise();
void rainbowStripePolarNoise();
void partyPolarNoise();
void forestPolarNoise();
void cloudPolarNoise();
void firePolarNoise();
void firePolarNoise2();
void lavaPolarNoise();
void oceanPolarNoise();
void blackAndWhitePolarNoise();
void blackAndBluePolarNoise();
#endif

// pacifica.h / prideplayground.h / colorwavesplayground.h
void pacifica_loop();
void pridePlayground();
void colorWavesPlayground();
#if IS_FIBONACCI
void pacifica_fibonacci_loop();
void pridePlaygroundFibonacci();
void colorWavesPlaygroundFibonacci();
#endif



#endif // ESP8266_FASTLED_WEBSERVER_COMMON_H
