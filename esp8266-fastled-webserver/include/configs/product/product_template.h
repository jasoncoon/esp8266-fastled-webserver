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

#error #if !defined(ESP8266_FASTLED_WEBSERVER_PRODUCT_xyzzy_H)
#error #define ESP8266_FASTLED_WEBSERVER_PRODUCT_xyzzy_H

// This template documents the settings to configure a specific "product"
// for building from this repository.

// The following are MANDATORY ... they must be defined to appropriate values
// This template provides the default values as used by the prior main branch

// LED_TYPE defines the control pattern for the LEDs
// See https://github.com/FastLED/FastLED/blob/b5874b588ade1d2639925e4e9719fa7d3c9d9e94/src/FastLED.h#L92-L119

#if !defined(LED_TYPE)
   #define LED_TYPE                      WS2811
#endif

// COLOR_ORDER defines the order that each pixel expects the data to arrive in
// See https://github.com/FastLED/FastLED/blob/765d4244889a692bb453cd4087af31e01c937035/src/pixeltypes.h#L852-L859
#if !defined(COLOR_ORDER)
   #define COLOR_ORDER                   RGB
#endif

// NUM_PIXELS  is the total number of pixels to be generated / output
#if !defined(NUM_PIXELS)
   #define NUM_PIXELS                    200
#endif
// AVAILABLE_MILLI_AMPS  is based on the power supply.  Recommended to use no more than 80% of actual rated maximum.
#if !defined(AVAILABLE_MILLI_AMPS)
   #error #define AVAILABLE_MILLI_AMPS          x2000x // IMPORTANT: set the max milli-Amps of your power supply (4A = 4000mA)
#endif

// MAX_MILLI_AMPS_PER_PIXEL  is used by FastLED to reduce change of exceeding total available milliamps.
#if !defined(MAX_MILLI_AMPS_PER_PIXEL)
   #define MAX_MILLI_AMPS_PER_PIXEL      60   // IMPORTANT: set to larger value if necessary
#endif

// FRAMES_PER_SECOND is a frame LIMIT per second.  This is approximate.
#if !defined(FRAMES_PER_SECOND)
   #define FRAMES_PER_SECOND             120
#endif

// DEFAULT_PATTERN_INDEX allows different products to default to different patterns
#if !defined(DEFAULT_PATTERN_INDEX)
   #define DEFAULT_PATTERN_INDEX         0
#endif

// DEFAULT_BRIGHTNESS_INDEX allows different products to default to different brightness (e.g., from brightness[5] array)
#if !defined(DEFAULT_BRIGHTNESS_INDEX)
   #define DEFAULT_BRIGHTNESS_INDEX      3
#endif

// DEFAULT_COLOR_CORRECTION defines which FastLED built-in color correction to apply
// Options include:
//     TypicalSMD5050     // 0xFFB0F0 == 255, 176, 240
//     TypicalLEDStrip    // 0xFFB0F0 == 255, 176, 240
//     Typical8mmPixel    // 0xFFE08C == 255, 224, 140
//     TypicalPixelString // 0xFFE08C == 255, 224, 140
//     UncorrectedColor   // 0xFFFFFF == 255, 255, 255
//
// Likely that you can encode any RGB value:
//     static_cast<LEDColorCorrection>(0xRRGGBB) 
//
// See https://github.com/FastLED/FastLED/blob/b5874b588ade1d2639925e4e9719fa7d3c9d9e94/src/color.h#L13-L32
#if !defined(DEFAULT_COLOR_CORRECTION)
   #define DEFAULT_COLOR_CORRECTION      TypicalLEDStrip
#endif


// NAME_PREFIX defines the product-specific prefix for auto-generated hostname
#if !defined(NAME_PREFIX)
   #define NAME_PREFIX                   "ESP8266-"
#endif

// PRODUCT_FRIENDLY_NAME is used in HTML page's title and navigation bar
// It is modified when js/app.js loads, by parsing json data generated from device
#if !defined(PRODUCT_FRIENDLY_NAME)
   #define PRODUCT_FRIENDLY_NAME         "ESP8266 + FastLED"
#endif

// IS_FIBONACCI is true when there are pre-defined mappings from physical pixel
// indices to Vogel order (which is also order of increasing radius),
// and the inverse mapping from Vogel order back to physical pixel index.
// IS_FIBONACCI also implies HAS_COORDINATE_MAP.
//
// When this is true, the following must be defined in map.h:
//
// const uint8_t physicalToFibonacci[NUM_PIXELS];
// const uint8_t fibonacciToPhysical[NUM_PIXELS];
//
// NOTE: physicalToFibonacci[] and fibonacciToPhysical[] are uint16_t when
//       NUM_PIXELS is greater than 256.
#if !defined(IS_FIBONACCI)
   #define IS_FIBONACCI                  1
#endif

// HAS_COORDINATE_MAP is true when there are mappings to define
// both the X,Y coordinates and the angles for each pixel.
//
// When this is true, the following must be defined in map.h:
//
// const uint8_t coordsX[NUM_PIXELS];
// const uint8_t coordsY[NUM_PIXELS];
// const uint8_t angles[NUM_PIXELS];
#if !defined(HAS_COORDINATE_MAP)
   #define HAS_COORDINATE_MAP            1
#endif

// HAS_POLAR_COORDS is true when there are mappings to define
// ... TODO: add definition for polar coordinates define ...
#if !defined(HAS_POLAR_COORDS)
   #define HAS_POLAR_COORDS              0
#endif


// PARALLEL_OUTPUT_CHANNELS indicates the number of independent channels
// that should be configured.  When this value == 1, DATA_PIN is used to
// control all pixels.
//
// When this value >= 2, this file must define:
//
// #define PIXELS_ON_DATA_PIN_1 wc
// #define PIXELS_ON_DATA_PIN_2 xc
// #define PIXELS_ON_DATA_PIN_3 yc
// ...
// #define PIXELS_ON_DATA_PIN_n zc
//
// *AND* the controller must support the required number of channels.
//
// The controller header file will define default values for the data pins,
// by defining the following symbols:
//
// #define DATA_PIN    w
// #define DATA_PIN_2  x
// #define DATA_PIN_3  y
// ...
// #define DATA_PIN_n  z
//
// These can be overridden from the board-default values by defining these
// values in the product header template.
//
// See Fibonacci512 and ESPTHING as examples of multi-channel outputs.
//
#if !defined(PARALLEL_OUTPUT_CHANNELS)
   #define PARALLEL_OUTPUT_CHANNELS   1
#endif

// // By default, no IR support is included.  Define ENABLE_IR as  to enable IR support.
// #define ENABLE_IR                  1

// // When ENABLE_IR is defined as 1, can also override controller-specific default:
// // #define IR_RECV_PIN  D99




#error #endif // ESP8266_FASTLED_WEBSERVER_PRODUCT_xyzz_H
