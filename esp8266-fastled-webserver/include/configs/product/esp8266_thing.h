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
#pragma once

#if !defined(ESP8266_FASTLED_WEBSERVER_PRODUCT_ESP8266_THING_H)
#define ESP8266_FASTLED_WEBSERVER_PRODUCT_ESP8266_THING_H

#if !defined(ARDUINO_ARCH_ESP8266)
   #error "ESP8266 Thing currently only supports ESP8266"
#endif


#if !defined(LED_TYPE)
   #define LED_TYPE                      WS2811
#endif
#if !defined(COLOR_ORDER)
   #define COLOR_ORDER                   RGB
#endif
#if !defined(NUM_PIXELS)
   #define NUM_PIXELS                    300
#endif
#if !defined(AVAILABLE_MILLI_AMPS)
   #define AVAILABLE_MILLI_AMPS          10000 // !!! 10Amps !!! Verify your power supply !!! IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#endif
#if !defined(MAX_MILLI_AMPS_PER_PIXEL)
   #define MAX_MILLI_AMPS_PER_PIXEL      60    // IMPORTANT: set to larger value if necessary
#endif
#if !defined(FRAMES_PER_SECOND)
   #define FRAMES_PER_SECOND             120
#endif
#if !defined(DEFAULT_PATTERN_INDEX)
   #define DEFAULT_PATTERN_INDEX         3
#endif
#if !defined(DEFAULT_BRIGHTNESS_INDEX)
   #define DEFAULT_BRIGHTNESS_INDEX      0
#endif
#if !defined(DEFAULT_COLOR_CORRECTION)
   #define DEFAULT_COLOR_CORRECTION      TypicalLEDStrip
#endif
#if !defined(NAME_PREFIX)
   #define NAME_PREFIX                   "ESP8266-"
#endif
#if !defined(PRODUCT_FRIENDLY_NAME)
   #define PRODUCT_FRIENDLY_NAME         "ESP8266 Thing"
#endif
#if !defined(IS_FIBONACCI)
   #define IS_FIBONACCI                  0
#endif
#if !defined(HAS_COORDINATE_MAP)
   #define HAS_COORDINATE_MAP            0
#endif
#if !defined(PARALLEL_OUTPUT_CHANNELS)
   #define PARALLEL_OUTPUT_CHANNELS      6
#endif
#if !defined(PIXELS_ON_DATA_PIN_1)
   #define PIXELS_ON_DATA_PIN_1 50
#endif
#if !defined(PIXELS_ON_DATA_PIN_2)
   #define PIXELS_ON_DATA_PIN_2 50
#endif
#if !defined(PIXELS_ON_DATA_PIN_3)
   #define PIXELS_ON_DATA_PIN_3 50
#endif
#if !defined(PIXELS_ON_DATA_PIN_4)
   #define PIXELS_ON_DATA_PIN_4 50
#endif
#if !defined(PIXELS_ON_DATA_PIN_5)
   #define PIXELS_ON_DATA_PIN_5 50
#endif
#if !defined(PIXELS_ON_DATA_PIN_6)
   #define PIXELS_ON_DATA_PIN_6 50
#endif

#if defined(ARDUINO_ARCH_ESP8266)
   // This board uses different data pins....
   #if !defined(DATA_PIN)
      #define DATA_PIN      D1 // d1 mini
   #endif
   #if !defined(DATA_PIN_2) && PARALLEL_OUTPUT_CHANNELS >= 2
      #define DATA_PIN_2    D2 // d1 mini
   #endif
   #if !defined(DATA_PIN_3) && PARALLEL_OUTPUT_CHANNELS >= 3
      #define DATA_PIN_3    D5 // d1 mini
   #endif
   #if !defined(DATA_PIN_4) && PARALLEL_OUTPUT_CHANNELS >= 4
      #define DATA_PIN_4    D6 // d1 mini
   #endif
   #if !defined(DATA_PIN_5) && PARALLEL_OUTPUT_CHANNELS >= 5
      #define DATA_PIN_5    D7 // d1 mini
   #endif
   #if !defined(DATA_PIN_6) && PARALLEL_OUTPUT_CHANNELS >= 6
      #define DATA_PIN_6    D8 // d1 mini
   #endif
#elif defined(ARDUINO_ARCH_ESP32)
   #error "ESP8266 Thing currently only supports ESP8266"
#else
   #error "ESP8266 Thing currently only supports ESP8266"
#endif




#endif // ESP8266_FASTLED_WEBSERVER_PRODUCT_ESP8266_THING_H
