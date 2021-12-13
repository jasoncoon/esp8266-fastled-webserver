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

#include "common.h"

WiFiManager wifiManager;
ESP8266WebServer webServer(80);

ESP8266HTTPUpdateServer httpUpdateServer;

int utcOffsetInSeconds = -6 * 60 * 60;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds, NTP_UPDATE_THROTTLE_MILLLISECONDS);

String nameString;

CRGB leds[NUM_PIXELS];

const uint8_t brightnessCount = 5;
const uint8_t brightnessMap[brightnessCount] = { 16, 32, 64, 128, 255 };
uint8_t brightnessIndex = DEFAULT_BRIGHTNESS_INDEX;

// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
uint8_t secondsPerPalette = 10;

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
uint8_t cooling = 49;

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
uint8_t sparking = 60;

uint8_t speed = 30;

///////////////////////////////////////////////////////////////////////


uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

CRGBPalette16 IceColors_p = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);

uint8_t currentPatternIndex = DEFAULT_PATTERN_INDEX; // Index number of which pattern is current
uint8_t autoplay = 0;

uint8_t autoplayDuration = 10;
unsigned long autoPlayTimeout = 0;

uint8_t showClock = 0;
uint8_t clockBackgroundFade = 160;
uint8_t utcOffsetIndex = 24; // map(-6, -12, 14, 0, 104); -12 to 14 in 15 minute increments, mapped to 0 to 104

uint8_t currentPaletteIndex = 0;

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

CRGB solidColor = CRGB::Blue;

// scale the brightness of all pixels down
void dimAll(byte value)
{
  for (auto led : leds) {
    led.nscale8(value);
  }
}

// List of patterns to cycle through.  Each is defined as a separate function below.

// NOTE: HAS_POLAR_COORDS implies HAS_COORDINATE_MAP
//       IS_FIBONACCI     implies HAS_COORDINATE_MAP

const PatternAndName patterns[] = {
  { pride,                             "Pride" },
#if IS_FIBONACCI
  { prideFibonacci,                    "Pride Fibonacci" },
#endif

  { colorWaves,                        "Color Waves" },
#if IS_FIBONACCI
  { colorWavesFibonacci,               "Color Waves Fibonacci" },
#endif

  { pridePlayground,                   "Pride Playground" },
#if IS_FIBONACCI
  { pridePlaygroundFibonacci,          "Pride Playground Fibonacci" },
#endif

  { colorWavesPlayground,              "Color Waves Playground" },
#if IS_FIBONACCI
  { colorWavesPlaygroundFibonacci,     "Color Waves Playground Fibonacci" },
#endif

  { wheel,                             "Wheel" },

#if IS_FIBONACCI
  { swirlFibonacci,                    "Swirl Fibonacci"},
  { fireFibonacci,                     "Fire Fibonacci" },
  { waterFibonacci,                    "Water Fibonacci" },
  { emitterFibonacci,                  "Emitter Fibonacci" },

  { pacifica_loop,                     "Pacifica" },
  { pacifica_fibonacci_loop,           "Pacifica Fibonacci" },
#endif

#if HAS_COORDINATE_MAP // really a wrong name... and likely doing way more computation than necessary
  { radarSweepPalette,                 "Radar Sweep Palette" },
#endif

#if HAS_COORDINATE_MAP
  // matrix patterns
  { anglePalette,                      "Angle Palette" },
  { radiusPalette,                     "Radius Palette" },
  { xPalette,                          "X Axis Palette" },
  { yPalette,                          "Y Axis Palette" },
  { xyPalette,                         "XY Axis Palette" },

  { angleGradientPalette,              "Angle Gradient Palette" },
  { radiusGradientPalette,             "Radius Gradient Palette" },
  { xGradientPalette,                  "X Axis Gradient Palette" },
  { yGradientPalette,                  "Y Axis Gradient Palette" },
  { xyGradientPalette,                 "XY Axis Gradient Palette" },
#endif

#if HAS_POLAR_COORDS
  // noise patterns (Polar variations)
  { gradientPalettePolarNoise,         "Gradient Palette Polar Noise" },
  { palettePolarNoise,                 "Palette Polar Noise" },
  { firePolarNoise,                    "Fire Polar Noise" },
  { firePolarNoise2,                   "Fire Polar Noise 2" },
  { lavaPolarNoise,                    "Lava Polar Noise" },
  { rainbowPolarNoise,                 "Rainbow Polar Noise" },
  { rainbowStripePolarNoise,           "Rainbow Stripe Polar Noise" },
  { partyPolarNoise,                   "Party Polar Noise" },
  { forestPolarNoise,                  "Forest Polar Noise" },
  { cloudPolarNoise,                   "Cloud Polar Noise" },
  { oceanPolarNoise,                   "Ocean Polar Noise" },
  { blackAndWhitePolarNoise,           "Black & White Polar Noise" },
  { blackAndBluePolarNoise,            "Black & Blue Polar Noise" },
#endif

#if HAS_COORDINATE_MAP
  // noise patterns
  { fireNoise,                         "Fire Noise" },
  { fireNoise2,                        "Fire Noise 2" },
  { lavaNoise,                         "Lava Noise" },
  { rainbowNoise,                      "Rainbow Noise" },
  { rainbowStripeNoise,                "Rainbow Stripe Noise" },
  { partyNoise,                        "Party Noise" },
  { forestNoise,                       "Forest Noise" },
  { cloudNoise,                        "Cloud Noise" },
  { oceanNoise,                        "Ocean Noise" },
  { blackAndWhiteNoise,                "Black & White Noise" },
  { blackAndBlueNoise,                 "Black & Blue Noise" },
#endif

#if IS_FIBONACCI
  { drawAnalogClock,                   "Analog Clock" },

  { drawSpiralAnalogClock13,           "Spiral Analog Clock 13" },
  { drawSpiralAnalogClock21,           "Spiral Analog Clock 21" },
  { drawSpiralAnalogClock34,           "Spiral Analog Clock 34" },
  { drawSpiralAnalogClock55,           "Spiral Analog Clock 55" },
  { drawSpiralAnalogClock89,           "Spiral Analog Clock 89" },

  { drawSpiralAnalogClock21and34,      "Spiral Analog Clock 21 & 34"},
  { drawSpiralAnalogClock13_21_and_34, "Spiral Analog Clock 13, 21 & 34"},
  { drawSpiralAnalogClock34_21_and_13, "Spiral Analog Clock 34, 21 & 13"},
#endif

#if defined(PRODUCT_KRAKEN64)
  // Kraken patterns ... these use body[], which is also used as a proxy for radius...
  { radiusPalette,                     "Kraken Palette" },
  { radiusGradientPalette,             "Kraken Gradient Palette" },
#endif

  // twinkle patterns
  { currentPaletteTwinkles, "Palette Twinkles" },
  { rainbowTwinkles,        "Rainbow Twinkles" },
  { snowTwinkles,           "Snow Twinkles" },
  { cloudTwinkles,          "Cloud Twinkles" },
  { incandescentTwinkles,   "Incandescent Twinkles" },

  // TwinkleFOX patterns
  { retroC9Twinkles,        "Retro C9 Twinkles" },
  { redWhiteTwinkles,       "Red & White Twinkles" },
  { blueWhiteTwinkles,      "Blue & White Twinkles" },
  { redGreenWhiteTwinkles,  "Red, Green & White Twinkles" },
  { fairyLightTwinkles,     "Fairy Light Twinkles" },
  { snow2Twinkles,          "Snow 2 Twinkles" },
  { hollyTwinkles,          "Holly Twinkles" },
  { iceTwinkles,            "Ice Twinkles" },
  { partyTwinkles,          "Party Twinkles" },
  { forestTwinkles,         "Forest Twinkles" },
  { lavaTwinkles,           "Lava Twinkles" },
  { fireTwinkles,           "Fire Twinkles" },
  { cloud2Twinkles,         "Cloud 2 Twinkles" },
  { oceanTwinkles,          "Ocean Twinkles" },

  { coolerpalettebow,       "PaletteBow" },
  { palettebowglitter,      "PaletteBow With Glitter" },
  { palettebow,             "Solid PaletteBow" },
  { rainbow,                "Rainbow" },
  { rainbowWithGlitter,     "Rainbow With Glitter" },
  { rainbowSolid,           "Solid Rainbow" },
  { confetti,               "Confetti" },
  { sinelon,                "Sinelon" },
  { bpm,                    "Beat" },
  { juggle,                 "Juggle" },
  { fire,                   "Fire" },
  { water,                  "Water" },
  { firepal,                "PaletteFire" },
  { waterpal,               "PaletteWater" },

  { strandTest,             "Strand Test" },
#if (PARALLEL_OUTPUT_CHANNELS > 1)
  { multi_test,             "Multi Test" },
#endif

  { showSolidColor,         "Solid Color" } // This *must* be the last pattern
};

