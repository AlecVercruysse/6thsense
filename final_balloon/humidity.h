//#include <SPI.h>
#include "adafruit/Adafruit_BME280.h"
#include "adafruit/Adafruit_BME280.cpp"

/**
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
/**/

Adafruit_BME280 bme; // I2C address 0x77
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float sensorReading; //create global variable to store sensor reading

void setupBME() {

    bool status;

    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
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
