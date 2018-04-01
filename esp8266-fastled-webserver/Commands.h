/*
 * ESP8266 + FastLED + IR Remote + MSGEQ7: https://github.com/jasoncoon/esp8266-fastled-webserver
 * Copyright (C) 2015 Jason Coon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

enum class InputCommand {
  None,
  Up,
  Down,
  Left,
  Right,
  Select,
  Brightness,
  PlayMode,
  Palette,
  Power,
  BrightnessUp,
  BrightnessDown,

  Pattern1,
  Pattern2,
  Pattern3,
  Pattern4,
  Pattern5,
  Pattern6,
  Pattern7,
  Pattern8,
  Pattern9,
  Pattern10,
  Pattern11,
  Pattern12,

  RedUp,
  RedDown,
  GreenUp,
  GreenDown,
  BlueUp,
  BlueDown,

  Red,
  RedOrange,
  Orange,
  YellowOrange,
  Yellow,

  Green,
  Lime,
  Aqua,
  Teal,
  Navy,

  Blue,
  RoyalBlue,
  Purple,
  Indigo,
  Magenta,

  White,
  Pink,
  LightPink,
  BabyBlue,
  LightBlue,
};

// IR Raw Key Codes for SparkFun remote
#define IRCODE_SPARKFUN_POWER  0x10EFD827 // 284153895
#define IRCODE_SPARKFUN_A      0x10EFF807 //
#define IRCODE_SPARKFUN_B      0x10EF7887
#define IRCODE_SPARKFUN_C      0x10EF58A7
#define IRCODE_SPARKFUN_UP     0x10EFA05F // 284139615
#define IRCODE_SPARKFUN_LEFT   0x10EF10EF
#define IRCODE_SPARKFUN_SELECT 0x10EF20DF
#define IRCODE_SPARKFUN_RIGHT  0x10EF807F
#define IRCODE_SPARKFUN_DOWN   0x10EF00FF
#define IRCODE_SPARKFUN_HELD   0xFFFFFFFF

// IR Raw Key Codes for Adafruit remote
#define IRCODE_ADAFRUIT_HELD        0x7FFFFFFF // 4294967295
#define IRCODE_ADAFRUIT_VOLUME_UP   0x00FD40BF // 16597183
#define IRCODE_ADAFRUIT_PLAY_PAUSE  0x00FD807F // 16613503
#define IRCODE_ADAFRUIT_VOLUME_DOWN 0x00FD00FF // 16580863
#define IRCODE_ADAFRUIT_SETUP       0x00FD20DF // 16589023
#define IRCODE_ADAFRUIT_UP          0x00FDA05F // 16621663
#define IRCODE_ADAFRUIT_STOP_MODE   0x00FD609F // 16605343
#define IRCODE_ADAFRUIT_LEFT        0x00FD10EF // 16584943
#define IRCODE_ADAFRUIT_ENTER_SAVE  0x00FD906F // 16617583
#define IRCODE_ADAFRUIT_RIGHT       0x00FD50AF // 16601263
#define IRCODE_ADAFRUIT_0_10_PLUS   0x00FD30CF // 16593103
#define IRCODE_ADAFRUIT_DOWN        0x00FDB04F // 16625743
#define IRCODE_ADAFRUIT_BACK        0x00FD708F // 16609423
#define IRCODE_ADAFRUIT_1           0x00FD08F7 // 16582903
#define IRCODE_ADAFRUIT_2           0x00FD8877 // 16615543
#define IRCODE_ADAFRUIT_3           0x00FD48B7 // 16599223
#define IRCODE_ADAFRUIT_4           0x00FD28D7 // 16591063
#define IRCODE_ADAFRUIT_5           0x00FDA857 // 16623703
#define IRCODE_ADAFRUIT_6           0x00FD6897 // 16607383
#define IRCODE_ADAFRUIT_7           0x00FD18E7 // 16586983
#define IRCODE_ADAFRUIT_8           0x00FD9867 // 16619623
#define IRCODE_ADAFRUIT_9           0x00FD58A7 // 16603303

// IR Raw Key Codes for eTopxizu 44Key IR Remote Controller for 5050 3528 RGB LED Light Strip
#define IRCODE_ETOPXIZU_HELD            0x7FFFFFFF // 4294967295
#define IRCODE_ETOPXIZU_POWER           16712445
#define IRCODE_ETOPXIZU_PLAY_PAUSE      16745085
#define IRCODE_ETOPXIZU_BRIGHTNESS_UP   16726725
#define IRCODE_ETOPXIZU_BRIGHTNESS_DOWN 16759365

#define IRCODE_ETOPXIZU_DIY1            16724175
#define IRCODE_ETOPXIZU_DIY2            16756815
#define IRCODE_ETOPXIZU_DIY3            16740495
#define IRCODE_ETOPXIZU_DIY4            16716015
#define IRCODE_ETOPXIZU_DIY5            16748655
#define IRCODE_ETOPXIZU_DIY6            16732335

#define IRCODE_ETOPXIZU_JUMP3           16720095
#define IRCODE_ETOPXIZU_JUMP7           16752735
#define IRCODE_ETOPXIZU_FADE3           16736415
#define IRCODE_ETOPXIZU_FADE7           16769055
#define IRCODE_ETOPXIZU_FLASH           16764975
#define IRCODE_ETOPXIZU_AUTO            16773135

#define IRCODE_ETOPXIZU_QUICK           16771095
#define IRCODE_ETOPXIZU_SLOW            16762935

#define IRCODE_ETOPXIZU_RED_UP          16722135
#define IRCODE_ETOPXIZU_RED_DOWN        16713975

#define IRCODE_ETOPXIZU_GREEN_UP        16754775
#define IRCODE_ETOPXIZU_GREEN_DOWN      16746615

#define IRCODE_ETOPXIZU_BLUE_UP         16738455
#define IRCODE_ETOPXIZU_BLUE_DOWN       16730295

#define IRCODE_ETOPXIZU_RED             16718565
#define IRCODE_ETOPXIZU_RED_ORANGE      16722645
#define IRCODE_ETOPXIZU_ORANGE          16714485
#define IRCODE_ETOPXIZU_YELLOW_ORANGE   16726215
#define IRCODE_ETOPXIZU_YELLOW          16718055

#define IRCODE_ETOPXIZU_GREEN           16751205
#define IRCODE_ETOPXIZU_LIME            16755285
#define IRCODE_ETOPXIZU_AQUA            16747125
#define IRCODE_ETOPXIZU_TEAL            16758855
#define IRCODE_ETOPXIZU_NAVY            16750695

#define IRCODE_ETOPXIZU_BLUE            16753245
#define IRCODE_ETOPXIZU_ROYAL_BLUE      16749165
#define IRCODE_ETOPXIZU_PURPLE          16757325
#define IRCODE_ETOPXIZU_INDIGO          16742535
#define IRCODE_ETOPXIZU_MAGENTA         16734375

#define IRCODE_ETOPXIZU_WHITE           16720605
#define IRCODE_ETOPXIZU_PINK            16716525
#define IRCODE_ETOPXIZU_LIGHT_PINK      16724685
#define IRCODE_ETOPXIZU_BABY_BLUE       16775175
#define IRCODE_ETOPXIZU_LIGHT_BLUE      16767015

bool sparkfunRemoteEnabled = true;
bool adafruitRemoteEnabled = true;
bool etopxizuRemoteEnabled = true;

// Low level IR code reading function
// Function will return 0 if no IR code available
unsigned long decodeIRCode() {

  decode_results results;

  results.value = 0;

  // Attempt to read an IR code ?
  if (irReceiver.decode(&results)) {
    delay(20);

    if (results.value != 0)
      Serial.println(results.value);

    // Prepare to receive the next IR code
    irReceiver.resume();
  }

  return results.value;
}

// Read an IR code
// Function will return 0 if no IR code available
unsigned long readIRCode() {

  // Is there an IR code to read ?
  unsigned long code = decodeIRCode();
  if (code == 0) {
    // No code so return 0
    return 0;
  }

  // Keep reading until code changes
  while (decodeIRCode() == code) {
    ;
  }
  // Serial.println(code);
  return code;
}

unsigned long lastIrCode = 0;

unsigned int holdStartTime = 0;
unsigned int defaultHoldDelay = 500;
bool isHolding = false;

unsigned int zeroStartTime = 0;
unsigned int zeroDelay = 120;

unsigned long readIRCode(unsigned int holdDelay) {
  // read the raw code from the sensor
  unsigned long irCode = readIRCode();

  //Serial.print(millis());
  //Serial.print("\t");
  //Serial.println(irCode);

  // don't return a short click until we know it's not a long hold
  // we'll have to wait for holdDelay ms to pass before returning a non-zero IR code
  // then, after that delay, as long as the button is held, we can keep returning the code
  // every time until it's released

  // the ir remote only sends codes every 107 ms or so (avg 106.875, max 111, min 102),
  // so the ir sensor will return 0 even if a button is held
  // so we have to wait longer than that before returning a non-zero code
  // in order to detect that a button has been released and is no longer held

  // only reset after we've gotten 0 back for more than the ir remote send interval
  unsigned int zeroTime = 0;

  if (irCode == 0) {
    zeroTime = millis() - zeroStartTime;
    if (zeroTime >= zeroDelay && lastIrCode != 0) {
      //Serial.println(F("zero delay has elapsed, returning last ir code"));
      // the button has been released for longer than the zero delay
      // start over delays over and return the last code
      irCode = lastIrCode;
      lastIrCode = 0;
      return irCode;
    }

    return 0;
  }

  // reset the zero timer every time a non-zero code is read
  zeroStartTime = millis();

  unsigned int heldTime = 0;

  if (irCode == IRCODE_SPARKFUN_HELD || irCode == IRCODE_ADAFRUIT_HELD) {
    // has the hold delay passed?
    heldTime = millis() - holdStartTime;
    if (heldTime >= holdDelay) {
      isHolding = true;
      //Serial.println(F("hold delay has elapsed, returning last ir code"));
      return lastIrCode;
    }
    else if (holdStartTime == 0) {
      isHolding = false;
      holdStartTime = millis();
    }
  }
  else {
    // not zero, not IRCODE_SPARKFUN_HELD
    // store it for use later, until the hold and zero delays have elapsed
    holdStartTime = millis();
    isHolding = false;
    lastIrCode = irCode;
    return 0;
  }

  return 0;
}

void heldButtonHasBeenHandled() {
  lastIrCode = 0;
  isHolding = false;
  holdStartTime = 0;
}

unsigned long waitForIRCode() {

  unsigned long irCode = readIRCode();
  while ((irCode == 0) || (irCode == 0xFFFFFFFF)) {
    delay(200);
    irCode = readIRCode();
  }
  return irCode;
}

InputCommand getCommand(unsigned long input) {
  if (adafruitRemoteEnabled) {
    switch (input) {
      case IRCODE_ADAFRUIT_UP:
        return InputCommand::Up;

      case IRCODE_ADAFRUIT_DOWN:
        return InputCommand::Down;

      case IRCODE_ADAFRUIT_LEFT:
        return InputCommand::Left;

      case IRCODE_ADAFRUIT_RIGHT:
        return InputCommand::Right;

      case IRCODE_ADAFRUIT_ENTER_SAVE:
        return InputCommand::Select;

      case IRCODE_ADAFRUIT_STOP_MODE:
      case IRCODE_ADAFRUIT_1:
        return InputCommand::PlayMode;

      case IRCODE_ADAFRUIT_2:
        return InputCommand::Palette;

      case IRCODE_ADAFRUIT_PLAY_PAUSE:
        return InputCommand::Power;

      case IRCODE_ADAFRUIT_VOLUME_UP:
        return InputCommand::BrightnessUp;

      case IRCODE_ADAFRUIT_VOLUME_DOWN:
        return InputCommand::BrightnessDown;
    }
  }

  if (sparkfunRemoteEnabled) {
    switch (input) {
      case IRCODE_SPARKFUN_UP:
        return InputCommand::Up;

      case IRCODE_SPARKFUN_DOWN:
        return InputCommand::Down;

      case IRCODE_SPARKFUN_LEFT:
        return InputCommand::Left;

      case IRCODE_SPARKFUN_RIGHT:
        return InputCommand::Right;

      case IRCODE_SPARKFUN_SELECT:
        return InputCommand::Select;

      case IRCODE_SPARKFUN_POWER:
        return InputCommand::Brightness;

      case IRCODE_SPARKFUN_A:
        return InputCommand::PlayMode;

      case IRCODE_SPARKFUN_B:
        return InputCommand::Palette;
    }
  }

  if (etopxizuRemoteEnabled) {
    switch (input) {
      case IRCODE_ETOPXIZU_QUICK:
        return InputCommand::Up;

      case IRCODE_ETOPXIZU_SLOW:
        return InputCommand::Down;

      case IRCODE_ETOPXIZU_PLAY_PAUSE:
        return InputCommand::PlayMode;

      case IRCODE_ETOPXIZU_POWER:
        return InputCommand::Power;

      case IRCODE_ETOPXIZU_BRIGHTNESS_UP:
        return InputCommand::BrightnessUp;
      case IRCODE_ETOPXIZU_BRIGHTNESS_DOWN:
        return InputCommand::BrightnessDown;

      case IRCODE_ETOPXIZU_DIY1:
        return InputCommand::Pattern1;
      case IRCODE_ETOPXIZU_DIY2:
        return InputCommand::Pattern2;
      case IRCODE_ETOPXIZU_DIY3:
        return InputCommand::Pattern3;
      case IRCODE_ETOPXIZU_DIY4:
        return InputCommand::Pattern4;
      case IRCODE_ETOPXIZU_DIY5:
        return InputCommand::Pattern5;
      case IRCODE_ETOPXIZU_DIY6:
        return InputCommand::Pattern6;
      case IRCODE_ETOPXIZU_JUMP3:
        return InputCommand::Pattern7;
      case IRCODE_ETOPXIZU_JUMP7:
        return InputCommand::Pattern8;
      case IRCODE_ETOPXIZU_FADE3:
        return InputCommand::Pattern9;
      case IRCODE_ETOPXIZU_FADE7:
        return InputCommand::Pattern10;
      case IRCODE_ETOPXIZU_FLASH:
        return InputCommand::Pattern11;
      case IRCODE_ETOPXIZU_AUTO:
        return InputCommand::Pattern12;

      case IRCODE_ETOPXIZU_RED_UP:
        return InputCommand::RedUp;
      case IRCODE_ETOPXIZU_RED_DOWN:
        return InputCommand::RedDown;

      case IRCODE_ETOPXIZU_GREEN_UP:
        return InputCommand::GreenUp;
      case IRCODE_ETOPXIZU_GREEN_DOWN:
        return InputCommand::GreenDown;

      case IRCODE_ETOPXIZU_BLUE_UP:
        return InputCommand::BlueUp;
      case IRCODE_ETOPXIZU_BLUE_DOWN:
        return InputCommand::BlueDown;

      case IRCODE_ETOPXIZU_RED:
        return InputCommand::Red;
      case IRCODE_ETOPXIZU_RED_ORANGE:
        return InputCommand::RedOrange;
      case IRCODE_ETOPXIZU_ORANGE:
        return InputCommand::Orange;
      case IRCODE_ETOPXIZU_YELLOW_ORANGE:
        return InputCommand::YellowOrange;
      case IRCODE_ETOPXIZU_YELLOW:
        return InputCommand::Yellow;

      case IRCODE_ETOPXIZU_GREEN:
        return InputCommand::Green;
      case IRCODE_ETOPXIZU_LIME:
        return InputCommand::Lime;
      case IRCODE_ETOPXIZU_AQUA:
        return InputCommand::Aqua;
      case IRCODE_ETOPXIZU_TEAL:
        return InputCommand::Teal;
      case IRCODE_ETOPXIZU_NAVY:
        return InputCommand::Navy;

      case IRCODE_ETOPXIZU_BLUE:
        return InputCommand::Blue;
      case IRCODE_ETOPXIZU_ROYAL_BLUE:
        return InputCommand::RoyalBlue;
      case IRCODE_ETOPXIZU_PURPLE:
        return InputCommand::Purple;
      case IRCODE_ETOPXIZU_INDIGO:
        return InputCommand::Indigo;
      case IRCODE_ETOPXIZU_MAGENTA:
        return InputCommand::Magenta;

      case IRCODE_ETOPXIZU_WHITE:
        return InputCommand::White;
      case IRCODE_ETOPXIZU_PINK:
        return InputCommand::Pink;
      case IRCODE_ETOPXIZU_LIGHT_PINK:
        return InputCommand::LightPink;
      case IRCODE_ETOPXIZU_BABY_BLUE:
        return InputCommand::BabyBlue;
      case IRCODE_ETOPXIZU_LIGHT_BLUE:
        return InputCommand::LightBlue;
    }
  }

  return InputCommand::None;
}

InputCommand readCommand() {
  return getCommand(readIRCode());
}

InputCommand readCommand(unsigned int holdDelay) {
  return getCommand(readIRCode(holdDelay));
}