const uint8_t patternCount = ARRAY_SIZE2(patterns);

const CRGBPalette16 palettes[] = {
  RainbowColors_p,
  RainbowStripeColors_p,
  CloudColors_p,
  LavaColors_p,
  OceanColors_p,

  ForestColors_p,
  PartyColors_p,
  HeatColors_p,
};
const uint8_t paletteCount = ARRAY_SIZE2(palettes);

const String paletteNames[] = {
  "Rainbow",
  "Rainbow Stripe",
  "Cloud",
  "Lava",
  "Ocean",

  "Forest",
  "Party",
  "Heat",
};
const uint8_t paletteNameCount = ARRAY_SIZE2(paletteNames);
static_assert(paletteCount == paletteNameCount, "");


// TODO / BUGBUG -- should this be ESP8266-specific?  Is this only for when IR enabled ???
// FIB128 did not have this...
#if defined(PRODUCT_FIBONACCI256)
  ADC_MODE(ADC_VCC);
#endif

void setup() {
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  uint16_t milliAmps = (AVAILABLE_MILLI_AMPS < MAX_MILLI_AMPS) ? AVAILABLE_MILLI_AMPS : MAX_MILLI_AMPS;

  #if PARALLEL_OUTPUT_CHANNELS == 1
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_PIXELS);         // for WS2812 (Neopixel)
  #else
  #if PARALLEL_OUTPUT_CHANNELS >= 2
  FastLED.addLeds<LED_TYPE, DATA_PIN,   COLOR_ORDER>(leds, LedOffset<1>(), LedCount<1>());
  FastLED.addLeds<LED_TYPE, DATA_PIN_2, COLOR_ORDER>(leds, LedOffset<2>(), LedCount<2>());
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 3
  FastLED.addLeds<LED_TYPE, DATA_PIN_3, COLOR_ORDER>(leds, LedOffset<3>(), LedCount<3>());
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 4
  FastLED.addLeds<LED_TYPE, DATA_PIN_4, COLOR_ORDER>(leds, LedOffset<4>(), LedCount<4>());
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 5
  FastLED.addLeds<LED_TYPE, DATA_PIN_5, COLOR_ORDER>(leds, LedOffset<5>(), LedCount<4>());
  #endif
  #if PARALLEL_OUTPUT_CHANNELS >= 6
  FastLED.addLeds<LED_TYPE, DATA_PIN_6, COLOR_ORDER>(leds, LedOffset<6>(), LedCount<4>());
  #endif
  #endif // PARALLEL_OUTPUT_CHANNELS

  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_PIXELS); // for APA102 (Dotstar)

  FastLED.setDither(false);
  FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, milliAmps);
  fill_solid(leds, NUM_PIXELS, CRGB::Black);
  FastLED.show();

  EEPROM.begin(512); // TODO: move settings (currently EEPROM) to fields.hpp/.cpp
  readSettings();

  FastLED.setBrightness(brightness);

