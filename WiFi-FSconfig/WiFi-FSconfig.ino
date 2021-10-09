/**
  WiFi Scanner
  Name: FS-example
  Purpose: Load WiFi config from json using SPIFFS and ArduinoJson

  @author Riccardo Zanella
  @version 0.2 20/01/19
*/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "FS.h"

const int BAUD_RATE = 115200;
const int SCAN_PERIOD = 1000;

const int WIFI_GRACETIME = 500;
const int WIFI_RETRY = 500;
const char *WIFI_CFG_FILE = "/wifi.json";

const char *ESSID;
const char *PASSWORD;

bool wifi_read_cfg (const char *filename) {

  File fid = SPIFFS.open(filename, "r");
  if (fid) {
    String content = fid.readString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + content.length() + 1;

    DynamicJsonDocument doc(capacity);
    DeserializationError err = deserializeJson(doc, content);
    if (err) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(err.c_str());
      return false;
    }
    ESSID = doc["ESSID"];
    PASSWORD = doc["PASSWORD"];
    return true;
  }
  else
    return false;
}

bool wifi_connect(int retry, int gracetime) {

  WiFi.begin(ESSID, PASSWORD);
  bool result = false;
  int status;
  
  Serial.print(F("WiFi connecting"));
  while (retry > 0 && !(result = ((status = WiFi.status()) == WL_CONNECTED))) {
    --retry;
    delay(gracetime);
    Serial.printf(".");
  }
    Serial.printf("\n");
  return result;
}

#define SAFE_CHECK(X) do {Serial.printf(X ? "ok" : "error");} while(0);

void setup() {
  // serial port settings
  Serial.begin(BAUD_RATE);

  // initializations
  
  // output logging
  Serial.printf("\nSPIFFS init: ");
  SAFE_CHECK(SPIFFS.begin());

  Serial.print("\nwifi load cfg: ");
  bool WIFI_CFG_OK = wifi_read_cfg(WIFI_CFG_FILE);
  Serial.print((WIFI_CFG_OK ? "ok" : "error"));
  
  Serial.print("\nwifi connect: ");
  bool WIFI_CONNECT_OK = wifi_connect(WIFI_RETRY, WIFI_GRACETIME);
  Serial.print((WIFI_CONNECT_OK ? "ok" : "error"));
  
  Serial.println();
}

void loop() {

  Serial.println(WiFi.localIP());
  delay(SCAN_PERIOD);
}
