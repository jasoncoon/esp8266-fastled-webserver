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

uint8_t power = 1;
uint8_t brightness = brightnessMap[brightnessIndex];

//String setPower(String value) {
//  power = value.toInt();
//  if(power < 0) power = 0;
//  else if (power > 1) power = 1;
//  return String(power);
//}

String getPower() {
  return String(power);
}

//String setBrightness(String value) {
//  brightness = value.toInt();
//  if(brightness < 0) brightness = 0;
//  else if (brightness > 255) brightness = 255;
//  return String(brightness);
//}

String getBrightness() {
  return String(brightness);
}

String getPattern() {
  return String(currentPatternIndex);
}

String getPatterns() {
  String json = "";

  for (uint8_t i = 0; i < patternCount; i++) {
    json += "\"" + patterns[i].name + "\"";
    if (i < patternCount - 1)
      json += ",";
  }

  return json;
}

String getPalette() {
  return String(currentPaletteIndex);
}

String getPalettes() {
  String json = "";

  for (uint8_t i = 0; i < paletteCount; i++) {
    json += "\"" + paletteNames[i] + "\"";
    if (i < paletteCount - 1)
      json += ",";
  }

  return json;
}

String getAutoplay() {
  return String(autoplay);
}

String getAutoplayDuration() {
  return String(autoplayDuration);
}

String getShowClock() {
  return String(showClock);
}

String getClockBackgroundFade() {
  return String(clockBackgroundFade);
}

void setShowClock(uint8_t value)
{
  showClock = value == 0 ? 0 : 1;

  EEPROM.write(9, showClock);
  EEPROM.commit();

  broadcastInt("showClock", showClock);
}

void setClockBackgroundFade(uint8_t value)
{
  clockBackgroundFade = value;

  EEPROM.write(10, clockBackgroundFade);
  EEPROM.commit();

  broadcastInt("clockBackgroundFade", clockBackgroundFade);
}

String getTimeZone() {
  return String(currentTimeZoneIndex);
}

String getTimeZones() {
  String json = "";

  for (uint8_t i = 0; i < timeZoneCount; i++) {
    json += "\"" + timeZones[i].utcOffset + " – " + timeZones[i].region + "\"";
    if (i < timeZoneCount - 1)
      json += ",";
  }

  return json;
}

String setTimeZone(uint8_t value)
{
  if (value >= timeZoneCount)
    value = timeZoneCount - 1;

  currentTimeZoneIndex = value;

  EEPROM.write(11, currentTimeZoneIndex);
  EEPROM.commit();

  timeClient.setTimeOffset(timeZones[currentTimeZoneIndex].utcOffsetInSeconds + (dst ? 3600 : 0));

  broadcastInt("timeZone", currentTimeZoneIndex);
}

String getDst() {
  return String(dst);
}

String setDst(uint8_t value)
{
  dst = value == 0 ? 0 : 1;

  EEPROM.write(12, dst);
  EEPROM.commit();

  timeClient.setTimeOffset(timeZones[currentTimeZoneIndex].utcOffsetInSeconds + (dst ? 3600 : 0));

  broadcastInt("dst", dst);
}


String getSolidColor() {
  return String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b);
}

String getCooling() {
  return String(cooling);
}

String getSparking() {
  return String(sparking);
}

String getSpeed() {
  return String(speed);
}

String getTwinkleSpeed() {
  return String(twinkleSpeed);
}

String getTwinkleDensity() {
  return String(twinkleDensity);
}

String getName() {
  return nameString;
}

// Pride Playground fields

String getSaturationBpm() {
  return String(saturationBpm);
}
String setSaturationBpm(String value)
{
  saturationBpm = value.toInt(); return value;
}

String getSaturationMin() {
  return String(saturationMin);
}
String setSaturationMin(String value) {
  saturationMin = value.toInt(); return value;
}

String getSaturationMax() {
  return String(saturationMax);
}
String setSaturationMax(String value) {
  saturationMax = value.toInt(); return value;
}

String getBrightDepthBpm() {
  return String(brightDepthBpm);
}
String setBrightDepthBpm(String value) {
  brightDepthBpm = value.toInt(); return value;
}

String getBrightDepthMin() {
  return String(brightDepthMin);
}
String setBrightDepthMin(String value) {
  brightDepthMin = value.toInt(); return value;
}

String getBrightDepthMax() {
  return String(brightDepthMax);
}
String setBrightDepthMax(String value) {
  brightDepthMax = value.toInt(); return value;
}

String getBrightThetaIncBpm() {
  return String(brightThetaIncBpm);
}
String setBrightThetaIncBpm(String value) {
  brightThetaIncBpm = value.toInt(); return value;
}

String getBrightThetaIncMin() {
  return String(brightThetaIncMin);
}
String setBrightThetaIncMin(String value) {
  brightThetaIncMin = value.toInt(); return value;
}

String getBrightThetaIncMax() {
  return String(brightThetaIncMax);
}
String setBrightThetaIncMax(String value) {
  brightThetaIncMax = value.toInt(); return value;
}

String getMsMultiplierBpm() {
  return String(msMultiplierBpm);
}
String setMsMultiplierBpm(String value) {
  msMultiplierBpm = value.toInt(); return value;
}

String getMsMultiplierMin() {
  return String(msMultiplierMin);
}
String setMsMultiplierMin(String value) {
  msMultiplierMin = value.toInt(); return value;
}

String getMsMultiplierMax() {
  return String(msMultiplierMax);
}
String setMsMultiplierMax(String value) {
  msMultiplierMax = value.toInt(); return value;
}