#if ENABLE_IR
  InitializeIR();
#endif // ENABLE_IR

  Serial.println();
  Serial.println(F("System Info:"));
  Serial.print( F("Max mA: ") ); Serial.println(milliAmps);
  Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  Serial.print( F("Boot Vers: ") ); Serial.println(system_get_boot_version());
  Serial.print( F("CPU: ") ); Serial.println(system_get_cpu_freq());
  Serial.print( F("SDK: ") ); Serial.println(system_get_sdk_version());
  Serial.print( F("Chip ID: ") ); Serial.println(system_get_chip_id());
  Serial.print( F("Flash ID: ") ); Serial.println(spi_flash_get_id());
  Serial.print( F("Flash Size: ") ); Serial.println(ESP.getFlashChipRealSize());
  Serial.print( F("Vcc: ") ); Serial.println(ESP.getVcc());
  Serial.print( F("MAC Address: ") ); Serial.println(WiFi.macAddress());
  Serial.println();

  Serial.println(F("Settings: "));
  Serial.print(F("brightness: ")); Serial.println(brightness);
  Serial.print(F("currentPatternIndex: ")); Serial.println(currentPatternIndex);
  Serial.print(F("solidColor.r: ")); Serial.println(solidColor.r);
  Serial.print(F("solidColor.g: ")); Serial.println(solidColor.g);
  Serial.print(F("solidColor.b: ")); Serial.println(solidColor.b);
  Serial.print(F("power: ")); Serial.println(power);
  Serial.print(F("autoplay: ")); Serial.println(autoplay);
  Serial.print(F("autoplayDuration: ")); Serial.println(autoplayDuration);
  Serial.print(F("currentPaletteIndex: ")); Serial.println(currentPaletteIndex);
  Serial.print(F("showClock: ")); Serial.println(showClock);
  Serial.print(F("clockBackgroundFade: ")); Serial.println(clockBackgroundFade);
  Serial.print(F("utcOffsetIndex: ")); Serial.println(utcOffsetIndex);
  Serial.print(F("utcOffsetInSeconds: ")); Serial.println(utcOffsetInSeconds);
  Serial.println();


  if (!MYFS.begin()) {
    Serial.println(F("An error occurred when attempting to mount the flash file system"));
  } else {
    Serial.println("FS contents:");

    Dir dir = MYFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), String(fileSize).c_str());
    }
    Serial.printf("\n");
  }


  // Do a little work to get a unique-ish name. Get the
  // last two bytes of the MAC (HEX'd)":

  // copy the mac address to a byte array
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);

  // format the last two digits to hex character array, like 0A0B
  char macID[5];
  sprintf(macID, "%02X%02X", mac[WL_MAC_ADDR_LENGTH - 2], mac[WL_MAC_ADDR_LENGTH - 1]);

  // convert the character array to a string
  String macIdString = macID;
  macIdString.toUpperCase();

  nameString = NAME_PREFIX + macIdString;

  // Allocation of variable-sized arrays on the stack is a GCC extension.
  // Converting this to be compile-time evaluated is possible:
  //     nameString.length() === strlen(NAME_PREFIX) + strlen(maxIdString)
  //     strlen(NAME_PREFIX) is compile-time constexpr (but changes per NAME_PREFIX)
  //     strlen(macIdString) is always 4
  // Therefore, can use the following to ensure statically evaluated at compile-time,
  // and avoid use of GCC extensions, with no performance loss.
  const size_t nameCharCount = static_eval<size_t, constexpr_strlen(NAME_PREFIX) + 4>::value;
  const size_t nameBufferSize = static_eval<size_t, nameCharCount+1>::value;
  char nameChar[nameBufferSize];
  memset(nameChar, 0, nameBufferSize);
  // Technically, this should *NEVER* need to check the nameString length.
  // However, I prefer to code defensively, since no static_assert() can detect this.
  size_t loopUntil = (nameCharCount <= nameString.length() ? nameCharCount : nameString.length());
  for (size_t i = 0; i < loopUntil; i++) {
    nameChar[i] = nameString.charAt(i);
  }

  Serial.printf("Name: %s\n", nameChar );

  // reset settings - wipe credentials for testing
  // wifiManager.resetSettings();

  wifiManager.setConfigPortalBlocking(false);

  //automatically connect using saved credentials if they exist
  //If connection fails it starts an access point with the specified name
  if (wifiManager.autoConnect(nameChar)) {
    Serial.println("Wi-Fi connected");
  }
  else {
    Serial.println("Wi-Fi manager portal running");
  }

  httpUpdateServer.setup(&webServer);

  webServer.on("/all", HTTP_GET, []() {
    String json = getFieldsJson();
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", json);
  });
  
  webServer.on("/product", HTTP_GET, []() {
    String json = "{\"productName\":\"" PRODUCT_FRIENDLY_NAME "\"}";
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", json);
  });

  webServer.on("/info", HTTP_GET, []() {
    String json = getInfoJson();
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "application/json", json);
  });

  webServer.on("/fieldValue", HTTP_GET, []() {
    String name = webServer.arg("name");
    String value = getFieldValue(name);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "text/json", value);
  });

  webServer.on("/fieldValue", HTTP_POST, []() {
    String name = webServer.arg("name");
    String value = webServer.arg("value");
    String newValue = setFieldValue(name, value);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "text/json", newValue);
  });

  webServer.on("/power", HTTP_POST, []() {
    String value = webServer.arg("value");
    setPower(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(power);
  });

  webServer.on("/cooling", HTTP_POST, []() {
    String value = webServer.arg("value");
    cooling = value.toInt();
    broadcastInt("cooling", cooling);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(cooling);
  });

  webServer.on("/sparking", HTTP_POST, []() {
    String value = webServer.arg("value");
    sparking = value.toInt();
    broadcastInt("sparking", sparking);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(sparking);
  });

  webServer.on("/speed", HTTP_POST, []() {
    String value = webServer.arg("value");
    speed = value.toInt();
    broadcastInt("speed", speed);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(speed);
  });

  webServer.on("/twinkleSpeed", HTTP_POST, []() {
    String value = webServer.arg("value");
    long tmp = value.toInt();
    if (tmp < 0) {
      tmp = 0;
    } else if (tmp > 8) {
      tmp = 8;
    }
    twinkleSpeed = (uint8_t)tmp;
    writeAndCommitSettings();
    broadcastInt("twinkleSpeed", twinkleSpeed);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(twinkleSpeed);
  });

  webServer.on("/twinkleDensity", HTTP_POST, []() {
    String value = webServer.arg("value");
    long tmp = value.toInt();
    if (tmp < 0) {
      tmp = 0;
    } else if (tmp > 8) {
      tmp = 8;
    }
    twinkleDensity = tmp;
    writeAndCommitSettings();
    broadcastInt("twinkleDensity", twinkleDensity);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(twinkleDensity);
  });

  webServer.on("/coolLikeIncandescent", HTTP_POST, []() {
    String value = webServer.arg("value");
    long tmp = value.toInt();
    if (tmp < 0) {
      tmp = 0;
    } else if (tmp > 1) {
      tmp = 1;
    }
    coolLikeIncandescent = tmp;
    writeAndCommitSettings();
    broadcastInt("coolLikeIncandescent", coolLikeIncandescent);
    sendInt(coolLikeIncandescent);
  });

  webServer.on("/solidColor", HTTP_POST, []() {
    String r = webServer.arg("r");
    String g = webServer.arg("g");
    String b = webServer.arg("b");
    setSolidColor(r.toInt(), g.toInt(), b.toInt());
    sendString(String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b));
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
  });

  webServer.on("/pattern", HTTP_POST, []() {
    String value = webServer.arg("value");
    setPattern(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(currentPatternIndex);
  });

  webServer.on("/patternName", HTTP_POST, []() {
    String value = webServer.arg("value");
    setPatternName(value);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(currentPatternIndex);
  });

  webServer.on("/palette", HTTP_POST, []() {
    String value = webServer.arg("value");
    setPalette(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(currentPaletteIndex);
  });

  webServer.on("/paletteName", HTTP_POST, []() {
    String value = webServer.arg("value");
    setPaletteName(value);
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(currentPaletteIndex);
  });

  webServer.on("/brightness", HTTP_POST, []() {
    String value = webServer.arg("value");
    setBrightness(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(brightness);
  });

  webServer.on("/autoplay", HTTP_POST, []() {
    String value = webServer.arg("value");
    setAutoplay(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(autoplay);
  });

  webServer.on("/autoplayDuration", HTTP_POST, []() {
    String value = webServer.arg("value");
    setAutoplayDuration(value.toInt());
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    sendInt(autoplayDuration);
  });

  webServer.on("/showClock", HTTP_POST, []() {
    String value = webServer.arg("value");
    long tmp = value.toInt();
    if (tmp < 0) {
      tmp = 0;
    } else if (tmp > 1) {
      tmp = 1;
    }
    setShowClock(tmp);
    sendInt(showClock);
  });

  webServer.on("/clockBackgroundFade", HTTP_POST, []() {
    String value = webServer.arg("value");
    long tmp = value.toInt();
    if (tmp < 0) {
      tmp = 0;
    } else if (tmp > 255) {
      tmp = 255;
    }
    setClockBackgroundFade(tmp);
    sendInt(clockBackgroundFade);
  });

  //list directory
  webServer.on("/list", HTTP_GET, handleFileList);
  //load editor
  webServer.on("/edit", HTTP_GET, []() {
    if (!handleFileRead("/edit.htm")) webServer.send(404, "text/plain", "FileNotFound");
  });
  //create file
  webServer.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  webServer.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  webServer.on("/edit", HTTP_POST, []() {
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.send(200, "text/plain", "");
  }, handleFileUpload);

  webServer.serveStatic("/", MYFS, "/", "max-age=86400");

  MDNS.begin(nameChar);
  MDNS.setHostname(nameChar);

  webServer.begin();
  Serial.println("HTTP web server started");

#if ENABLE_WEBSOCKETS
  InitializeWebSocketServer();
#endif // ENABLE_WEBSOCKETS

  autoPlayTimeout = millis() + (autoplayDuration * 1000);
#if ENABLE_ARDUINO_OTA
  ArduinoOTA.begin();
#endif
  timeClient.begin();
}

void sendInt(uint8_t value)
{
  sendString(String(value));
}

void sendString(String value)
{
  webServer.send(200, "text/plain", value);
}

// TODO: Add board-specific entropy sources
// e.g., using `uint32_t esp_random()`, if exposed in Arduino ESP32 / ESP8266 BSPs
// e.g., directly reading from 0x3FF20E44 on ESP8266 (dangerous! no entropy validation, whitening)
// e.g., directly reading from 0x3FF75144 on ESP32   (dangerous! no entropy validation, whitening)
// e.g., directly reading from RANDOM_REG32          (dangerous! no entropy validation, whitening)
// e.g., using a library, such as https://github.com/marvinroger/ESP8266TrueRandom/blob/master/ESP8266TrueRandom.cpp (less dangerous?)
// e.g., directly reading REG_READ(WDEV_RND_REG)     (dangerous! no check for sufficient clock cycles passed for entropy)
void loop() {
  // Modify random number generator seed; we use a lot of it.  (Note: this is still deterministic)
  random16_add_entropy(random(65535));

  //webSocketsServer.loop();

  wifiManager.process();
  webServer.handleClient();
  MDNS.update();

  static bool hasConnected = false;

  EVERY_N_SECONDS(1) {
    if (WiFi.status() != WL_CONNECTED) {
      //      Serial.printf("Connecting to %s\n", ssid);
      hasConnected = false;
    }
    else if (!hasConnected) {
      hasConnected = true;
      MDNS.begin(nameString);
      MDNS.setHostname(nameString);
      webServer.begin();
      Serial.println("HTTP web server started");
      Serial.print("Connected! Open http://");
      Serial.print(WiFi.localIP());
      Serial.print(" or http://");
      Serial.print(nameString);
      Serial.println(".local in your browser");
    } else {
      timeClient.update(); // NTPClient has throttling built-in
    }
  }

  checkPingTimer();
#if ENABLE_IR
  handleIrInput();  // empty function when ENABLE_IR is not defined
#endif // ENABLE_IR

  if (power == 0) {
    fill_solid(leds, NUM_PIXELS, CRGB::Black);
    FastLED.delay(1000 / FRAMES_PER_SECOND); // this function calls FastLED.show() at least once
    return;
  }

  // EVERY_N_SECONDS(10) {
  //   Serial.print( F("Heap: ") ); Serial.println(system_get_free_heap_size());
  // }

  // change to a new cpt-city gradient palette
  EVERY_N_SECONDS( secondsPerPalette ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(40) {
    // slowly blend the current palette to the next
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 8);
    gHue++;  // slowly cycle the "base color" through the rainbow
  }

  if (autoplay && (millis() > autoPlayTimeout)) {
    adjustPattern(true);
    autoPlayTimeout = millis() + (autoplayDuration * 1000);
  }

  // Call the current pattern function once, updating the 'leds' array
  patterns[currentPatternIndex].pattern();

  #if IS_FIBONACCI
  if (showClock) drawAnalogClock();
  #endif

  // insert a delay to keep the framerate modest ... this is guaranteed to call FastLED.show() at least once
  FastLED.delay(1000 / FRAMES_PER_SECOND);
#if ENABLE_ARDUINO_OTA
  ArduinoOTA.handle();
#endif

}

// TODO: Save settings in file system, not EEPROM!
const uint8_t SETTINGS_MAGIC_BYTE = 0x96;
void readSettings()
{
  // check for "magic number" so we know settings have been written to EEPROM
  // and it's not just full of random bytes

  if (EEPROM.read(511) != SETTINGS_MAGIC_BYTE) {
    return;
  }

  brightness = EEPROM.read(0);

  currentPatternIndex = EEPROM.read(1);
  if (currentPatternIndex >= patternCount) {
    currentPatternIndex = patternCount - 1;
  }

  byte r = EEPROM.read(2);
  byte g = EEPROM.read(3);
  byte b = EEPROM.read(4);

  if (r == 0 && g == 0 && b == 0)
  {
  }
  else
  {
    solidColor = CRGB(r, g, b);
  }

  power = EEPROM.read(5);

  autoplay = EEPROM.read(6);
  autoplayDuration = EEPROM.read(7);

  currentPaletteIndex = EEPROM.read(8);
  if (currentPaletteIndex >= paletteCount) {
    currentPaletteIndex = paletteCount - 1;
  }

  twinkleSpeed = EEPROM.read(9);
  twinkleDensity = EEPROM.read(10);

  cooling = EEPROM.read(11);
  sparking = EEPROM.read(12);

  coolLikeIncandescent = EEPROM.read(13);

  showClock = EEPROM.read(14);
  clockBackgroundFade = EEPROM.read(15);
  utcOffsetIndex = EEPROM.read(16);
  setUtcOffsetIndex(utcOffsetIndex);
}

void writeAndCommitSettings() {
  EEPROM.write(0, brightness);
  EEPROM.write(1, currentPatternIndex);
  EEPROM.write(2, solidColor.r);
  EEPROM.write(3, solidColor.g);
  EEPROM.write(4, solidColor.b);
  EEPROM.write(5, power);
  EEPROM.write(6, autoplay);
  EEPROM.write(7, autoplayDuration);
  EEPROM.write(8, currentPaletteIndex);
  EEPROM.write(9, twinkleSpeed);
  EEPROM.write(10, twinkleDensity);
  EEPROM.write(11, cooling);
  EEPROM.write(12, sparking);
  EEPROM.write(13, coolLikeIncandescent);
  EEPROM.write(14, showClock);
  EEPROM.write(15, clockBackgroundFade);
  EEPROM.write(16, utcOffsetIndex);
  EEPROM.write(511, SETTINGS_MAGIC_BYTE);
  EEPROM.commit();
}

void setPower(uint8_t value)
{
  power = value == 0 ? 0 : 1;
  writeAndCommitSettings();
  broadcastInt("power", value);
}
void setAutoplay(uint8_t value)
{
  autoplay = value == 0 ? 0 : 1;
  writeAndCommitSettings();
  broadcastInt("autoplay", autoplay);
}

void setAutoplayDuration(uint8_t value)
{
  autoplayDuration = value;
  writeAndCommitSettings();
  autoPlayTimeout = millis() + (autoplayDuration * 1000);

  broadcastInt("autoplayDuration", autoplayDuration);
}

void setSolidColor(CRGB color)
{
  setSolidColor(color.r, color.g, color.b);
}

void setSolidColor(uint8_t r, uint8_t g, uint8_t b)
{
  solidColor = CRGB(r, g, b);
  writeAndCommitSettings();
  setPattern(patternCount - 1);

  broadcastString("color", String(solidColor.r) + "," + String(solidColor.g) + "," + String(solidColor.b));
}

// increase or decrease the current pattern number, and wrap around at the ends
void adjustPattern(bool up)
{
  if (up)
    currentPatternIndex++;
  else
    currentPatternIndex--;

  // wrap around at the end
  if (currentPatternIndex >= patternCount) {
    currentPatternIndex = 0;
  }

  if (autoplay == 0) {
    writeAndCommitSettings();
  }

  broadcastInt("pattern", currentPatternIndex);
}

void setPattern(uint8_t value)
{
  if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

  if (autoplay == 0) {
    writeAndCommitSettings();
  }

  broadcastInt("pattern", currentPatternIndex);
}

void setPatternName(String name)
{
  for (uint8_t i = 0; i < patternCount; i++) {
    if (patterns[i].name == name) {
      setPattern(i);
      break;
    }
  }
}

void setPalette(uint8_t value)
{
  if (value >= paletteCount)
    value = paletteCount - 1;

  currentPaletteIndex = value;
  writeAndCommitSettings();
  broadcastInt("palette", currentPaletteIndex);
}

void setPaletteName(String name)
{
  for (uint8_t i = 0; i < paletteCount; i++) {
    if (paletteNames[i] == name) {
      setPalette(i);
      break;
    }
  }
}

void adjustBrightness(bool up)
{
  if (up && brightnessIndex < brightnessCount - 1)
    brightnessIndex++;
  else if (!up && brightnessIndex > 0)
    brightnessIndex--;

  brightness = brightnessMap[brightnessIndex];

  FastLED.setBrightness(brightness);
  writeAndCommitSettings();
  broadcastInt("brightness", brightness);
}

void setBrightness(uint8_t value)
{
  brightness = value;

  FastLED.setBrightness(brightness);
  writeAndCommitSettings();
  broadcastInt("brightness", brightness);
}

void strandTest()
{
  static size_t i = 0;

  EVERY_N_SECONDS(1)
  {
    i++;
    if (i >= NUM_PIXELS)
      i = 0;
  }

  fill_solid(leds, NUM_PIXELS, CRGB::Black);

  leds[i] = solidColor;
}

void showSolidColor()
{
  fill_solid(leds, NUM_PIXELS, solidColor);
}

// Patterns from FastLED example DemoReel100: https://github.com/FastLED/FastLED/blob/master/examples/DemoReel100/DemoReel100.ino

void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_PIXELS, gHue, 255 / NUM_PIXELS);
}

void palettebow()
{
  fill_solid(leds, NUM_PIXELS, ColorFromPalette(palettes[currentPaletteIndex], gHue, 255));
}

void coolerpalettebow()
{
  static uint8_t paletteStartIndex = 0;
  // Why is it safe to use a `paletteStartIndex` over 16?
  // The palettes array is defined as an array of `CRGBPalette16`,
  // which means there are exactly 16 CRGB elements. Therefore,
  // the only valid indices are [0..15].
  //
  // fill_palette<CRGBPalette16>() calls ColorFromPalette<CRGBPalette16>().
  // Manual review shows that *each* of the ColorFromPalette<>() functions safely
  // accept any `uint8_t` value for the start index.  Therefore, there's an
  // undocumented contract that the `uint8_t startIndex` provided to the function
  // can be any value, without causing out-of-bounds access of the palette array.
  paletteStartIndex--;
  fill_palette( leds, NUM_PIXELS, paletteStartIndex, (256 / NUM_PIXELS) + 1, palettes[currentPaletteIndex], 255, LINEARBLEND);
}

void palettebowglitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  palettebow();
  addGlitter(80);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void rainbowSolid()
{
  fill_solid(leds, NUM_PIXELS, CHSV(gHue, 255, 255));
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_PIXELS, 10);
  int pos = random16(NUM_PIXELS);
  // leds[pos] += CHSV( gHue + random8(64), 200, 255);
  leds[pos] += ColorFromPalette(palettes[currentPaletteIndex], gHue + random8(64));
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_PIXELS, 20);
  int pos = beatsin16(speed, 0, NUM_PIXELS);
  static int prevpos = 0;
  CRGB color = ColorFromPalette(palettes[currentPaletteIndex], gHue, 255);
  if ( pos < prevpos ) {
    fill_solid( leds + pos, (prevpos - pos) + 1, color);
  } else {
    fill_solid( leds + prevpos, (pos - prevpos) + 1, color);
  }
  prevpos = pos;
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t beat = beatsin8( speed, 64, 255);
  CRGBPalette16 palette = palettes[currentPaletteIndex];
  for ( int i = 0; i < NUM_PIXELS; i++) {
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle()
{
  static uint8_t    numdots =   4; // Number of dots in use.
  static uint8_t   faderate =   2; // How long should the trails be. Very low value = longer trails.
  static uint8_t     hueinc =  255 / numdots - 1; // Incremental change in hue between each dot.
  static uint8_t    thishue =   0; // Starting hue.
  static uint8_t     curhue =   0; // The current hue
  static uint8_t    thissat = 255; // Saturation of the colour.
  static uint8_t thisbright = 255; // How bright should the LED/display be.
  static uint8_t   basebeat =   5; // Higher = faster movement.

  static uint8_t lastSecond =  99;  // Static variable, means it's only defined once. This is our 'debounce' variable.
  uint8_t secondHand = (millis() / 1000) % 30; // IMPORTANT!!! Change '30' to a different value to change duration of the loop.

  if (lastSecond != secondHand) { // Debounce to make sure we're not repeating an assignment.
    lastSecond = secondHand;
    switch (secondHand) {
      case  0: numdots = 1; basebeat = 20; hueinc = 16; faderate = 2; thishue = 0; break; // You can change values here, one at a time , or altogether.
      case 10: numdots = 4; basebeat = 10; hueinc = 16; faderate = 8; thishue = 128; break;
      case 20: numdots = 8; basebeat =  3; hueinc =  0; faderate = 8; thishue = random8(); break; // Only gets called once, and not continuously for the next several seconds. Therefore, no rainbows.
      case 30: break;
    }
  }

  // Several colored dots, weaving in and out of sync with each other
  curhue = thishue; // Reset the hue values.
  fadeToBlackBy(leds, NUM_PIXELS, faderate);
  for ( int i = 0; i < numdots; i++) {
    //beat16 is a FastLED 3.1 function
    leds[beatsin16(basebeat + i + numdots, 0, NUM_PIXELS)] += CHSV(gHue + curhue, thissat, thisbright);
    curhue += hueinc;
  }
}

void fire()
{
  heatMap(HeatColors_p, true);
}

void water()
{
  heatMap(IceColors_p, false);
}

void firepal()
{
  heatMap(palettes[currentPaletteIndex], true);
}

void waterpal()
{
  heatMap(palettes[currentPaletteIndex], false);
}

// Pride2015 by Mark Kriegsman: https://gist.github.com/kriegsman/964de772d64c502760e5
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void fillWithPride(bool useFibonacciOrder)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < NUM_PIXELS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);

    uint16_t pixelnumber = i;

