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
    delay(500);
    count = 0;
    accelCount = 0;
    timeElapsed = 0;
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
}


void loop() {
//    if (Serial.available() > 0) {
//      if (Serial.read() > -1) {
//        count++;
//      }
//    }
    if (timeElapsed > 10000) {
        OpenLog.print("Millis: ");
        OpenLog.println(millis());
//        OpenLog.print("CPM: ");
//        OpenLog.println(count * 6);
        timeElapsed = 0;
//        count = 0;
    } 
    mpu6050.update();
    accel_arr[accelCount] = getTheAccel();
    accelCount ++;
    if (accelCount == numAccelCounts - 1)
    {
      accelCount = 0;
      for (int i = 0; i < numAccelCounts; i ++)
      {
        String the_accel_data = accel_arr[i];
        if (the_accel_data.length() > 7)
        {
          OpenLog.print(the_accel_data);
          OpenLog.print(",\n"); 
        }
      }
    }
}

String getTheAccel()
{
  String accx = String(mpu6050.getAccX());
  //Serial.print(total); Serial.print(", ");
  String accy = String(mpu6050.getAccY());
  //Serial.print(accy); Serial.print(", ");
  String accz = String(mpu6050.getAccZ());
  //Serial.println(accz);
  //total.concat(", ");
  //total.concat(accy); total.concat(", ");
  //total.concat(accz); total.concat("\n");
  //return total;
  return String(accx + "," + accy + "," + accz);
//  return String(mpu6050.getGyroZ() + mpu6050.getGyroY() + mpu6050.getGyroX());
}
