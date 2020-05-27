/*
   Author: Ammon Shepherd
   Date: 29 April 2020

   Basic Clock Functionality

   Parts:
     - NeoPixel Ring: https://www.adafruit.com/product/1643
     - Flora Wearable Ultimate GPS Module: https://www.adafruit.com/product/1059
     - Flora Main Board: https://www.adafruit.com/product/659

   NeoPixel Ring LED Layout:
     Time : LED #
     - 12 : 0
     - 01 : 1
     - 02 : 2
     - 03 : 3
     - 04 : 4
     - 05 : 5
     - 06 : 6
     - 07 : 7
     - 08 : 8
     - 09 : 9
     - 10 : 10
     - 11 : 11


*/
#include <Adafruit_NeoPixel.h>
#define LED_PIN 6
#define LED_COUNT 12


Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.setBrightness(3);
  Serial.begin(9600);
}

void loop() {
  minutes();
}

void minutes() {
  int red, green, blue;
  red = 200;
  green = 50;
  blue = 50;
  for ( int m = 0; m < LED_COUNT; m++ ){
    ring.setPixelColor(m, ring.Color(red, green, blue) );
    ring.show();
    seconds();
    delay(60000 * 5);
    
  }
}

/* Show the seconds (green)
 *  Each LED is five seconds, so it blinks 5 times
 */
void seconds() {
  int red, green, blue;
  // Loop through all the LEDs
  for ( int p = 0; p < LED_COUNT; p++ ) {
    // 5 times for each LED
    for ( int c = 1; c <= 5; c++ ) {
      // turn off all the pixels first
      //ring.clear();
      // Show no light for 200ths of a second
      ring.setPixelColor(p, ring.Color(0,0,0));
      ring.show();
      delay(200);
      
      // Set the color values, the brightness increases each second
      red = 10 + c * 4;
      green = 50 + c * 37;
      blue = 10 + c * 6; 
      ring.setPixelColor(p, ring.Color(red, green, blue));
      ring.show();
      delay(800);
    }
  }
}