#if IS_FIBONACCI
    if (useFibonacciOrder) pixelnumber = fibonacciToPhysical[i];
#else
    (void)useFibonacciOrder; // unused parameter
#endif

    pixelnumber = (NUM_PIXELS - 1) - pixelnumber;

    nblend( leds[pixelnumber], newcolor, 64);
  }
}
void pride() {
  fillWithPride(false);
}
#if IS_FIBONACCI // prideFibonacci() uses fibonacciToPhysical
void prideFibonacci() {
  fillWithPride(true);
}
#endif


void fillRadialPaletteShift(bool useFibonacciOrder)
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    #if IS_FIBONACCI
      uint16_t idx = useFibonacciOrder ? fibonacciToPhysical[i] : i;
    #else
      (void)useFibonacciOrder;
      uint16_t idx = i;
    #endif
    leds[idx] = ColorFromPalette(gCurrentPalette, i + gHue, 255, LINEARBLEND);
  }
}
void fillRadialPaletteShiftOutward(bool useFibonacciOrder)
{
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    #if IS_FIBONACCI
      uint16_t idx = useFibonacciOrder ? fibonacciToPhysical[i] : i;
    #else
      (void)useFibonacciOrder;
      uint16_t idx = i;
    #endif
    leds[idx] = ColorFromPalette(gCurrentPalette, i - gHue, 255, LINEARBLEND);
  }
}
// TODO: define function radialPaletteShiftFibonacci()
void radialPaletteShift()
{
  #if IS_FIBONACCI
    fillRadialPaletteShift(true);
  #else
    fillRadialPaletteShift(false);
  #endif
}
// TODO: define function radialPaletteShiftOutwardFibonacci(), and update to call corresponding function
void radialPaletteShiftOutward()
{
  #if IS_FIBONACCI
    fillRadialPaletteShiftOutward(true);
  #else
    fillRadialPaletteShiftOutward(false);
  #endif
}


