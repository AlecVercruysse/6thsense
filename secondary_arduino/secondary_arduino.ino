#include <SoftwareSerial.h>
#include "elapsedMillis/elapsedMillis.h"
#include <MPU6050_tockn.h>
#include <Wire.h>

SoftwareSerial OpenLog(4, 5);
elapsedMillis timeElapsed;
int count;
int accelCount;
#define numAccelCounts 100
String accel_arr[numAccelCounts];
MPU6050 mpu6050(Wire);


void setup() {
    Serial.begin(9600);
    OpenLog.begin(9600);
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);
    delay(500);
    count = 0;
    accelCount = 0;
    timeElapsed = 0;
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
    //setupAccel();
    digitalWrite(13, HIGH);
}


void loop() {
    if (Serial.available() > 0) {
      if (Serial.read() > -1) {
        count++;
      }
    }
    if (timeElapsed > 10000) {
        OpenLog.print(millis());
        OpenLog.print("CPM: ");
        OpenLog.println(count * 6);
        timeElapsed = 0;
        count = 0;
    }
    mpu6050.update();
    accel_arr[accelCount] = getTheAccel();
    accelCount ++;
    if (accelCount >= numAccelCounts)
    {
      accelCount = 0;
      for (int i = 0; i < numAccelCounts; i ++)
      {
        OpenLog.print(accel_arr[i]);
        OpenLog.print("; ");
      }
      OpenLog.println();
    }
}

String getTheAccel()
{
  return String(mpu6050.getGyroZ() + mpu6050.getGyroY() + mpu6050.getGyroX());
}
