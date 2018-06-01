/*
 *  This sketch is a part of examples code of ESP9266WiFi lib made by Ivan Grokhotkov
 *
 *  To compile this sketch You need to add this library https://github.com/esp8266/Arduino
 *
 *  This sketch demonstrates how to scan WiFi networks. 
 *  The API is almost the same as with the WiFi Shield library, 
 *
 * OLED class "Display" replace the "Serial" api calls of arduino core.
 */
#include "ESP8266WiFi.h"
#include <oled.h>

// There are 2 different versions of the board
// OLED Display=OLED(2,14,4); 
OLED Display=OLED(4,5,16);

void setup() {
  Display.begin(); // Instead of Serial.begin(9600)
  Display.setTTYMode(true); // This set the TTY mode

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Display.print("Setup done");
}

void loop() {
  Display.println();
  Display.print("Scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Display.println("\r\nScan done");
  if (n == 0)
    Display.printf("No networks found");
  else
  {
    Display.printf("%d networks found",n);
    for (int i = 0; i < n; ++i)
    {
      Display.println();
      // Print SSID and RSSI for each network found
      Display.print(i + 1);
      Display.print(": ");
      Display.print(WiFi.SSID(i));
      Display.print(" (");
      Display.print(WiFi.RSSI(i));
      Display.print(")");
      delay(10);
    }
  }
  
  // Wait a bit before scanning again
  delay(5000);
}
