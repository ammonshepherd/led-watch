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
  

  show_time(GPS.hour, "hours");
  show_time(GPS.minute, "minutes");
  show_time(GPS.seconds, "seconds");
  

  /*
    for (int s = 0; s < 60; s++) {
      seconds(s);
      delay(1000);
    }
  */

}

void show_time(int t, String color) {
  int led;
  if ( t >= 0 and t < 5) {
    led = 0;
  }
  if ( t > 4 and t < 10 ) {
    led = 1;
  }
  if ( t > 9 and t < 15 ) {
    led = 2;
  }
  if ( t > 14 and t < 20 ) {
    led = 3;
  }
  if ( t > 19 and t < 25 ) {
    led = 4;
  }
  if ( t > 24 and t < 30 ) {
    led = 5;
  }
  if ( t > 29 and t < 35 ) {
    led = 6;
  }
  if ( t > 34 and t < 40 ) {
    led = 7;
  }
  if ( t > 39 and t < 45 ) {
    led = 8;
  }
  if ( t > 44 and t < 50 ) {
    led = 9;
  }
  if ( t > 49 and t < 55 ) {
    led = 10;
  }
  if ( t > 54) {
    led = 11;
  }

  strip.clear();
  uint32_t display_color;
  if (color == "hours") {
    display_color = HOURS_COLOR;
  } else if (color == "minutes") {
    display_color = MINUTES_COLOR;
  } else if (color == "seconds") {
    display_color = SECONDS_COLOR;
  }
  strip.setPixelColor(led, display_color);
  strip.show();
}
