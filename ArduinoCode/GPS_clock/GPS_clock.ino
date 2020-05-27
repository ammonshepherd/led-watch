/* GPS LED Watch
 * author: Ammon Shepherd
 * date: May 2020
 * License: use it however you want, just attribute back to me.
 *
 * Much of the GPS code is from Adafruit GPS 
 * https://github.com/adafruit/Adafruit_GPS/blob/master/examples/GPS_HardwareSerial_Timing/GPS_HardwareSerial_Timing.ino
 *
*/
#include <Adafruit_GPS.h>

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO true

uint32_t timer = millis();

/* LED Ring code

*/
#include <Adafruit_NeoPixel.h>
#define LED_PIN 6
#define LED_COUNT 12

Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // connect at 115200 so we can read the GPS fast enough and echo without
  // dropping chars also spit it out
  Serial.begin(115200);
  //Serial.println("Adafruit GPS library basic test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ); // 10 second update time
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  // Ask for firmware version
  //GPSSerial.println(PMTK_Q_RELEASE);

  ring.begin();
  ring.setBrightness(3);
}

void loop() // run over and over again
{
  ring.clear();

  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  if (GPSECHO)
    if (c)
      //Serial.print(c);
      // if a sentence is received, we can check the checksum, parse it...
      if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
        //Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived()
        // flag to false
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag
          // to false
          return; // we can fail to parse a sentence in which case we should just
        // wait for another
      }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis())
    timer = millis();


  // approximately every 2 seconds or so, random intervals, print out the
  // current stats
  static unsigned nextInterval = 2000;
  if (millis() - timer > nextInterval) {
    timer = millis(); // reset the timer
    nextInterval = 1500 + random(1000);
    // Time in seconds keeps increasing after we get the NMEA sentence.
    // This estimate will lag real time due to transmission and parsing delays,
    // but the lag should be small and should also be consistent.
    int s = GPS.seconds + GPS.milliseconds / 1000. + GPS.secondsSinceTime();
    int m = GPS.minute;
    int h = GPS.hour;

    while (s > 60) {
      s -= 60;
      m++;
    }
    while (m > 60) {
      m -= 60;
      h++;
    }
    while (h > 24) {
      h -= 24;

    }

    // Convert hours from 0-24 to 1-12
    if (h > 12) {
      h -= 12;
    } else if (h = 0) {
      h = 12;
    }

    /* LED pin 9 is at the 12 hour spot. LED pin 3 is at
       the 6 hour spot.

       The hour is from GPS.hour which is 4 hours ahead
       of EST, so the pin number compensates for EST by
       subtracting 4.

       Hour | PIN
          1 = 6
          2 = 5
          3 = 8
          4 = 9
          5 = 10
          6 = 11
          7 = 0
          8 = 1
          9 = 2
         10 = 3
         11 = 4
         12 = 5
    */
    int hourLED = 6;
    if (h > 6) {
      hourLED = h - 7;
    } else if (h < 7) {
      hourLED = h + 5;
    }

    /* The minute LED can be generated easily by
        dividing m by 5 (m/5 = led pin) since
        (60/5) is a number between 1-12, but we need
        to rotate the numbers since pin 9 is at the
        12 hour mark.

        Minute | Pin
           1-5 = 9
          6-10 = 10
         11-15 = 11
         16-20 = 0
         21-25 = 1
         26-30 = 2
         31-35 = 3
         36-40 = 4
         41-45 = 5
         46-50 = 6
         51-55 = 7
         56-60 = 8
    */
    int minuteLED = 0;
    if (m > 15) {
      minuteLED = m / 5 - 3;
    } else if (m < 16 ) {
      minuteLED = m / 5 + 9;
    }


    // Seconds are the same as minutes, but they go around faster.
    int secLED = 0;
    if (s > 15) {
      secLED = s / 5 - 3;
    } else if (m < 16) {
      secLED = s / 5 + 9;
    }

    // If the hour and minute are the same LED, make it purple
    if (hourLED == minuteLED) {
      ring.setPixelColor(hourLED, ring.Color(200, 50, 200) );
    } else {
      // Set the hour LED
      ring.setPixelColor(hourLED, ring.Color(200, 50, 50) );
      // Set the minute LED
      ring.setPixelColor(minuteLED, ring.Color(50, 50, 100 + m * 2) );
    }

    // Set the second LED - and it blinks
    ring.setPixelColor(secLED, ring.Color(50, 50 + s * 5, 50) );
    ring.show();
    delay(400);

    /* If the second is on the hour or minute LED set the
       LED as red or green, otherwise set it as off for a
       nice blinking affect.
    */
    if (secLED == hourLED) {
      ring.setPixelColor(hourLED, ring.Color(200, 50, 50) );
    } else if (secLED == minuteLED) {
      ring.setPixelColor(minuteLED, ring.Color(50, 50, 100 + m * 2) );
    } else {
      ring.setPixelColor(secLED, ring.Color(0, 0, 0) );
    }

    // Light up the LEDs based on the colors set above.
    ring.show();

    // Uncomment to show GPS data
    //details(h, m, s);
  }
}


void details(int h, int m, int s) {
  Serial.print("\n\nTime: ");
  if (h < 10)
    Serial.print("0");
  Serial.print(h, DEC);
  Serial.print(':');
  if (m < 10)
    Serial.print("0");
  Serial.print(m, DEC);
  Serial.print(':');
  if (s < 10)
    Serial.print("0");
  Serial.println(s, 3);
  Serial.print("Fix: ");
  Serial.print((int)GPS.fix);
  Serial.print(" quality: ");
  Serial.println((int)GPS.fixquality);
  Serial.print("Time [s] since last fix: ");
  Serial.println(GPS.secondsSinceFix(), 3);
  Serial.print("    since last GPS time: ");
  Serial.println(GPS.secondsSinceTime(), 3);
  Serial.print("    since last GPS date: ");
  Serial.println(GPS.secondsSinceDate(), 3);
  if (GPS.fix) {
    Serial.print("Location: ");
    Serial.print(GPS.latitude, 4);
    Serial.print(GPS.lat);
    Serial.print(", ");
    Serial.print(GPS.longitude, 4);
    Serial.println(GPS.lon);
    Serial.print("Speed (knots): ");
    Serial.println(GPS.speed);
    Serial.print("Angle: ");
    Serial.println(GPS.angle);
    Serial.print("Altitude: ");
    Serial.println(GPS.altitude);
    Serial.print("Satellites: ");
    Serial.println((int)GPS.satellites);
  }

}
