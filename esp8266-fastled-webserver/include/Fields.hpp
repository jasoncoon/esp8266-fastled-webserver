/*
   ESP8266 + FastLED + IR Remote: https://github.com/jasoncoon/esp8266-fastled-webserver
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
#pragma once
#if !defined(FIELDS_HPP)
#define FIELDS_HPP

extern uint8_t power;
extern uint8_t brightness;

void setShowClock(uint8_t value);
void setClockBackgroundFade(uint8_t value);
uint8_t setUtcOffsetIndex(uint8_t value);

String getFieldValue(String name);
String setFieldValue(String name, String value);
String getFieldsJson();


#endif
