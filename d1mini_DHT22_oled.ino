#define serdebug
#ifdef serdebug
#define DebugPrint(...) {  Serial.print(__VA_ARGS__); }
#define DebugPrintln(...) {  Serial.println(__VA_ARGS__); }
#else
#define DebugPrint(...) { }
#define DebugPrintln(...) { }
#endif

#include "cy_wifi.h"
#include "cy_ota.h""
#include "cy_weather.h"

#include "DHT.h"
#include <Ticker.h>

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

const char* gv_hostname = "D1DHTOLED";

Ticker TickMeas;
boolean gv_TickMeas;

void HandleTickMeas( ){
  gv_TickMeas = true;
}

void setup() {
#ifdef serdebug
  Serial.begin(115200);
#endif
  DebugPrintln("\n" + String(__DATE__) + ", " + String(__TIME__) + " " + String(__FILE__));

  // These three lines of code are all you need to initialize the
  // OLED and print the splash screen.

  oled.begin();     // Initialize the OLED
  oled.flipVertical(true);
  oled.flipHorizontal(true);
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(0, 0); // points cursor to x=0 y=0
  oled.println("try WiFi");
  oled.display();   // Display what's in the buffer (splashscreen)

  wifi_init(gv_hostname);

  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(0, 0); // points cursor to x=0 y=0
  oled.println("WiFi OK");
  oled.display();   // Display what's in the buffer (splashscreen)

  delay(500);

  init_ota(gv_hostname);

  dht.begin();
  do_sensor();
  gv_TickMeas = false;
  TickMeas.attach(10, HandleTickMeas);
}

void loop() {
  check_ota();

  if (gv_TickMeas == true) {

    do_sensor();
    gv_TickMeas = false;

  }

  delay(500);


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
    DebugPrintln("Failed to read from DHT sensor!");
    return;
  }

  DebugPrint("Humidity: ");
  DebugPrint(h);
  DebugPrint(" % \t");
  DebugPrint("Temperature: ");
  DebugPrint(t);
  DebugPrint(" *C ");
  DebugPrintln("");

  oled.clear(PAGE);  // Clear the buffer
  oled.setFontType(0); // set font type 0, please see declaration in SFE_MicroOLED.cpp
  oled.setCursor(0, 0); // points cursor to x=0 y=0

  oled.println("Humidity");
  oled.print(h);
  oled.println(" % ");
  oled.println(" ");

  oled.println("Temp.");
  oled.print(t);
  oled.println(" \tC");
  oled.println(" ");

  oled.display(); // Draw the memory buffer
}
