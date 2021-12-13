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

#if !defined(ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP32_H)
#define ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP32_H


// Would need to define (and use) the additional DATA_PIN_x symbols,
// and update the corresponding code using those symbols.
static_assert(PARALLEL_OUTPUT_CHANNELS <= 4, "While ESP32 supports 16 parallel outputs, currently only support four parallel outputs");

// TODO: consider using I2S (instead of RMT) on the ESP32:
//
// #define FASTLED_ESP32_I2S true
//
// See https://github.com/FastLED/FastLED/issues/1220#issuecomment-822677011

#if !defined(DATA_PIN)
   #if PARALLEL_OUTPUT_CHANNELS == 1
      #define DATA_PIN   18 // d1 mini32 (same physical location as D5 on the d1 mini)
   #else
      #define DATA_PIN   23 // d1 mini32 (same physical location as D7 on the d1 mini)
   #endif
#endif

#if !defined(DATA_PIN_2) && PARALLEL_OUTPUT_CHANNELS >= 2
   #define DATA_PIN_2    19 // d1 mini32 (same physical location as D6 on the d1 mini)
#endif
#if !defined(DATA_PIN_3) && PARALLEL_OUTPUT_CHANNELS >= 3
   #define DATA_PIN_3     5 // d1 mini32 (same physical location as D8 on the d1 mini)
#endif
#if !defined(DATA_PIN_4) && PARALLEL_OUTPUT_CHANNELS >= 4
   #define DATA_PIN_4    18 // d1 mini32 (same physical location as D5 on the d1 mini)
#endif



#if defined(ENABLE_IR) && !defined(IR_RECV_PIN)
   // Default pin for ESP32 is 16 (for d1 mini32, this is the same physical location as D4 on the d1 mini)
   #define IR_RECV_PIN   16  // TODO: VERIFY THIS IS CORRECT VALUE
#endif

#endif // ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP32_H
