#include <ESP8266WebServer.h>
#include "ServerHandlers.h"
#include "FS.h"

extern ESP8266WebServer server;

String getPage(const char * filename){
  String out = "";
  SPIFFS.begin();
  File fid = SPIFFS.open(filename, "r");
  if (fid){
    out += fid.readString();
    fid.close();
  }
  else{
    out += "Can't read ";
    out += filename; 
  }
  return out;
}

void handleRoot() {
  String out = getPage("/index.html"); 
  server.send(200, "text/html", out);
  
  SPIFFS.end();
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}