// based on FastLED example Fire2012WithPalette: https://github.com/FastLED/FastLED/blob/master/examples/Fire2012WithPalette/Fire2012WithPalette.ino
void heatMap(const CRGBPalette16& palette, bool up)
{
  fill_solid(leds, NUM_PIXELS, CRGB::Black);

  // Modify random number generator seed; we use a lot of it.  (Note: this is still deterministic)
  random16_add_entropy(random(256));

  // Array of temperature readings at each simulation cell
  static byte heat[NUM_PIXELS];

  byte colorindex;

  // Step 1.  Cool down every cell a little
  for ( uint16_t i = 0; i < NUM_PIXELS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((cooling * 10) / NUM_PIXELS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( uint16_t k = NUM_PIXELS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < sparking ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( uint16_t j = 0; j < NUM_PIXELS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    colorindex = scale8(heat[j], 190);

    CRGB color = ColorFromPalette(palette, colorindex);

    if (up) {
      leds[j] = color;
    }
    else {
      leds[(NUM_PIXELS - 1) - j] = color;
    }
  }
}

void addGlitter( uint8_t chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_PIXELS) ] += CRGB::White;
  }
}

///////////////////////////////////////////////////////////////////////

uint8_t beatsaw8( accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
                  uint32_t timebase = 0, uint8_t phase_offset = 0)
{
  uint8_t beat = beat8( beats_per_minute, timebase);
  uint8_t beatsaw = beat + phase_offset;
  uint8_t rangewidth = highest - lowest;
  uint8_t scaledbeat = scale8( beatsaw, rangewidth);
  uint8_t result = lowest + scaledbeat;
  return result;
}


// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void fillWithColorwaves( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& palette, bool useFibonacciOrder)
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  // uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for ( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
#if IS_FIBONACCI
    if (useFibonacciOrder) pixelnumber = fibonacciToPhysical[i];
#else
    (void)useFibonacciOrder;
#endif
    pixelnumber = (numleds - 1) - pixelnumber;

    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}

void colorWaves()
{
  fillWithColorwaves( leds, NUM_PIXELS, gCurrentPalette, false);
}
#if IS_FIBONACCI // colorWavesFibonacci() uses fibonacciToPhysical
void colorWavesFibonacci()
{
  fillWithColorwaves( leds, NUM_PIXELS, gCurrentPalette, true);
}
#endif

// Alternate rendering function just scrolls the current palette
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette) {
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_PIXELS) + 1, gCurrentPalette, 255, LINEARBLEND);
}

#if IS_FIBONACCI // swirlFibonacci() uses physicalToFibonacci and angles
void swirlFibonacci() {

  const float z = 2.5; // zoom (2.0)
  const float w = 3.0; // number of wings (3)
  const float p_min = 0.1; const float p_max = 2.0; // puff up (default: 1.0)
  const float d_min = 0.1; const float d_max = 2.0; // dent (default: 0.5)
  const float s_min = -3.0; const float s_max = 2.0; // swirl (default: -2.0)
  const float g_min = 0.1; const float g_max = 0.5; // glow (default: 0.2)
  const float b = 240; // inverse brightness (240)

  const float p = p_min + beatsin88(13*speed) / (float)UINT16_MAX * (p_max - p_min);
  const float d = d_min + beatsin88(17*speed) / (float)UINT16_MAX * (d_max - d_min);
  const float s = s_min + beatsin88(7*speed) / (float)UINT16_MAX * (s_max - s_min);
  const float g = g_min + beatsin88(27*speed) / (float)UINT16_MAX * (g_max - g_min);

  CRGBPalette16 palette( gGradientPalettes[1] ); // es_rivendell_15_gp

  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    float r = physicalToFibonacci[i] / 256.0 * z;
    float a = (angles[i] + (beat88(3*speed)>>3)) / 256.0 * TWO_PI;
    float v = r - p + d * sin(w * a + s * r * r);
    float c = 255 - b * pow(fabs(v), g);
    if (c < 0) c = 0;
    else if (c > 255) c = 255;

    leds[i] = ColorFromPalette(palette, (uint8_t)c);
  }
}
#endif

