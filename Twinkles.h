// based on ColorTwinkles by Mark Kriegsman: https://gist.github.com/kriegsman/5408ecd397744ba0393e

#define STARTING_BRIGHTNESS 64
#define FADE_IN_SPEED       32
#define FADE_OUT_SPEED      20
#define DENSITY            255

enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };

// Compact implementation of
// the directionFlags array, using just one BIT of RAM
// per pixel.  This requires a bunch of bit wrangling,
// but conserves precious RAM.  The cost is a few
// cycles and about 100 bytes of flash program memory.

uint8_t  directionFlags[NUM_STRIPS][MAX_LEDS_PER_STRIP]; 

CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter)
{
  CRGB incrementalColor = color;
  incrementalColor.nscale8( howMuchBrighter);
  return color + incrementalColor;
}

CRGB makeDarker( const CRGB& color, fract8 howMuchDarker)
{
  CRGB newcolor = color;
  newcolor.nscale8( 255 - howMuchDarker);
  return newcolor;
}

bool getPixelDirection( uint16_t stripIndex, uint16_t i)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  andMask = 1 << bitNum;
  return (directionFlags[stripIndex][index] & andMask) != 0;
}

void setPixelDirection( uint16_t stripIndex, uint16_t i, bool dir)
{
  uint16_t index = i / 8;
  uint8_t  bitNum = i & 0x07;

  uint8_t  orMask = 1 << bitNum;
  uint8_t andMask = 255 - orMask;
  uint8_t value = directionFlags[stripIndex][index] & andMask;
  if ( dir ) {
    value += orMask;
  }
  directionFlags[stripIndex][index] = value;
}

void brightenOrDarkenEachPixel(uint16_t stripIndex, fract8 fadeUpAmount, fract8 fadeDownAmount)
{
  for ( uint16_t i = 0; i < num_leds_list[stripIndex]; i++) {
    if ( getPixelDirection(stripIndex, i) == GETTING_DARKER) {
      // This pixel is getting darker
      leds[stripIndex][i] = makeDarker( leds[stripIndex][i], fadeDownAmount);
    } else {
      // This pixel is getting brighter
      leds[stripIndex][i] = makeBrighter( leds[stripIndex][i], fadeUpAmount);
      // now check to see if we've maxxed out the brightness
      if ( leds[stripIndex][i].r == 255 || leds[stripIndex][i].g == 255 || leds[stripIndex][i].b == 255) {
        // if so, turn around and start getting darker
        setPixelDirection(stripIndex, i, GETTING_DARKER);
      }
    }
  }
}

void init_directions() {
  for (int s = 0; s < NUM_STRIPS; s++) {
    directionFlags[s][(num_leds_list[s] + 7) / 8] = 0;
  }
}

void colortwinkles()
{
  init_directions();
  EVERY_N_MILLIS(30)
  {
    // Make each pixel brighter or darker, depending on
    // its 'direction' flag.
    for (int s = 0; s < NUM_STRIPS; s++) {
      brightenOrDarkenEachPixel(s, FADE_IN_SPEED, FADE_OUT_SPEED);
    
      // Now consider adding a new random twinkle
      if ( random8() < DENSITY ) {
        int pos = random16(num_leds_list[s]);
        if ( !leds[s][pos]) {
          leds[s][pos] = ColorFromPalette( gCurrentPalette, random8(), STARTING_BRIGHTNESS, NOBLEND);
          setPixelDirection(s, pos, GETTING_BRIGHTER);
        }
      }
    }
  }
}

void cloudTwinkles()
{
  gCurrentPalette = CloudColors_p; // Blues and whites!
  colortwinkles();
}

void rainbowTwinkles()
{
  gCurrentPalette = RainbowColors_p;
  colortwinkles();
}

void snowTwinkles()
{
  CRGB w(85, 85, 85), W(CRGB::White);

  gCurrentPalette = CRGBPalette16( W, W, W, W, w, w, w, w, w, w, w, w, w, w, w, w );
  colortwinkles();
}

void incandescentTwinkles()
{
  CRGB l(0xE1A024);

  gCurrentPalette = CRGBPalette16( l, l, l, l, l, l, l, l, l, l, l, l, l, l, l, l );
  colortwinkles();
}
