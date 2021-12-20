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

#if !defined(ESP8266_FASTLED_WEBSERVER_PRODUCT_1628RINGS_H)
#define ESP8266_FASTLED_WEBSERVER_PRODUCT_1628RINGS_H

#if !defined(LED_TYPE)
   #define LED_TYPE                      WS2812B
#endif
#if !defined(COLOR_ORDER)
   #define COLOR_ORDER                   GRB
#endif
#if !defined(NUM_PIXELS)
   #define NUM_PIXELS                    1628
#endif
#if !defined(AVAILABLE_MILLI_AMPS)
   #define AVAILABLE_MILLI_AMPS          2000 // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#endif
#if !defined(MAX_MILLI_AMPS_PER_PIXEL)
   #define MAX_MILLI_AMPS_PER_PIXEL      60   // IMPORTANT: set to larger value if necessary
#endif
#if !defined(FRAMES_PER_SECOND)
   #define FRAMES_PER_SECOND             1000 // No enforced delay?
#endif
#if !defined(DEFAULT_PATTERN_INDEX)
   #define DEFAULT_PATTERN_INDEX         0
#endif
#if !defined(DEFAULT_BRIGHTNESS_INDEX)
   #define DEFAULT_BRIGHTNESS_INDEX      0
#endif
#if !defined(DEFAULT_COLOR_CORRECTION)
   #define DEFAULT_COLOR_CORRECTION      UncorrectedColor // 1628-Rings did not specify; This is FastLED default
#endif
#if !defined(NAME_PREFIX)
   #define NAME_PREFIX                   "sol-v2-"
#endif
#if !defined(PRODUCT_FRIENDLY_NAME)
   #define PRODUCT_FRIENDLY_NAME         "Sol v2"
#endif
#if !defined(IS_FIBONACCI)
   #define IS_FIBONACCI                  0
#endif
#if !defined(HAS_COORDINATE_MAP)
   #define HAS_COORDINATE_MAP            1
#endif
#if !defined(PARALLEL_OUTPUT_CHANNELS) // WARNING:  Refresh rate for 1628 pixels over single output is ~15 frames / second MAXIMUM....
   #define PARALLEL_OUTPUT_CHANNELS      1
#endif




#endif // ESP8266_FASTLED_WEBSERVER_PRODUCT_1628RINGS_H
