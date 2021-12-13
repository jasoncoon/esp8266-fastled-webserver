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

#if !defined(ESP8266_FASTLED_WEBSERVER_CONTROLLER_TEMPLATE_H)
#define ESP8266_FASTLED_WEBSERVER_CONTROLLER_TEMPLATE_H

// This header file describes the configuration for specific controllers
// (e.g., ESP8266, ESP32, Adafruit QT Py, etc.)

// The only MANDATORY setting is DATA_PIN.


// The DATA_PIN defines the MCU's pin that is used to control the pixels
#define DATA_PIN                      D5 // d1 mini
// #define DATA_PIN                      18 // d1 mini32 (same physical location as D5 on the d1 mini)

// TODO: enable SPI-based chipsets based on whether CLOCK_PIN is defined?

// FastLED configurations that may need to change, depending on the controller in use...
// #define FASTLED_ALLOW_INTERRUPTS      1
// #define INTERRUPT_THRESHOLD           1
#define FASTLED_INTERRUPT_RETRY_COUNT 0

// ENABLE_IR defines whether to include support for an IR receiver
// #define ENABLE_IR
// #define IR_RECV_PIN                   D4 // d1 mini
// #define IR_RECV_PIN                   16 // d1 mini32 (same physical location as D4 on the d1 mini)

#endif // ESP8266_FASTLED_WEBSERVER_CONTROLLER_TEMPLATE_H
