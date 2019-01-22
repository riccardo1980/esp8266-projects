# Simple Web Server

See also [ESP8266 First Web Server](https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html).

1. Install Arduino IDE tool for file upload to esp8266 following the steps described [here](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html#uploading-files-to-file-system).  
2. Install ArduinoJson library: see [here](https://arduinojson.org/v5/doc/instsllation/) for details.
3. Create __data/wifi.json__ with your configuration parameters (ESSID, PASSWORD): use __data/wifi-tpl.json__ as a template.
4. Load the content of __data__ folder using installed tool: tools > ESP8266 Sketch Data Upload
