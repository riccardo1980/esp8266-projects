
#include <Wire.h>
#include <Adafruit_BME280.h>

#define BME280_I2C_ADDRESS (0x76)
#define SERIAL_BAUD_RATE (115200)
#define LOOP_DELAY (5000)

Adafruit_BME280 bme;
unsigned long last_run;
bool bme_status;
/*
 * IDEA: basic meas. device for greenhouse
 * 
 * measurements: temperature, pressure, humidity
 * transport: mqtt over wifi
 *  
 *  BASE:
 *  1. basic code for sensor handling
 *  2. SPIFFS support
 *  3. json config file handling 
 *  4. wifi authentication
 *  5. timestamp retrieval 
 *  6. mqtt authentication & send
 *  
 *  ADVANCED:
 *  7. bme280 suggested scenarios:
 *    - https://github.com/adafruit/Adafruit_BME280_Library/blob/master/examples/advancedsettings/advancedsettings.ino
 *  8. esp8266 deep sleep mode 
 *  
 */

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println(F("BME280 test"));

  bme_status = bme.begin(BME280_I2C_ADDRESS);  
  if (!bme_status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }
  
  last_run = millis();
  Serial.println("-- Default Test --");
  Serial.println();
}


void loop() {
  if (!bme_status){
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    return;
  }
  if(millis() - last_run > LOOP_DELAY){
    last_run = millis();
    float temperature; // C
    float pressure;    // Pa
    float humidity;    // percent
  
    getValues(&temperature, &pressure, &humidity);
    printValues(temperature, pressure, humidity);    
  
  }
}

void getValues(float *temperature, float *pressure, float *humidity){
  *temperature = bme.readTemperature(); // C
  *pressure = bme.readPressure();       // Pa
  *humidity = bme.readHumidity();       // percent
}


#define TEMPERATURE_C_DECIMALS (1)
#define PRESSURE_HPA_DECIMALS (0)
#define HUMIDITY_PERC_DECIMALS (0)
void printValues(float temperature, float pressure, float humidity) {
  Serial.print("Temperature = ");
  Serial.print(temperature, TEMPERATURE_C_DECIMALS);
  Serial.println(" °C");
  
  Serial.print("Pressure = ");
  Serial.print(pressure / 100.0F, PRESSURE_HPA_DECIMALS);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(humidity, HUMIDITY_PERC_DECIMALS);
  Serial.println(" %");

  Serial.println();
}
