/**
  WiFi Scanner
  Name: FS-example
  Purpose: Test station mode WiFi capabilities

  @author Riccardo Zanella
  @version 0.1 19/01/19
*/
#include "FS.h"

const int BAUD_RATE = 115200;

void setup() {
  // serial port settings
  Serial.begin(BAUD_RATE);

  // WiFi settings
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(WIFI_SETUP_GRACETIME);

  Serial.println("Setup done");
}

void loop() {
}
