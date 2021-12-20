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

#if !defined(ESP8266_FASTLED_WEBSERVER_PRODUCT_KRAKEN64_H)
#define ESP8266_FASTLED_WEBSERVER_PRODUCT_KRAKEN64_H

// FIBONACCI64 comes in multiple variants:
//   Full  (86mm) ==> ESP8266, WS2812B-5050 == ~60mA / pixel, no touch support built-in
//       https://www.tindie.com/products/jasoncoon/fibonacci64-86mm-disc-with-64-ws2812b-rgb-leds/
//                
//   Mini  (64mm) ==> ESP8266, WS2812B-3535 == ~??mA / pixel, no touch support built-in
//       https://www.tindie.com/products/jasoncoon/fibonacci64-mini-64mm-disc-with-64-rgb-leds/
//

// TODO: Enable Micro and Nano, which have no WiFi and use SAMD21E...
//   Micro (40mm) ==> QT Py,   WS2812C-2020 ==  ~5mA / pixel
//   Nano  (33mm) ==> QT Py,   SK6805-EC15  ==  ~5mA / pixel
//
//       https://www.tindie.com/products/jasoncoon/fibonacci64-micro-40mm-disc-with-64-rgb-leds/
//       https://github.com/jasoncoon/fibonacci64-touch-demo
//
//       https://www.tindie.com/products/jasoncoon/fibonacci64-nano-33mm-disc-with-64-rgb-leds/
//       https://github.com/jasoncoon/fibonacci64-nano-touch-demo
//
// NOTE: code sets to use WS2812B timings, even on Nano (SK6805 timings differ from WS2812B)
// NOTE: QT Py data pin A10, brightness 32
// NOTE: QT Py touch pins:
//       Micro: A0, A1, A2, A3
//       Nano:  A0, A1, A2
//

#if !defined(LED_TYPE)
   #define LED_TYPE                      WS2812B
#endif
#if !defined(COLOR_ORDER)
   #define COLOR_ORDER                   GRB
#endif
#if !defined(NUM_PIXELS)
   #define NUM_PIXELS                    64
#endif
#if !defined(AVAILABLE_MILLI_AMPS)
   #define AVAILABLE_MILLI_AMPS          1400 // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#endif
#if !defined(MAX_MILLI_AMPS_PER_PIXEL)
   #define MAX_MILLI_AMPS_PER_PIXEL      60   // IMPORTANT: set to larger value if necessary
#endif
#if !defined(FRAMES_PER_SECOND)
   #define FRAMES_PER_SECOND             120
#endif
#if !defined(DEFAULT_PATTERN_INDEX)
   #define DEFAULT_PATTERN_INDEX         0
#endif
#if !defined(DEFAULT_BRIGHTNESS_INDEX)
   #define DEFAULT_BRIGHTNESS_INDEX      3
#endif
#if !defined(DEFAULT_COLOR_CORRECTION)
   #define DEFAULT_COLOR_CORRECTION      TypicalSMD5050
#endif
#if !defined(NAME_PREFIX)
   #define NAME_PREFIX                   "Kraken64-"
#endif
#if !defined(PRODUCT_FRIENDLY_NAME)
   #define PRODUCT_FRIENDLY_NAME         "Kraken64"
#endif
#if !defined(IS_FIBONACCI)
   #define IS_FIBONACCI                  0
#endif
#if !defined(HAS_COORDINATE_MAP)
   #define HAS_COORDINATE_MAP            1
#endif
#if !defined(PARALLEL_OUTPUT_CHANNELS)
   #define PARALLEL_OUTPUT_CHANNELS      1
#endif


#endif // ESP8266_FASTLED_WEBSERVER_PRODUCT_KRAKEN64_H
