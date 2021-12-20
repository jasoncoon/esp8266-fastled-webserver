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

#if !defined(ESP8266_FASTLED_WEBSERVER_PRODUCT_FIB1024_MINI_H)
#define ESP8266_FASTLED_WEBSERVER_PRODUCT_FIB1024_MINI_H

#if !defined(LED_TYPE)
   #define LED_TYPE                      WS2812B
#endif
#if !defined(COLOR_ORDER)
   #define COLOR_ORDER                   GRB
#endif
#if !defined(NUM_PIXELS)
   #define NUM_PIXELS                    1024
#endif
#if !defined(AVAILABLE_MILLI_AMPS)
   #define AVAILABLE_MILLI_AMPS          10000 // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#endif
#if !defined(MAX_MILLI_AMPS_PER_PIXEL)
   #define MAX_MILLI_AMPS_PER_PIXEL      48   // IMPORTANT: set to larger value if necessary
#endif
#if !defined(FRAMES_PER_SECOND)
   #define FRAMES_PER_SECOND             120
#endif
#if !defined(DEFAULT_PATTERN_INDEX)
   #define DEFAULT_PATTERN_INDEX         3
#endif
#if !defined(DEFAULT_BRIGHTNESS_INDEX)
   #define DEFAULT_BRIGHTNESS_INDEX      3
#endif
#if !defined(DEFAULT_COLOR_CORRECTION)
   #define DEFAULT_COLOR_CORRECTION      TypicalSMD5050
#endif
#if !defined(NAME_PREFIX)
   #define NAME_PREFIX                   "Fibonacci1024-"
#endif
#if !defined(PRODUCT_FRIENDLY_NAME)
   #define PRODUCT_FRIENDLY_NAME         "Fibonacci1024"
#endif
#if !defined(IS_FIBONACCI)
   #define IS_FIBONACCI                  1
#endif
#if !defined(HAS_COORDINATE_MAP)
   #define HAS_COORDINATE_MAP            1
#endif
#if !defined(PARALLEL_OUTPUT_CHANNELS)
   #define PARALLEL_OUTPUT_CHANNELS      5
#endif
#if !defined(PIXELS_ON_DATA_PIN_1)
   #define PIXELS_ON_DATA_PIN_1 205
#endif
#if !defined(PIXELS_ON_DATA_PIN_2)
   #define PIXELS_ON_DATA_PIN_2 205
#endif
#if !defined(PIXELS_ON_DATA_PIN_3)
   #define PIXELS_ON_DATA_PIN_3 205
#endif
#if !defined(PIXELS_ON_DATA_PIN_4)
   #define PIXELS_ON_DATA_PIN_4 205
#endif
#if !defined(PIXELS_ON_DATA_PIN_5)
   #define PIXELS_ON_DATA_PIN_5 204
#endif

#if defined(ARDUINO_ARCH_ESP8266)
   // This board uses different data pins....
   #if !defined(DATA_PIN)
      #define DATA_PIN      D5 // d1 mini
   #endif
   #if !defined(DATA_PIN_2) && PARALLEL_OUTPUT_CHANNELS >= 2
      #define DATA_PIN_2    D7 // d1 mini
   #endif
   #if !defined(DATA_PIN_3) && PARALLEL_OUTPUT_CHANNELS >= 3
      #define DATA_PIN_3    D6 // d1 mini
   #endif
   #if !defined(DATA_PIN_4) && PARALLEL_OUTPUT_CHANNELS >= 4
      #define DATA_PIN_4    D1 // d1 mini
   #endif
   #if !defined(DATA_PIN_5) && PARALLEL_OUTPUT_CHANNELS >= 5
      #define DATA_PIN_5    D2 // d1 mini
   #endif
#elif defined(ARDUINO_ARCH_ESP32)
   #error "F1024 currently only supports ESP8266"
#else
   #error "F1024 currently only supports ESP8266"
#endif

#endif // ESP8266_FASTLED_WEBSERVER_PRODUCT_FIB1024_MINI_H
