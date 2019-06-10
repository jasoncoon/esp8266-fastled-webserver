#ifndef _LED_CUSTOMIZATION_h
#define _LED_CUSTOMIZATION_h

#include "CustomPalettes.h"

// List of patterns to cycle through.  Each is defined as a separate function below.
PatternAndNameList patterns = {
  { pride,                  "Pride" },
  { colorWaves,             "Color Waves" },

  // twinkle patterns
  //{ rainbowTwinkles,        "Rainbow Twinkles" },
  { snowTwinkles,           "Snow Twinkles" },
  //{ cloudTwinkles,          "Cloud Twinkles" },
  //{ incandescentTwinkles,   "Incandescent Twinkles" },

  // TwinkleFOX patterns
  //{ ifusTwinkles,           "IfusTwinkles"},
  //{ retroC9Twinkles,        "Retro C9 Twinkles" },
  //{ redWhiteTwinkles,       "Red & White Twinkles" },
  //{ blueWhiteTwinkles,      "Blue & White Twinkles" },
  //{ redGreenWhiteTwinkles,  "Red, Green & White Twinkles" },
  //{ fairyLightTwinkles,     "Fairy Light Twinkles" },
  { snow2Twinkles,          "Snow 2 Twinkles" },
  //{ hollyTwinkles,          "Holly Twinkles" },
  { iceTwinkles,            "Ice Twinkles" },
  { partyTwinkles,          "Party Twinkles" },
  //{ forestTwinkles,         "Forest Twinkles" },
  //{ lavaTwinkles,           "Lava Twinkles" },
  //{ fireTwinkles,           "Fire Twinkles" },
  //{ cloud2Twinkles,         "Cloud 2 Twinkles" },
  { oceanTwinkles,          "Ocean Twinkles" },

  { rainbow,                "Rainbow" },
  { rainbowWithGlitter,     "Rainbow With Glitter" },
  { rainbowSolid,           "Solid Rainbow" },
  //{ confetti,               "Confetti" },
  { sinelon,                "Sinelon" },
  //{ bpm,                    "Beat" },
  //{ juggle,                 "Juggle" },
  //{ fire,                   "Fire" },
  //{ water,                  "Water" },

  { showSolidSparkling,     "Solid Color & Glitter" },
  { showSolidColor,         "Solid Color" },
};

// list of pallette entries
const CRGBPalette16 palettes[] = {
  RainbowColors_p,
  RainbowStripeColors_p,
  CloudColors_p,
  LavaColors_p,
  OceanColors_p,
  ForestColors_p,
  PartyColors_p,
  HeatColors_p,
  SolidRed_p,
  SolidGreen_p,
  SolidBlue_p,
  SolidWhite_p,
  SolidWarm_p,
};

// compiler will help ensure you have the same number of entries
const uint8_t paletteCount = ARRAY_SIZE(palettes);

// names of the palettes
const String paletteNames[paletteCount] = {
  "Rainbow",
  "Rainbow Stripe",
  "Cloud",
  "Lava",
  "Ocean",
  "Forest",
  "Party",
  "Heat",
  "Solid Red",
  "Solid Green",
  "Solid Blue",
  "Solid White",
  "Solid Warm",
};

#endif