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

#if !defined(ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP8266_H)
#define ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP8266_H

static_assert(PARALLEL_OUTPUT_CHANNELS <= 6, "ESP8266 only supports six parallel outputs");

#if !defined(DATA_PIN)
   #if PARALLEL_OUTPUT_CHANNELS == 1
      #define DATA_PIN   D5 // d1 mini
   #else
      #define DATA_PIN   D7 // Fib512 uses different default for primary output pin
   #endif
#endif

#if !defined(DATA_PIN_2) && PARALLEL_OUTPUT_CHANNELS >= 2
   #define DATA_PIN_2    D6 // d1 mini
#endif
#if !defined(DATA_PIN_3) && PARALLEL_OUTPUT_CHANNELS >= 3
   #define DATA_PIN_3    D8 // d1 mini
#endif
#if !defined(DATA_PIN_4) && PARALLEL_OUTPUT_CHANNELS >= 4
   #define DATA_PIN_4    D5 // d1 mini
#endif
#if !defined(DATA_PIN_5) && PARALLEL_OUTPUT_CHANNELS >= 5
   #define DATA_PIN_5    D1 // d1 mini
#endif
#if !defined(DATA_PIN_6) && PARALLEL_OUTPUT_CHANNELS >= 6
   #define DATA_PIN_6    D2 // d1 mini
#endif

#if defined(ENABLE_IR) && !defined(IR_RECV_PIN)
   #define IR_RECV_PIN   D4
#endif


#define FASTLED_INTERRUPT_RETRY_COUNT 0

#endif // ESP8266_FASTLED_WEBSERVER_CONTROLLER_ESP8266_H