String getHueIncBpm() {
  return String(hueIncBpm);
}
String setHueIncBpm(String value) {
  hueIncBpm = value.toInt(); return value;
}

String getHueIncMin() {
  return String(hueIncMin);
}
String setHueIncMin(String value) {
  hueIncMin = value.toInt(); return value;
}

String getHueIncMax() {
  return String(hueIncMax);
}
String setHueIncMax(String value) {
  hueIncMax = value.toInt(); return value;
}

String getSHueBpm() {
  return String(sHueBpm);
}
String setSHueBpm(String value) {
  sHueBpm = value.toInt(); return value;
}

String getSHueMin() {
  return String(sHueMin);
}
String setSHueMin(String value) {
  sHueMin = value.toInt(); return value;
}

String getSHueMax() {
  return String(sHueMax);
}
String setSHueMax(String value) {
  sHueMax = value.toInt(); return value;
}

FieldList fields = {
    {"name", "Name", LabelFieldType, 0, 0, getName},

    {"power", "Power", BooleanFieldType, 0, 1, getPower},
    {"brightness", "Brightness", NumberFieldType, 1, 255, getBrightness},
    {"pattern", "Pattern", SelectFieldType, 0, patternCount, getPattern, getPatterns},
    {"palette", "Palette", SelectFieldType, 0, paletteCount, getPalette, getPalettes},
    {"speed", "Speed", NumberFieldType, 1, 255, getSpeed},

    {"autoplaySection", "Autoplay", SectionFieldType},
    {"autoplay", "Autoplay", BooleanFieldType, 0, 1, getAutoplay},
    {"autoplayDuration", "Autoplay Duration", NumberFieldType, 0, 255, getAutoplayDuration},
    
    {"clock", "Clock", SectionFieldType},
    {"showClock", "Show Clock", BooleanFieldType, 0, 1, getShowClock},
    {"clockBackgroundFade", "Background Fade", NumberFieldType, 0, 255, getClockBackgroundFade},
    {"timeZone", "Time Zone", SelectFieldType, 0, timeZoneCount, getTimeZone, getTimeZones},
    {"dst", "Daylight Saving", BooleanFieldType, 0, 1, getDst},

    {"solidColorSection", "Solid Color", SectionFieldType},
    {"solidColor", "Color", ColorFieldType, 0, 255, getSolidColor},

    {"prideSection", "Pride & ColorWaves", SectionFieldType},

    {"saturationBpm", "Saturation BPM", NumberFieldType, 0, 255, getSaturationBpm, NULL, setSaturationBpm},
    {"saturationMin", "Saturation Min", NumberFieldType, 0, 255, getSaturationMin, NULL, setSaturationMin},
    {"saturationMax", "Saturation Max", NumberFieldType, 0, 255, getSaturationMax, NULL, setSaturationMax},

    {"brightDepthBpm", "Brightness Depth BPM", NumberFieldType, 0, 255, getBrightDepthBpm, NULL, setBrightDepthBpm},
    {"brightDepthMin", "Brightness Depth Min", NumberFieldType, 0, 255, getBrightDepthMin, NULL, setBrightDepthMin},
    {"brightDepthMax", "Brightness Depth Max", NumberFieldType, 0, 255, getBrightDepthMax, NULL, setBrightDepthMax},

    {"brightThetaIncBpm", "Bright Theta Inc BPM", NumberFieldType, 0, 255, getBrightThetaIncBpm, NULL, setBrightThetaIncBpm},
    {"brightThetaIncMin", "Bright Theta Inc Min", NumberFieldType, 0, 255, getBrightThetaIncMin, NULL, setBrightThetaIncMin},
    {"brightThetaIncMax", "Bright Theta Inc Max", NumberFieldType, 0, 255, getBrightThetaIncMax, NULL, setBrightThetaIncMax},

    {"msMultiplierBpm", "Time Multiplier BPM", NumberFieldType, 0, 255, getMsMultiplierBpm, NULL, setMsMultiplierBpm},
    {"msMultiplierMin", "Time Multiplier Min", NumberFieldType, 0, 255, getMsMultiplierMin, NULL, setMsMultiplierMin},
    {"msMultiplierMax", "Time Multiplier Max", NumberFieldType, 0, 255, getMsMultiplierMax, NULL, setMsMultiplierMax},

    {"hueIncBpm", "Hue Inc BPM", NumberFieldType, 0, 255, getHueIncBpm, NULL, setHueIncBpm},
    {"hueIncMin", "Hue Inc Min", NumberFieldType, 0, 255, getHueIncMin, NULL, setHueIncMin},
    {"hueIncMax", "Hue Inc Max", NumberFieldType, 0, 255, getHueIncMax, NULL, setHueIncMax},

    {"sHueBpm", "S Hue BPM", NumberFieldType, 0, 255, getSHueBpm, NULL, setSHueBpm},
    {"sHueMin", "S Hue Min", NumberFieldType, 0, 255, getSHueMin, NULL, setSHueMin},
    {"sHueMax", "S Hue Max", NumberFieldType, 0, 255, getSHueMax, NULL, setSHueMax},


    {"fireSection", "Fire & Water", SectionFieldType},
    {"cooling", "Cooling", NumberFieldType, 0, 255, getCooling},
    {"sparking", "Sparking", NumberFieldType, 0, 255, getSparking},

    {"twinklesSection", "Twinkles", SectionFieldType},
    {"twinkleSpeed", "Twinkle Speed", NumberFieldType, 0, 8, getTwinkleSpeed},
    {"twinkleDensity", "Twinkle Density", NumberFieldType, 0, 8, getTwinkleDensity}
};

uint8_t fieldCount = ARRAY_SIZE(fields);
