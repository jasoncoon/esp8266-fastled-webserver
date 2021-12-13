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
#include "common.h"

// only items outside the anonymous namespace can be exported.
// this keeps the global namespace cleaner, with no runtime/RAM costs.
inline namespace {
  enum struct Field_t : uint8_t {
    Number,
    Boolean,
    Select,
    Color,
    Section,
    String,
    Label,
    UtcOffset,
  };
  constexpr bool IsValid(const Field_t& t) {
    return (
      (t == Field_t::Number   ) ||
      (t == Field_t::Boolean  ) ||
      (t == Field_t::Select   ) ||
      (t == Field_t::Color    ) ||
      (t == Field_t::Section  ) ||
      (t == Field_t::String   ) ||
      (t == Field_t::Label    ) ||
      (t == Field_t::UtcOffset) );
  }

  const String ToString(const Field_t& t) {
    return
      (t == Field_t::String)    ?  F("String")   :
      (t == Field_t::Label)     ?  F("Label")    :
      (t == Field_t::Color)     ?  F("Color")    :
      (t == Field_t::Boolean)   ?  F("Boolean")  :
      (t == Field_t::Number)    ?  F("Number")   :
      (t == Field_t::Section)   ?  F("Section")  :
      (t == Field_t::Select)    ?  F("Select")   :
      (t == Field_t::UtcOffset) ?  F("UtcOffsetIndex") :
                                   F("Invalid")  ;
  }

  inline namespace LegacyStringGettersAndSetters { // This just helps folding / hiding these functions


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

    // TODO: wrapper class for EEPROM settings, to help ensure clear understanding
    //       of all locations that can cause settings committed / EEPROM write.
    //       Also simplifies reading prior value before writing new value (extend EEPROM life)
    //       Also simplifies later extending CRC, transactional updates, bypassing "dead"
    //       areas of EEPROM, etc.

    String getBrightness() {
      return String(brightness);
    }

    String getPattern() {
      return String(currentPatternIndex);
    }

    String getPalette() {
      return String(currentPaletteIndex);
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

    String getCoolLikeIncandescent() {
      return String(coolLikeIncandescent);
    }

    String getName() {
      return nameString;
    }

    String getUtcOffsetIndex() {
      return String(utcOffsetIndex);
    }

    String setUtcOffsetIndexString(String value)
    {
      auto tmp = setUtcOffsetIndex(value.toInt()); 
      return String(tmp);
    }

    // Pride Playground fields

    String getSaturationBpm() {
      return String(saturationBpm);
    }
    String setSaturationBpm(String value)
    {
      saturationBpm = value.toInt();
      return value;
    }

    String getSaturationMin() {
      return String(saturationMin);
    }
    String setSaturationMin(String value) {
      saturationMin = value.toInt();
      return value;
    }

    String getSaturationMax() {
      return String(saturationMax);
    }
    String setSaturationMax(String value) {
      saturationMax = value.toInt();
      return value;
    }

    String getBrightDepthBpm() {
      return String(brightDepthBpm);
    }
    String setBrightDepthBpm(String value) {
      brightDepthBpm = value.toInt();
      return value;
    }

    String getBrightDepthMin() {
      return String(brightDepthMin);
    }
    String setBrightDepthMin(String value) {
      brightDepthMin = value.toInt();
      return value;
    }

    String getBrightDepthMax() {
      return String(brightDepthMax);
    }
    String setBrightDepthMax(String value) {
      brightDepthMax = value.toInt();
      return value;
    }

    String getBrightThetaIncBpm() {
      return String(brightThetaIncBpm);
    }
    String setBrightThetaIncBpm(String value) {
      brightThetaIncBpm = value.toInt();
      return value;
    }

    String getBrightThetaIncMin() {
      return String(brightThetaIncMin);
    }
    String setBrightThetaIncMin(String value) {
      brightThetaIncMin = value.toInt();
      return value;
    }

    String getBrightThetaIncMax() {
      return String(brightThetaIncMax);
    }
    String setBrightThetaIncMax(String value) {
      brightThetaIncMax = value.toInt();
      return value;
    }

    String getMsMultiplierBpm() {
      return String(msMultiplierBpm);
    }
    String setMsMultiplierBpm(String value) {
      msMultiplierBpm = value.toInt();
      return value;
    }

    String getMsMultiplierMin() {
      return String(msMultiplierMin);
    }
    String setMsMultiplierMin(String value) {
      msMultiplierMin = value.toInt();
      return value;
    }

    String getMsMultiplierMax() {
      return String(msMultiplierMax);
    }
    String setMsMultiplierMax(String value) {
      msMultiplierMax = value.toInt();
      return value;
    }

    String getHueIncBpm() {
      return String(hueIncBpm);
    }
    String setHueIncBpm(String value) {
      hueIncBpm = value.toInt();
      return value;
    }

    String getHueIncMin() {
      return String(hueIncMin);
    }
    String setHueIncMin(String value) {
      hueIncMin = value.toInt();
      return value;
    }

    String getHueIncMax() {
      return String(hueIncMax);
    }
    String setHueIncMax(String value) {
      hueIncMax = value.toInt();
      return value;
    }

    String getSHueBpm() {
      return String(sHueBpm);
    }
    String setSHueBpm(String value) {
      sHueBpm = value.toInt();
      return value;
    }

    String getSHueMin() {
      return String(sHueMin);
    }
    String setSHueMin(String value) {
      sHueMin = value.toInt();
      return value;
    }

    String getSHueMax() {
      return String(sHueMax);
    }
    String setSHueMax(String value) {
      sHueMax = value.toInt();
      return value;
    }

  }

  inline namespace Options {
    void getPatterns(JsonVariant dst) {
      for (uint8_t i = 0; i < patternCount; i++) {
        dst.add(patterns[i].name);
      }
    }

    void getPalettes(JsonVariant dst) {
      for (uint8_t i = 0; i < paletteCount; i++) {
        dst.add(paletteNames[i]);
      }
    }
  }

  typedef String (*FieldSetter)(String);
  typedef String (*FieldGetter)();
  typedef void (*FieldOptions)(JsonVariant);
  struct Field {
    const String name;
    const String label;
    Field_t type;
    uint8_t min;
    uint8_t max;
    FieldGetter  getValue;
    FieldOptions getOptions;
    FieldSetter  setValue;
  };

  void convertToJson(const Field& field, JsonVariant dst) {
    if (field.name.length() == 0) {
      return;
    }
    if (!IsValid(field.type)) {
      return;
    }

    dst[F("name")]  = field.name;
    dst[F("label")] = field.label;

    dst[F("type")]  = ToString(field.type);
    // obj[F("readonly")] = (field.setValue == nullptr);
    if (field.getValue != nullptr) {
      switch (field.type) {
        case Field_t::Color: // legacy ... comma-separated string of decimals values
        case Field_t::String:
        case Field_t::Label:
          dst[F("value")] = field.getValue();
          break;

        case Field_t::Boolean:
        case Field_t::Number:
        case Field_t::Section:
        case Field_t::Select:
        case Field_t::UtcOffset:
          dst[F("value")] = field.getValue().toInt(); // TODO: fix double-conversion
          break;
        
        // intentionally no default ... causes compilation warning if new enum types added w/o updating here
      }
    }
    if ((field.type == Field_t::Number) || (field.type == Field_t::UtcOffset)) {
      dst[F("min")] = field.min;
      dst[F("max")] = field.max;
    }
    if (field.getOptions != nullptr) {
      JsonArray options = dst.createNestedArray(F("options"));
      field.getOptions(options);
    }
    return;
  }

  // passing array reference works fine, but need to make the function a template
  // to capture the array size... on the positive side, no need to pass `count` parameter
  template <size_t N>
  Field  getField(String name, const Field (&fields)[N]) {
    for (uint8_t i = 0; i < N; i++) {
      Field field = fields[i];
      if (field.name == name) {
        return field;
      }
    }
    return Field();
  }
  template <size_t N>
  String getFieldValue(String name, const Field (&fields)[N]) {
    Field field = getField(name, fields);
    if (field.getValue) {
      return field.getValue();
    }
    return String();
  }
  template <size_t N>
  String setFieldValue(String name, String value, const Field (&fields)[N]) {
    Field field = getField(name, fields);
    if (field.setValue) {
      return field.setValue(value);
    }
    return String();
  }

  template <size_t N>
  String getFieldsJson(const Field (&fields)[N]) {
    // As of 2021-11-23, https://arduinojson.org/v6/assistant/ reports minimum 8041 bytes required (for Fib256).
    // This depends upon three things:
    // 1. the number of fields
    // 2. the number of palettes (and their names)
    // 3. the number of patterns (and their names)
    DynamicJsonDocument jsonDoc(8192);
    JsonArray array = jsonDoc.to<JsonArray>(); // document is an array of fields
    for (const Field& field : fields) {
      array.add(field);
    }
    // that's all ... just serialize the result to String (or anything else...)
    String result;
    result.reserve(6*1024); // ~4939 for Fib256 ... pre-allocation is a performance optimization
    serializeJson(jsonDoc, result);
    return result;
  }

  // name, label, type, min, max, getValue, getOptions, setValue
  // only items that use the 'getOptions': patterns and palettes
  // only items that support 'setValue': options used for pridePlayground
  const Field fields[] = {
      {"name",                 "Name",                   Field_t::Label,     0,            0, getName,       nullptr,     nullptr},
      {"power",                "Power",                  Field_t::Boolean,   0,            1, getPower,      nullptr,     nullptr},
      {"brightness",           "Brightness",             Field_t::Number,    1,          255, getBrightness, nullptr,     nullptr},
      {"pattern",              "Pattern",                Field_t::Select,    0, patternCount, getPattern,    getPatterns, nullptr},
      {"palette",              "Palette",                Field_t::Select,    0, paletteCount, getPalette,    getPalettes, nullptr},
      {"speed",                "Speed",                  Field_t::Number,    1,          255, getSpeed,      nullptr,     nullptr},

      //--------------------------------------------------------------------------------------------------------
      {"autoplaySection",      "Autoplay",               Field_t::Section,   0,   0, nullptr,                 nullptr, nullptr},
      {"autoplay",             "Autoplay",               Field_t::Boolean,   0,   1, getAutoplay,             nullptr, nullptr},
      {"autoplayDuration",     "Autoplay Duration",      Field_t::Number,    0, 255, getAutoplayDuration,     nullptr, nullptr},

      //--------------------------------------------------------------------------------------------------------
      {"clock",                "Clock",                  Field_t::Section,   0,   0, nullptr,                 nullptr, nullptr},	
      {"showClock",            "Show Clock",             Field_t::Boolean,   0,   1, getShowClock,            nullptr, nullptr},	
      {"clockBackgroundFade",  "Background Fade",        Field_t::Number,    0, 255, getClockBackgroundFade,  nullptr, nullptr},
      {"utcOffsetIndex",       "UTC Offset",             Field_t::UtcOffset, 0, 104, getUtcOffsetIndex,       nullptr, setUtcOffsetIndexString},

      //--------------------------------------------------------------------------------------------------------
      {"solidColorSection",    "Solid Color",            Field_t::Section,  0,   0, nullptr,                 nullptr, nullptr},
      {"solidColor",           "Color",                  Field_t::Color,    0, 255, getSolidColor,           nullptr, nullptr},

      //--------------------------------------------------------------------------------------------------------
      {"fireSection",          "Fire & Water",           Field_t::Section,  0,   0, nullptr,                 nullptr, nullptr},
      {"cooling",              "Cooling",                Field_t::Number,   0, 255, getCooling,              nullptr, nullptr},
      {"sparking",             "Sparking",               Field_t::Number,   0, 255, getSparking,             nullptr, nullptr},

      //--------------------------------------------------------------------------------------------------------
      {"twinklesSection",      "Twinkles",               Field_t::Section,  0,   0, nullptr,                 nullptr, nullptr},
      {"twinkleSpeed",         "Twinkle Speed",          Field_t::Number,   0,   8, getTwinkleSpeed,         nullptr, nullptr},
      {"twinkleDensity",       "Twinkle Density",        Field_t::Number,   0,   8, getTwinkleDensity,       nullptr, nullptr},
      {"coolLikeIncandescent", "Incandescent Cool",      Field_t::Boolean,  0,   1, getCoolLikeIncandescent, nullptr, nullptr},

      //--------------------------------------------------------------------------------------------------------
      {"prideSection",         "Pride Playground",       Field_t::Section,  0,   0, nullptr,                 nullptr, nullptr             },
      {"saturationBpm",        "Saturation BPM",         Field_t::Number,   0, 255, getSaturationBpm,        nullptr, setSaturationBpm    },
      {"saturationMin",        "Saturation Min",         Field_t::Number,   0, 255, getSaturationMin,        nullptr, setSaturationMin    },
      {"saturationMax",        "Saturation Max",         Field_t::Number,   0, 255, getSaturationMax,        nullptr, setSaturationMax    },
      {"brightDepthBpm",       "Brightness Depth BPM",   Field_t::Number,   0, 255, getBrightDepthBpm,       nullptr, setBrightDepthBpm   },
      {"brightDepthMin",       "Brightness Depth Min",   Field_t::Number,   0, 255, getBrightDepthMin,       nullptr, setBrightDepthMin   },
      {"brightDepthMax",       "Brightness Depth Max",   Field_t::Number,   0, 255, getBrightDepthMax,       nullptr, setBrightDepthMax   },
      {"brightThetaIncBpm",    "Bright Theta Inc BPM",   Field_t::Number,   0, 255, getBrightThetaIncBpm,    nullptr, setBrightThetaIncBpm},
      {"brightThetaIncMin",    "Bright Theta Inc Min",   Field_t::Number,   0, 255, getBrightThetaIncMin,    nullptr, setBrightThetaIncMin},
      {"brightThetaIncMax",    "Bright Theta Inc Max",   Field_t::Number,   0, 255, getBrightThetaIncMax,    nullptr, setBrightThetaIncMax},
      {"msMultiplierBpm",      "Time Multiplier BPM",    Field_t::Number,   0, 255, getMsMultiplierBpm,      nullptr, setMsMultiplierBpm  },
      {"msMultiplierMin",      "Time Multiplier Min",    Field_t::Number,   0, 255, getMsMultiplierMin,      nullptr, setMsMultiplierMin  },
      {"msMultiplierMax",      "Time Multiplier Max",    Field_t::Number,   0, 255, getMsMultiplierMax,      nullptr, setMsMultiplierMax  },
      {"hueIncBpm",            "Hue Inc BPM",            Field_t::Number,   0, 255, getHueIncBpm,            nullptr, setHueIncBpm        },
      {"hueIncMin",            "Hue Inc Min",            Field_t::Number,   0, 255, getHueIncMin,            nullptr, setHueIncMin        },
      {"hueIncMax",            "Hue Inc Max",            Field_t::Number,   0, 255, getHueIncMax,            nullptr, setHueIncMax        },
      {"sHueBpm",              "S Hue BPM",              Field_t::Number,   0, 255, getSHueBpm,              nullptr, setSHueBpm          },
      {"sHueMin",              "S Hue Min",              Field_t::Number,   0, 255, getSHueMin,              nullptr, setSHueMin          },
      {"sHueMax",              "S Hue Max",              Field_t::Number,   0, 255, getSHueMax,              nullptr, setSHueMax          },
  };

  Field getField(String name) {
    return getField(name, fields);
  }

} // end inline anonymous namespace

// Everything else is exported from this file....

void setShowClock(uint8_t value)
{
  showClock = value == 0 ? 0 : 1;
  writeAndCommitSettings();
  broadcastInt("showClock", showClock);
}
void setClockBackgroundFade(uint8_t value)
{
  clockBackgroundFade = value;
  writeAndCommitSettings();
  broadcastInt("clockBackgroundFade", clockBackgroundFade);
}
uint8_t setUtcOffsetIndex(uint8_t value) {
  utcOffsetIndex = value;
  if (utcOffsetIndex > 104) utcOffsetIndex = 104; // 0 === -12 hours, 104 === +14 hours
  
  const int32_t UTC_OFFSET_MINIMUM_MINUTES = ((int32_t)-12) * 60; // corresponds to index 0
  const int32_t UTC_OFFSET_INCREMENT_MINUTES = 15; // each higher index increments by this amount
  
  // minutes above the minimum
  int32_t tmp = utcOffsetIndex * UTC_OFFSET_INCREMENT_MINUTES;
  // add that to the minimum value
  tmp = UTC_OFFSET_MINIMUM_MINUTES + tmp;
  // convert to seconds
  utcOffsetInSeconds = tmp * 60;

  Serial.print(F("utcOffsetIndex: ")); Serial.println(utcOffsetIndex);
  Serial.print(F("utcOffsetInSeconds: ")); Serial.println(utcOffsetInSeconds);
  timeClient.setTimeOffset(utcOffsetInSeconds);
  writeAndCommitSettings();
  return utcOffsetIndex;
}



// The following simplify getting JSON to allow dynamic interaction via webpage scripts
uint8_t power = 1;
uint8_t brightness = brightnessMap[brightnessIndex];

// getFieldsJson() lists all the options that the user can set, and is used by (at least)
// the built-in webserver to generate UI to adjust these options.
String getFieldsJson() {
  return getFieldsJson(fields);
}

// getFieldValue() is used to get a current value for the 
String getFieldValue(String name) {
  return getFieldValue(name, fields);
}
String setFieldValue(String name, String value) {
  return setFieldValue(name, value, fields);
}

