/*
   Author: Ammon Shepherd
   Date: November 21, 2019

   Display the time on an Adafruit NeoPixel Ring with Adafruit GPS module and Flora

   Parts:
     - NeoPixel Ring: https://www.adafruit.com/product/1643
     - Flora Wearable Ultimate GPS Module: https://www.adafruit.com/product/1059
     - Flora Main Board: https://www.adafruit.com/product/659

    NeoPixel Ring Pin Layout:
     Time : PIN
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
#include <Adafruit_GPS.h>

#define GPSSerial Serial1
#define LED_PIN 6
#define LED_COUNT 12

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_GPS GPS(&GPSSerial);
// Set to false to not send serial data
#define GPSECHO true

uint32_t timer = millis();

void setup() {
  Serial.begin(115200);
  Serial.println("LED Watch Code");

  // Start the GPS stuff
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);

  // Start the LED ring off
  strip.begin();
  strip.show();
  strip.setBrightness(3);


}

void loop() {
  char c = GPS.read();

  // debug info, set above
  if (GPSECHO) {
    if (c) {
      Serial.print(c);
    }
  }
  if (GPS.newNMEAreceived()) {
    Serial.println(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA())) {
      return;
    }
  }

  if (timer > millis() ) {
    timer = millis();
  }

  Serial.print("hour: ");
  Serial.println(GPS.hour, DEC);

  Serial.print("minute: ");
  Serial.println(GPS.minute, DEC);

  Serial.print("second: ");
  Serial.println(GPS.seconds, DEC);

  Serial.print("day: ");
  Serial.println(GPS.day, DEC);

  Serial.print("month: ");
  Serial.println(GPS.month, DEC);

  Serial.print("year: ");
  Serial.println(GPS.year, DEC);

  //seconds(GPS.seconds);
  
  for (int s = 0; s < 60; s++) {
    seconds(s);
    delay(1000);
  }
  

}


void seconds(int s) {
  // map the input 0-60 to 0-11, then constrain to be a whole number
  Serial.print(s);
  int led;
  if ( s >= 0 and s < 6) {
    Serial.println(" = zero - 5");
    led = 0;
  }
  if ( s > 4 and s < 10 ) {
    led = 1;
  }
  if ( s > 9 and s < 16 ) {
    led = 2;
  }
  if ( s > 15 and s < 21 ) {
    led = 3;
  }
  if ( s > 20 and s < 26 ) {
    led = 4;
  }
  if ( s > 25 and s < 31 ) {
    led = 5;
  }
  if ( s > 30 and s < 36 ) {
    led = 6;
  }
  if ( s > 35 and s < 41 ) {
    led = 7;
  }
  if ( s > 40 and s < 46 ) {
    led = 8;
  }
  if ( s > 45 and s < 51 ) {
    led = 9;
  }
  if ( s > 50 and s < 56 ) {
    led = 10;
  }
  if ( s > 55) {
    led = 11;
  }
  uint32_t color = strip.Color(0, 230, 0);
  strip.clear();
  strip.setPixelColor(led, color);
  strip.show();
}