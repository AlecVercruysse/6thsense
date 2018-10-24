//
// Code for Kris Winer's inexpensive spectrometer using AMS' AS7265X Spectral Senor Suite. Code adapted from
// his repository, found at https://github.com/kriswiner/AS7265X
//
// Created by Alec Vercruysse on 10/12/18.
//
//#include <Wire.h>
#include "AS7265X.h"
#include "AS7265X.cpp"

#define intPin 3
bool intFlag = false;
uint8_t MAJOR, PATCH, BUILD, status;
float calData[18];
int16_t rawData[18];
uint32_t spectrometer_run = 0;
uint16_t freq[18] = {610, 680, 730, 760, 810, 860, 560, 585, 645, 705, 900, 940, 410, 435, 460, 485, 510, 535}; // latest data sheet
uint8_t ledIndCurrent0 = led_ind_1_mA, ledDrvCurrent0 = led_drv_12_5_mA;
uint8_t ledIndCurrent1 = led_ind_1_mA, ledDrvCurrent1 = led_drv_12_5_mA;
uint8_t ledIndCurrent2 = led_ind_1_mA, ledDrvCurrent2 = led_drv_12_5_mA;
uint8_t gain = gain_16x, mode = mode2, intTime = 36;
AS7265X AS7265X(intPin);

/*
 * spectrometer method (AV individual experiment)
 * Callback function for when INT of spectrometer is low
 */
void myIntHandler ()
{
    intFlag = true;
}

void setupSpectrometer() {
    //Wire.begin(); //start i2c master mode. Unused because Wire.begin() is already called for the arduino.
    //Wire.setClock(400000); // I2C frequency at 400 kHz. Best not to use because other devices are also on the i2c bus
    delay(1000);
    pinMode(intPin, INPUT_PULLUP);
    AS7265X.I2Cscan();
    Serial.println("I2C scan done");
    AS7265X.init(gain, mode, intTime);
    //confirm device is working:
    byte c = AS7265X.getDevType();
    Serial.print("AS72651 "); Serial.print("Device Type = 0x"); Serial.print(c, HEX);  Serial.println(" should be 0x40");
    Serial.println(" ");
    byte d = AS7265X.getHWVersion();
    Serial.print("AS72651 "); Serial.print("HW Version = 0x"); Serial.print(d, HEX); Serial.println(" should be 0x41");
    Serial.println(" ");
    uint16_t e = AS7265X.getFWMajorVersion();
    Serial.print("AS72651 "); Serial.print("FW Major Version = 0x"); Serial.print(e, HEX);
    Serial.println(" ");
    uint16_t f = AS7265X.getFWPatchVersion();
    Serial.print("AS72651 "); Serial.print("FW Patch Version = 0x"); Serial.print(f, HEX);
    Serial.println(" ");
    uint16_t g = AS7265X.getFWBuildVersion();
    Serial.print("AS72651 "); Serial.print("FW Build Version = 0x"); Serial.print(g, HEX);
    Serial.println(" ");
    delay(1000);
    //Configure leds, for devices 0 (master), 1 and 2 (slaves)
    AS7265X.configureLed(ledIndCurrent0, ledDrvCurrent0, 0);
    AS7265X.disableIndLed(0);
    AS7265X.disableDrvLed(0);
    delay(100);
    AS7265X.configureLed(ledIndCurrent1, ledDrvCurrent1, 1);
    AS7265X.disableIndLed(1);
    //AS7265X.enableDrvLed(1);
    AS7265X.disableDrvLed(1); // turn off all LEDs, we're looking at the skys
    delay(100);
    AS7265X.configureLed(ledIndCurrent2, ledDrvCurrent2, 2);
    AS7265X.disableIndLed(2);
    AS7265X.disableDrvLed(2);
    delay(100);
    Serial.print("attaching interrupt to myIntHandler()...");
    attachInterrupt(digitalPinToInterrupt(intPin), myIntHandler, FALLING);
    Serial.println("done");
}

String checkSpectrometer() {
    String toReturn;
    if (digitalRead(intPin) == LOW) {//if (intFlag) {
        spectrometer_run++;
        intFlag = false;
        status = AS7265X.getStatus();
        if (status & 0x02) {
            AS7265X.readCalData(calData);
            toReturn.concat(spectrometer_run); toReturn.concat(">");
            for (int i = 0; i < 18; i++) {
                //Serial.prin
                // t(spectrometer_run); Serial.print(","); Serial.print(freq[i]); Serial.print(","); Serial.println(calData[i]);
                toReturn.concat(freq[i]); toReturn.concat(":"); toReturn.concat(calData[i]);
            }
            //Serial.println(" ");
            //toReturn.concat(",");
        }
    }
    return toReturn;
}

