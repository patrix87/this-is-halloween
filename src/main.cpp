#include <Arduino.h>
#include <FastLED.h>

// Define the number of LEDs and the data pin
#define NUM_LEDS_B 44
#define NUM_LEDS_A 44
#define DATA_PIN_A 2
#define DATA_PIN_B 4
#define LED_TYPE WS2812B
CRGB leds_a[NUM_LEDS_A];
CRGB leds_b[NUM_LEDS_B];

#define FRAMES_PER_SECOND 120
#define GLITTER 32

// Original source: https://github.com/lowell80/HalloweenLights

// http://angrytools.com/gradient/?0_c87d02,20_000000,22_7A16A1,40_510D6E,54_052455,58_000000,100_c87d02,88_69853D,92_000000,34_7a16a1,34_7a16a1,14_B57002,19_000000,82_3A6E2D,65_438034,95_c87d02&0_100,100_100&l_321
DEFINE_GRADIENT_PALETTE( LowellFall_No6_gp ) {
    0,   135,  53,   1,
   35,   104,  41,   1,
   48,     0,   0,   0,
   51,     0,   0,   0,
   56,    37,   1,  80,
   86,    37,   1,  80,
  102,    12,   1,  31,
  137,     1,   3,  16,
  147,     0,   0,   0,
  165,     7,  55,   4,
  209,     5,  40,   3,
  224,    25,  60,   7,
  234,     0,   0,   0,
  242,   135,  53,   1,
  255,   135,  53,   1
};


CRGBPalette16 palette = LowellFall_No6_gp;

void setup() {
  // put your setup code here, to run once:
  // Configure two strips of 44 WS2812B LEDs on pins 2 and 3
  delay(500); // 0.5 second delay for recovery
  // FastLED.setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(255);
  FastLED.addLeds<LED_TYPE, DATA_PIN_A, GRB>(leds_a, NUM_LEDS_A);
  FastLED.addLeds<LED_TYPE, DATA_PIN_B, GRB>(leds_b, NUM_LEDS_B);
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void rainbow() 
{
  // FastLED's built-in rainbow generator
  // fill_palette(leds, NUM_LEDS, gHue, 7, palette, 255, LINEARBLEND);
  // A
  fill_palette(leds_a, NUM_LEDS_A, gHue, 3, palette, 255, LINEARBLEND);
  // B
  fill_palette(leds_b, NUM_LEDS_B, gHue, 3, palette, 255, LINEARBLEND);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  // A
  if( random8() < chanceOfGlitter) {
    leds_a[ random16(NUM_LEDS_A) ] += CRGB::White;
  }
  // B
  if( random8() < chanceOfGlitter) {
    leds_b[ random16(NUM_LEDS_B) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  // A
  fadeToBlackBy( leds_a, NUM_LEDS_A, 10);
  leds_a[random16(NUM_LEDS_A)] = ColorFromPalette(palette, gHue+random8(64));
  // B
  fadeToBlackBy( leds_a, NUM_LEDS_B, 10);
  leds_b[random16(NUM_LEDS_B)] = ColorFromPalette(palette, gHue+random8(64));

}

void sinelon()
{    
  // a colored dot sweeping back and forth, with fading trails
  // A
  fadeToBlackBy( leds_a, NUM_LEDS_A, 20);
  leds_a[beatsin16( 13, 0, NUM_LEDS_A-1 )] = ColorFromPalette(palette, gHue);
  // B
  fadeToBlackBy( leds_b, NUM_LEDS_B, 20);
  leds_b[beatsin16( 13, 0, NUM_LEDS_B-1 )] = ColorFromPalette(palette, gHue);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  // Common
  uint8_t BeatsPerMinute = 62;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  // A
  for( int i = 0; i < NUM_LEDS_A; i++) { //9948
    leds_a[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
  // B
  for( int i = 0; i < NUM_LEDS_B; i++) { //9948
    leds_b[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  // A
  byte dots_a = 10;
  byte dothue_a = 0;
  fadeToBlackBy( leds_a, NUM_LEDS_A, 20);
    for( int i = 0; i < 8; i++) {
    leds_a[beatsin16( i+(dots_a-1), 0, NUM_LEDS_A-1 )] |= ColorFromPalette(palette, dothue_a, 255);
    dothue_a += (4*dots_a);
  }
  // B
  byte dots_b = 10;
  byte dothue_b = 0;
  fadeToBlackBy( leds_b, NUM_LEDS_B, 20);
    for( int i = 0; i < 8; i++) {
    leds_b[beatsin16( i+(dots_b-1), 0, NUM_LEDS_B-1 )] |= ColorFromPalette(palette, dothue_b, 255);
    dothue_b += (4*dots_b);
  }
}

void rainbowWithGlitter() 
{
  rainbow();
  addGlitter(GLITTER);
}

void jungleWithGlitter(){
  juggle();
  addGlitter(GLITTER);
}

void sinelonWithGlitter(){
  sinelon();
  addGlitter(GLITTER);
}

void bpmWithGlitter(){
  bpm();
  addGlitter(GLITTER);
}

void confettiWithGlitter(){
  confetti();
  addGlitter(GLITTER);
}


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {rainbowWithGlitter,sinelonWithGlitter,bpmWithGlitter,jungleWithGlitter,confettiWithGlitter};

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}


void loop() {
  // put your main code here, to run repeatedly:
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 25 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 15 ) { nextPattern(); } // change patterns periodically

}
