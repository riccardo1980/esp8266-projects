/**
  WebServer and LED
  Name: TinyWeather
  Purpose: Loads data from BMP280 and Shows values via WiFi

  @author Riccardo Zanella
  @version 0.1 28/09/19
*/

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include "FS.h"
#include "ServerHandlers.h"

#define STATUS_LED D0

const int BAUD_RATE = 115200;

const int WIFI_GRACETIME = 500;
const int WIFI_RETRY = 5;
const char *WIFI_CFG_FILE = "/wifi.json";

const char *ESSID;
const char *PASSWORD;
const int SERVER_PORT = 80;
const char *SERVER_NAME = "tinyweather";
ESP8266WebServer server(SERVER_PORT);

bool wifi_read_cfg (const char *filename){
  File fid = SPIFFS.open(filename, "r");
  if (fid){
    String content = fid.readString();
    fid.close();
    const size_t capacity = JSON_OBJECT_SIZE(2) + 60;
    DynamicJsonDocument config(capacity);
    DeserializationError error = deserializeJson(config, content);
    if (error)
      return false;
    ESSID = config["ESSID"];
    PASSWORD = config["PASSWORD"];
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

bool mDNS_start(){
  return MDNS.begin(SERVER_NAME);
}

void setup() {
  // serial port settings
  Serial.begin(BAUD_RATE);
  pinMode(STATUS_LED, OUTPUT);

  // initializations
  bool SPIFFS_IS_INIT = SPIFFS.begin();
  bool WIFI_CFG_OK = wifi_read_cfg(WIFI_CFG_FILE);
  SPIFFS.end();
  
  bool WIFI_CONNECT_OK = wifi_connect(WIFI_RETRY, WIFI_GRACETIME);
  bool MDNS_OK = mDNS_start();
  
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  
  // output logging
  Serial.print("\nSPIFFS init: ");
  Serial.print((SPIFFS_IS_INIT?"ok":"error")); 

  Serial.print("\nwifi load cfg: ");
  Serial.print((WIFI_CFG_OK?"ok":"error")); 
  
  Serial.print("\nwifi connect: ");
  Serial.print((WIFI_CONNECT_OK?WiFi.localIP().toString():"error")); 

  Serial.print("\nmDNS responder: ");
  Serial.print((MDNS_OK?"ok":"error")); 
  
  if (SPIFFS_IS_INIT && WIFI_CONNECT_OK && MDNS_OK){
    Serial.print("\nHTTP server started");
    digitalWrite(STATUS_LED, LOW);
  }

}

void loop() {
  server.handleClient();
}
