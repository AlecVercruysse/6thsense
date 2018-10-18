//#include "VernierLib.h" //include Vernier functions in this sketch
//VernierLib Vernier; //create an instance of the VernierLib library
#include <Wire.h>
#include <SPI.h>
//#include "adafruit/Adafruit_Sensor.h"
#include "adafruit/Adafruit_BME280.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float sensorReading; //create global variable to store sensor reading
unsigned long delayTime;

void setupBME() {
    Serial.begin(9600);
    //Vernier.autoID(); //identify the sensor being used

    bool status;
    //Serial.println("Standard,Arduino");

    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    status = bme.begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

String getBMEHumidity() {
    // not bme380, but rather the standard/probe
//    float arduinoVoltage = analogRead(A0); //print data value 
//    float sensorVoltage = arduinoVoltage / 1024 * 5.0;
//    float humidityPCT = sensorVoltage * 30.43 - 25.81;
    
    //Serial.print("Standard humidity: ");
    //Serial.print(humidityPCT);
    //Serial.print(",");
    //Serial.println(" %");
    
    //Serial.print("Pressure = ");
    //Serial.print(bme.readPressure() / 100.0F);
    //Serial.println(" hPa");

    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");
//
//    Serial.print("Arduino humidity = ");
//    Serial.print(bme.readHumidity());
//    Serial.println(" %");

    String theHumidity = String(bme.readHumidity());

//    String humidity = String("Arduino humidity = " + theHumidity + " %");
    return theHumidity;

//    Serial.print("Arduino temperature = ");
//    Serial.print(bme.readTemperature());
//    Serial.println(" *C");
//
//    Serial.println();
}

String getBMETemp()
{
//    Serial.print("Arduino temperature = ");
//    Serial.print(bme.readTemperature());
//    Serial.println(" *C");

    String theTemp = String(bme.readTemperature());

//    String temperature = String("Arduino temperature = " + theTemp + " *C");
    return theTemp;
}


void loopBME() { 
    printValues();
    delay(1000);
}
