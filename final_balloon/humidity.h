//#include <SPI.h>
#include "adafruit/Adafruit_BME280.h"
#include "adafruit/Adafruit_BME280.cpp"

Adafruit_BME280 bme; // I2C address 0x77

float sensorReading; //create global variable to store sensor reading

void setupBME() {
    bool status;
    Serial.println("starting BME");
    status = bme.begin();
    Serial.println("bme status: "); Serial.println(status);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

String getBMEHumidity() {
    String theHumidity = String(bme.readHumidity());
    return theHumidity;
}

String getBMETemp()
{
    String theTemp = String(bme.readTemperature());
    return theTemp;
}