#if IS_FIBONACCI // fireFibonacci() uses coordsX/coordsY
// TODO: combine with normal fire effect
void fireFibonacci() {
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    uint8_t n = qsub8(inoise8((y << 2) - beat88(speed << 2), (x << 2)), y);

    leds[i] = ColorFromPalette(HeatColors_p, n);
  }
}
#endif

#if IS_FIBONACCI // waterFibonacci() uses coordsX/coordsY
// TODO: combine with normal water effect
void waterFibonacci() {
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint16_t x = coordsX[i];
    uint16_t y = coordsY[i];

    uint8_t n = inoise8((y << 2) + beat88(speed << 2), (x << 4));

    leds[i] = ColorFromPalette(IceColors_p, n);
  }
}
#endif

#if IS_FIBONACCI // emitterFibonacci() uses angle, antialiasPixelAR()
/**
 * Emits arcs of color spreading out from the center to the edge of the disc.
 */
void emitterFibonacci() {
  static CRGB ledBuffer[NUM_PIXELS]; // buffer for better fade behavior
  const uint8_t dAngle = 32; // angular span of the traces
  const uint8_t dRadius = 12; // radial width of the traces
  const uint8_t vSpeed = 16; // max speed variation


  static const uint8_t eCount = 7; // Number of simultanious traces
  static uint8_t angle[eCount]; // individual trace angles
  static uint16_t timeOffset[eCount]; // individual offsets from beat8() function
  static uint8_t speedOffset[eCount]; // individual speed offsets limited by vSpeed
  static uint8_t sparkIdx = 0; // randomizer cycles through traces to spark new ones

  // spark new trace
  EVERY_N_MILLIS(20) {
    if (random8(17) <= (speed >> 4)) { // increase change rate for higher speeds
      angle[sparkIdx] = random8();
      speedOffset[sparkIdx] = random8(vSpeed); // individual speed variation
      timeOffset[sparkIdx] = beat8(qadd8(speed,speedOffset[sparkIdx]));
      sparkIdx = addmod8(sparkIdx, 1, eCount); // continue randomizer at next spark
    }
  }

  // fade traces
  fadeToBlackBy( ledBuffer, NUM_PIXELS, 6 + (speed >> 3));

  // draw traces
  for (uint8_t e = 0; e < eCount; e++) {
    uint8_t startRadius = sub8(beat8(qadd8(speed, speedOffset[e])), timeOffset[e]);
    uint8_t endRadius = add8(startRadius, dRadius + (speed>>5)); // increase radial width for higher speeds
    antialiasPixelAR(angle[e], dAngle, startRadius, endRadius, ColorFromPalette(gCurrentPalette, startRadius), ledBuffer);
  }

  // copy buffer to actual strip
  memcpy(leds, ledBuffer, sizeof(ledBuffer[0])*NUM_PIXELS);
}
#endif

