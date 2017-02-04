/*
   ESP8266 + FastLED + MQTT: https://github.com/jasoncoon/esp8266-fastled-webserver
   Copyright (C) 2015 Jason Coon

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


// TODO Flickering LED's ... https://github.com/FastLED/FastLED/issues/394 or https://github.com/FastLED/FastLED/issues/306
#define FASTLED_ALLOW_INTERRUPTS 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
#include "FastLED.h"
FASTLED_USING_NAMESPACE

extern "C" {
#include "user_interface.h"
}

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> // Only use it with SSL on MQTT Broker
#include <EEPROM.h>
#include <PubSubClient.h>
#include "GradientPalettes.h"
#include "Settings.h"

CRGB leds[NUM_LEDS];

uint8_t patternIndex = 0;

const uint8_t brightnessCount = 5;
uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
int brightnessIndex = 0;
uint8_t brightness = brightnessMap[brightnessIndex];

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
#define SECONDS_PER_PALETTE 10

///////////////////////////////////////////////////////////////////////

// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

uint8_t currentPatternIndex = 0; // Index number of which pattern is current
bool autoplayEnabled = false;

uint8_t autoPlayDurationSeconds = 10;
unsigned int autoPlayTimeout = 0;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

CRGB solidColor = CRGB::Black;

uint8_t power = 1;

// Mqtt Vars
WiFiClientSecure espClient;
PubSubClient client(espClient);


typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];
// List of patterns to cycle through.  Each is defined as a separate function below.

#include "PatternLogics.h"

PatternAndNameList patterns = {
  { colorwaves, "Color Waves" },
  { palettetest, "Palette Test" },
  { pride, "Pride" },
  { rainbow, "Rainbow" },
  { rainbowWithGlitter, "Rainbow With Glitter" },
  { confetti, "Confetti" },
  { sinelon, "Sinelon" },
  { juggle, "Juggle" },
  { bpm, "BPM" },
  { showSolidColor, "Solid Color" },
};
const uint8_t patternCount = ARRAY_SIZE(patterns);

#include "Inits.h"


void setup(void) {
  Serial.begin(115200);
  delay(100);
  //Serial.setDebugOutput(true);
  initFastLED();
  EEPROM.begin(512);
  loadSettings();
  logSys();

  initWlan();

  // Only to validate certs if u have problems ...
  // verifytls();

  //Mqtt Init
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);
}

// Format is: command:value
// value has to be a number, except rgb commands
void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  char tmp[length + 1];
  strncpy(tmp, (char*)payload, length);
  tmp[length] = '\0';
  String data(tmp);


  Serial.printf("Received Data from Topic: %s", data.c_str());
  Serial.println();
  if ( data.length() > 0) {

    if (data.startsWith("rgb(")) {
      data.replace("rgb(","");
      String r =  getValue(data, ',', 0);
      String g =  getValue(data, ',', 1);

      String b =  getValue(data, ',', 2);
      b.replace("hallo","");
      Serial.printf("Received R: %s G: %s B: %s", r.c_str(), g.c_str(), b.c_str());
      Serial.println();
      
      if (r.length() > 0 && g.length() > 0 && b.length() > 0) {
        setSolidColor(r.toInt(), g.toInt(), b.toInt());
      }
    }else {
      String command =  getValue(data, ':', 0);
      String value = getValue(data, ':', 1);
  
      if (command.length() > 0) {
  
        if (command.equals("power")) {
          if (isValidNumber(value)) {
            setPower(value.toInt());
          }
        } else if (command.equals("solidcolor")) {
          String r =  getValue(data, ':', 2);
          String g =  getValue(data, ':', 4);
          String b =  getValue(data, ':', 6);
          Serial.printf("Received R: %s G: %s B: %s", r.c_str(), g.c_str(), b.c_str());
          Serial.println();
          if (r.length() > 0 && g.length() > 0 && b.length() > 0) {
            setSolidColor(r.toInt(), g.toInt(), b.toInt());
          }
        } else if (command.equals("pattern")) {
          if (isValidNumber(value)) {
            setPattern(value.toInt());
          }
        } else if (command.equals("brightness")) {
          if (isValidNumber(value)) {
            setBrightness(value.toInt());
          }
        } else if (command.equals("brightnessAdjust")) {
          if (isValidNumber(value)) {
            adjustBrightness(value.toInt() == 0 ? false : true);
          }
        } else if (command.equals("patternAdjust")) {
          if (isValidNumber(value)) {
            adjustPattern(value.toInt() == 0 ? false : true);
          }
        }
      }
    }
    
   
  }
  Serial.println("Finished Topic Data ...");

}



void loop(void) {
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy(random(65535));
  if (!client.connected()) {
    reconnectMqtt();
  }
  client.loop();

  if (power == 0) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    FastLED.show();
    FastLED.delay(15);
    return;
  }

  // EVERY_N_SECONDS(10) {
  //   Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  // }

  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  // change to a new cpt-city gradient palette
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  // slowly blend the current cpt-city gradient palette to the next
  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }

  if (autoplayEnabled && millis() > autoPlayTimeout) {
    adjustPattern(true);
    autoPlayTimeout = millis() + (autoPlayDurationSeconds * 1000);
  }

  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternIndex].pattern();

  FastLED.show();

  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


void reconnectMqtt() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(mqtt_clientid, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setPower(uint8_t value)
{
  power = value == 0 ? 0 : 1;
}

void setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  solidColor = CRGB(r, g, b);

  EEPROM.write(2, r);
  EEPROM.write(3, g);
  EEPROM.write(4, b);

  setPattern(patternCount - 1);
}

// increase or decrease the current pattern number, and wrap around at theends
void adjustPattern(bool up)
{
  if (up)
    currentPatternIndex++;
  else
    currentPatternIndex--;

  // wrap around at the ends
  if (currentPatternIndex < 0)
    currentPatternIndex = patternCount - 1;
  if (currentPatternIndex >= patternCount)
    currentPatternIndex = 0;

  EEPROM.write(1, currentPatternIndex);
  EEPROM.commit();
}

void setPattern(int value)
{
  // don't wrap around at the ends
  if (value < 0)
    value = 0;
  else if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

  EEPROM.write(1, currentPatternIndex);
  EEPROM.commit();
}

// adjust the brightness, and wrap around at the ends
void adjustBrightness(bool up)
{
  if (up)
    brightnessIndex++;
  else
    brightnessIndex--;

  // wrap around at the ends
  if (brightnessIndex < 0)
    brightnessIndex = brightnessCount - 1;
  else if (brightnessIndex >= brightnessCount)
    brightnessIndex = 0;

  brightness = brightnessMap[brightnessIndex];

  FastLED.setBrightness(brightness);

  EEPROM.write(0, brightness);
  EEPROM.commit();
}

void setBrightness(int value)
{
  // don't wrap around at the ends
  if (value > 255)
    value = 255;
  else if (value < 0) value = 0;

  brightness = value;

  FastLED.setBrightness(brightness);

  EEPROM.write(0, brightness);
  EEPROM.commit();
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


boolean isValidNumber(String str) {
  // TODO replace with regex check
  bool result = false;
  for (byte i = 0; i < str.length(); i++)
  {
    if (isDigit(str.charAt(i))) {
      result = true;
    } else {
      result = false;
      break;
    }
  }
  return result;
}
