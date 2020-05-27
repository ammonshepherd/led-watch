/*
   Author: Ammon Shepherd
   Date: 29 April 2020

   Make all the LEDs brightest white

   Parts:
     - NeoPixel Ring: https://www.adafruit.com/product/1643
     - Flora Wearable Ultimate GPS Module: https://www.adafruit.com/product/1059
     - Flora Main Board: https://www.adafruit.com/product/659
*/
#include <Adafruit_NeoPixel.h>
#define LED_PIN 6
#define LED_COUNT 12


#define BUTTON_PIN 10
#define DEF_BRIGHT 0

int btn_state = 0;

Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  ring.begin();
  ring.show();  

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  int brightness;
  
  //Finish button code. button state starts at 0 (see above), every time
  // the button is pressed, it switches to the next level, and when reaches
  // 3 loops back to 0
  
  switch (btn_state) {
    case 0: brightness = 255; break;
    case 1: brightness = 100; break;
    case 3: brightness = 5; break;
    default: brightness = 255; 
  }

  ring.setBrightness(brightness);
  for ( int i = 0; i <= LED_COUNT; i++) {
    ring.setPixelColor(i, ring.Color(255, 255, 255));
    ring.show();
  }

}
