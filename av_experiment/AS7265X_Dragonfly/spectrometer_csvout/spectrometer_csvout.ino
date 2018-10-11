#include <Wire.h>
#include "AS7265X.h"

#define intPin 3

bool intFlag = false;
uint8_t MAJOR, PATCH, BUILD, status;
float calData[18];
int16_t rawData[18];
uint16_t freq[18] = {610, 680, 730, 760, 810, 860, 560, 585, 645, 705, 900, 940, 410, 435, 460, 485, 510, 535}; // latest data sheet

uint8_t ledIndCurrent0 = led_ind_1_mA, ledDrvCurrent0 = led_drv_12_5_mA;
uint8_t ledIndCurrent1 = led_ind_1_mA, ledDrvCurrent1 = led_drv_12_5_mA;
uint8_t ledIndCurrent2 = led_ind_1_mA, ledDrvCurrent2 = led_drv_12_5_mA;

uint8_t gain = gain_16x, mode = mode2, intTime = 36;

AS7265X AS7265X(intPin);

void setup() {

  Serial.begin(115200);
  delay(4000);

  Wire.begin(/*TWI_PINS_20_21*/); // set master mode
  Wire.setClock(400000);      // I2C frequency at 400 kHz
  delay(1000);

  pinMode(intPin, INPUT); // set up the interrupt pin

  AS7265X.I2Cscan();
  Serial.println("I2C scan done");

  AS7265X.init(gain, mode, intTime);

  // get some information about the device hardware and firmware
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
  AS7265X.enableDrvLed(1);
  delay(100);
  AS7265X.configureLed(ledIndCurrent2, ledDrvCurrent2, 2);
  AS7265X.disableIndLed(2);
  AS7265X.disableDrvLed(2);
  delay(100);

  Serial.print("attaching interrupt to myIntHandler()...");
  attachInterrupt(intPin, myIntHandler, FALLING);
  Serial.println("done");
  Serial.println("status:" + AS7265X.getStatus());
}

int run = 0;
void loop() {

  if (intFlag)
  {
    run++;
    intFlag = false;
    status = AS7265X.getStatus();
    if (status & 0x02)
    {
      /*
        AS7265X.readRawData(rawData);
        for(int i = 0; i < 18; i++)
        {
         Serial.println(rawData[i]);
        }
      */
      AS7265X.readCalData(calData);
      for (int i = 0; i < 18; i++)
      {
        Serial.print(run); Serial.print(","); Serial.print(freq[i]); Serial.print(","); Serial.println(calData[i]);
      }
      Serial.println(" ");

      /*
       for (int i = 0; i < 3; i++)
      {
        Serial.print("Temperature of device "); Serial.print(i); Serial.print (" is "); Serial.print(AS7265X.getTemperature(i), 0); Serial.println(" C");
      }
      Serial.println(" ");
      */

      AS7265X.enableIndLed(0); delay(10); AS7265X.disableIndLed(0); // blink indicator led
    }

    delay(1000);
  }

}

void myIntHandler ()
{
  intFlag = true;
}
