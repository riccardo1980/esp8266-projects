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
const int WIFI_RETRY = 5;
const char *WIFI_CFG_FILE = "/wifi.json";

const char *ESSID;
const char *PASSWORD;

bool wifi_read_cfg (const char *filename){
  File fid = SPIFFS.open(filename, "r");
  if (fid){
    String content = fid.readString();
    const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject& root = jsonBuffer.parseObject(content);
    ESSID = root["ESSID"];
    PASSWORD = root["PASSWORD"];
    return true;
  }
  else
    return false;
}

bool wifi_connect(int retry, int gracetime){
  WiFi.begin(ESSID, PASSWORD);
  bool result = false;
  while(retry > 0 && !(result = (WiFi.status() == WL_CONNECTED))){ 
    --retry;
    delay(gracetime);
  }
  return result;
}

void setup() {
  // serial port settings
  Serial.begin(BAUD_RATE);

  // initializations
  bool SPIFFS_IS_INIT = SPIFFS.begin();
  bool WIFI_CFG_OK = wifi_read_cfg(WIFI_CFG_FILE);
  bool WIFI_CONNECT_OK = wifi_connect(WIFI_RETRY, WIFI_GRACETIME);
  
  // output logging
  Serial.print("\nSPIFFS init: ");
  Serial.print((SPIFFS_IS_INIT?"ok":"error")); 

  Serial.print("\nwifi load cfg: ");
  Serial.print((WIFI_CFG_OK?"ok":"error")); 
  
  Serial.print("\nwifi connect: ");
  Serial.print((WIFI_CONNECT_OK?"ok":"error")); 
  
}

void loop() {
  Serial.println(WiFi.localIP());
  delay(SCAN_PERIOD);
}
