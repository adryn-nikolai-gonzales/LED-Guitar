/*******************LED GUITAR CODE*******************
 * 
 *         Author: Adryn N Gonzales
 *           Date: September 05 2020
 *  Last Modified: January 23 2021
 *  
 *    Description: Code for my personal project, a guitar with LED's 
 *    reactive to the audio signal produced by the guitar.
 *    The brightness of the LED's are directly controlled by the 
 *    intensity of the guitar's output analog signal.
 *    
 *    
 *    TODO: *Make audio reading more 'smooth'
 *              (for every few cycles, the brightness will be 0)
 *          *Customize Colors/Gradients
 *              (perhaps down the line include push button
 *               or potentiometer to shuffle through colors)
 *          *Improve on interpolation calculations for LED brightness
 *               to better represent the sound loudness 
 *    

 */
#include "FastLED.h"

#define LED_PIN    5      //digital output into LED strip: pin 5
#define INPUT_PIN  A0     //analog input pin
#define NUM_LEDS   30     //number of total leds
#define LED_TYPE   WS2812 //led model type

CRGB leds[NUM_LEDS]; 

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

#define UPDATES_PER_SECOND 50
uint8_t val = 0;  //variable to store the value read from A0
uint8_t max = 0;  //variable used to help store the max value from a few cycles
unsigned int count = 0;// count number of cycles to store a max value

void setup() { 
  delay(5000); //safety delay
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
  
  Serial.begin(9600); //for monitor debugging

  //setup led information
  FastLED.addLeds<LED_TYPE, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //set initial brightness
  FastLED.setBrightness(50);

}

void loop()
{
    // read signal value
    val = analogRead(INPUT_PIN);  // read the input pin 
    if (val > max) {
      max = val;
    }

    if (count % 50 == 0) { // save the max value for 50 cycles of the loop
      max = 0;
      count = 0;
    }

    count++;
   
    static uint8_t startIndex = 0;
    static uint8_t brightness = 1;

    // interpolate read value to brightness
    // NOTE: the analog signals generally range from 5 mV - 150 mV
    // However, the read values returned from the arduino are 20 - 50 (+/- of 3)
    //     The calcs below attempts to interpolate the readings to a 0-255 scale
    brightness = (max - 20) * 8.5;
    
    // brightness safeguard
    // will have to convert to uint8
    //if (b_val > 255) {
      //b_val = 255;
    //} else if (b_val < 1) {
      //b_val = 0;
    //}

    
    startIndex = startIndex + 1; // motion speed

    Serial.println(brightness);  // debug tool
    //Serial.println(b_val);
    
    FillLEDsFromPaletteColors(startIndex, brightness);
    currentPalette = LavaColors_p; //included in library
    currentBlending = LINEARBLEND; 
        
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex, uint8_t brightness)
{
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
