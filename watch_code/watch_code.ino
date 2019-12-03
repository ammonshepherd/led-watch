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

uint32_t SECONDS_COLOR = strip.Color(0, 230, 0);
uint32_t MINUTES_COLOR = strip.Color(230, 0, 0);
uint32_t HOURS_COLOR = strip.Color(0, 0, 230);


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
  //Serial.begin(9600);
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

    if (GPS.newNMEAreceived()) {
      Serial.println(GPS.lastNMEA());
      if (!GPS.parse(GPS.lastNMEA())) {
        return;
      }
    }

  }
  
  hours(GPS.hour);
  minutes(GPS.minute);
  seconds(GPS.seconds);


  /*
    for (int s = 0; s < 60; s++) {
      seconds(s);
      delay(1000);
    }
  */

}

void hours(int h) {
  int led;
  if ( h >= 0 and h < 5) {
    led = 0;
  }
  if ( h > 4 and h < 10 ) {
    led = 1;
  }
  if ( h > 9 and h < 15 ) {
    led = 2;
  }
  if ( h > 14 and h < 20 ) {
    led = 3;
  }
  if ( h > 19 and h < 25 ) {
    led = 4;
  }
  if ( h > 24 and h < 30 ) {
    led = 5;
  }
  if ( h > 29 and h < 35 ) {
    led = 6;
  }
  if ( h > 34 and h < 40 ) {
    led = 7;
  }
  if ( h > 39 and h < 45 ) {
    led = 8;
  }
  if ( h > 44 and h < 50 ) {
    led = 9;
  }
  if ( h > 49 and h < 55 ) {
    led = 10;
  }
  if ( h > 54) {
    led = 11;
  }

  strip.clear();
  strip.setPixelColor(led, HOURS_COLOR);
  strip.show();
}

void minutes(int m) {
  Serial.println(m);
  int led;
  if ( m >= 0 and m < 5) {
    Serial.println(" = zero - 5");
    led = 0;
  }
  if ( m > 4 and m < 10 ) {
    led = 1;
  }
  if ( m > 9 and m < 15 ) {
    led = 2;
  }
  if ( m > 14 and m < 20 ) {
    led = 3;
  }
  if ( m > 19 and m < 25 ) {
    led = 4;
  }
  if ( m > 24 and m < 30 ) {
    led = 5;
  }
  if ( m > 29 and m < 35 ) {
    led = 6;
  }
  if ( m > 34 and m < 40 ) {
    led = 7;
  }
  if ( m > 39 and m < 45 ) {
    led = 8;
  }
  if ( m > 44 and m < 50 ) {
    led = 9;
  }
  if ( m > 49 and m < 55 ) {
    led = 10;
  }
  if ( m > 54) {
    led = 11;
  }

  strip.clear();
  strip.setPixelColor(led, MINUTES_COLOR);
  strip.show();
}

void seconds(int s) {
  // map the input 0-60 to 0-11, then constrain to be a whole number?
  Serial.println(s);
  int led;
  if ( s >= 0 and s < 5) {
    Serial.println(" = zero - 5");
    led = 0;
  }
  if ( s > 4 and s < 10 ) {
    led = 1;
  }
  if ( s > 9 and s < 15 ) {
    led = 2;
  }
  if ( s > 14 and s < 20 ) {
    led = 3;
  }
  if ( s > 19 and s < 25 ) {
    led = 4;
  }
  if ( s > 24 and s < 30 ) {
    led = 5;
  }
  if ( s > 29 and s < 35 ) {
    led = 6;
  }
  if ( s > 34 and s < 40 ) {
    led = 7;
  }
  if ( s > 39 and s < 45 ) {
    led = 8;
  }
  if ( s > 44 and s < 50 ) {
    led = 9;
  }
  if ( s > 49 and s < 55 ) {
    led = 10;
  }
  if ( s > 54) {
    led = 11;
  }

  strip.clear();
  strip.setPixelColor(led, SECONDS_COLOR);
  strip.show();
}
