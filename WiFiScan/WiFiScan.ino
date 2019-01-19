/**
  WiFi Scanner
  Name: WIFIScan
  Purpose: Test station mode WiFi capabilities

  @author Riccardo Zanella
  @version 0.1 19/01/19
*/
#include "ESP8266WiFi.h"

const int BAUD_RATE = 115200;
const int WIFI_SETUP_GRACETIME = 100;
const int SCAN_PERIOD = 500;

void setup() {
  // serial port settings
  Serial.begin(BAUD_RATE);

  // WiFi settings
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(WIFI_SETUP_GRACETIME);

  Serial.println("Setup done");
}

void network_stats_print(int net){  
  String serial_buffer(net);
  serial_buffer += ": (";
  serial_buffer += WiFi.RSSI(net);
  serial_buffer += ") ";
  serial_buffer += WiFi.SSID(net);
  serial_buffer += (WiFi.encryptionType(net) == ENC_TYPE_NONE)?"":"*";
  
  Serial.print("\n");
  Serial.print(serial_buffer);
}

void loop() {
  Serial.print("\n\nscan start");
  int total_networks = WiFi.scanNetworks();
  
  Serial.print(", found ");
  Serial.print(total_networks);
  Serial.print(" networks:");

  for (int net = 0; net < total_networks; ++net)
    network_stats_print(net);
   
  delay(SCAN_PERIOD);
}
