

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Metro.h>

#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

#define DHTPIN D5    // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration


Metro go_metro = Metro(1000);

void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");

  // These three lines of code are all you need to initialize the
  // OLED and print the splash screen.

  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)


  dht.begin();

  //setup_wifi( );

  do_sensor();
}

void loop() {
  if (go_metro.check() == 1) {

    do_sensor();

  }


}

void do_sensor() {

  get_dht22();

}

void get_dht22() {

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println("");

  oled.clear(PAGE);  // Clear the buffer
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(0, 0); // points cursor to x=27 y=0

  oled.println("Humidity:");
  oled.print(h);
  oled.println(" %\t");
  oled.println(" ");

  oled.println("Temp.: ");
  oled.print(t);
  oled.println(" *C");
  oled.println(" ");
  
  oled.flipVertical(true);
  oled.flipHorizontal(true);

  oled.display(); // Draw the memory buffer
}