void wheel() {
  for (uint16_t i = 0; i < NUM_PIXELS; i++) {
    uint8_t j = beat8(speed);
    uint8_t hue = i + j;
    leds[i] = CHSV(hue, 255, 255);
  }
}


#if (PARALLEL_OUTPUT_CHANNELS > 1)

void multi_test() {
  static bool debug = true;
  const uint8_t step = (256 / PARALLEL_OUTPUT_CHANNELS);

  if (debug) {
    Serial.print("step: ");
    Serial.println(step);
  }

  static_assert(PARALLEL_OUTPUT_CHANNELS <= 6, "");
  for (uint8_t strip = 0; strip < PARALLEL_OUTPUT_CHANNELS; strip++) {
    uint16_t pixelOffset;
    uint16_t pixelCount;
    if (strip == 0) {
      pixelOffset = LedOffset<1>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<1>();  // uses one-based indices... sigh.
    }
#if (PARALLEL_OUTPUT_CHANNELS >= 2)
    else if (strip == 1) {
      pixelOffset = LedOffset<2>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<2>();  // uses one-based indices... sigh.
    }
#endif    
#if (PARALLEL_OUTPUT_CHANNELS >= 3)
    else if (strip == 2) {
      pixelOffset = LedOffset<3>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<3>();  // uses one-based indices... sigh.
    }
#endif
#if (PARALLEL_OUTPUT_CHANNELS >= 4)
    else if (strip == 3) {
      pixelOffset = LedOffset<4>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<4>();  // uses one-based indices... sigh.
    }
#endif
#if (PARALLEL_OUTPUT_CHANNELS > 5)
    else if (strip == 4) {
      pixelOffset = LedOffset<5>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<5>();  // uses one-based indices... sigh.
    }
#endif
#if (PARALLEL_OUTPUT_CHANNELS > 6)
    else if (strip == 5) {
      pixelOffset = LedOffset<6>(); // uses one-based indices... sigh.
      pixelCount  = LedCount<6>();  // uses one-based indices... sigh. 
    }
#endif
    else {
      break;
    }

    uint8_t hue = gHue + strip * step;
    CHSV c = CHSV(hue, 255, 255);
    
    if (debug) {
      Serial.print("hue: ");
      Serial.println(hue);
    }

    for (uint16_t i = 0; i < pixelCount; i++) {
      uint16_t j = i + pixelOffset;
      leds[j] = c;

      if (debug) {
        Serial.print("j: ");
        Serial.println(j);
      }
    }
  }

  debug = false;
}
#endif

