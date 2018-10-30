#include <SoftwareSerial.h>
#include "elapsedMillis/elapsedMillis.h"
#include "accelerometer/accelerometer.h"

SoftwareSerial OpenLog(4, 5);
elapsedMillis timeElapsed;
int count;
int accelCount;
#define numAccelCounts 100
String accel_arr[numAccelCounts];

void setup() {
    Serial.begin(9600);
    OpenLog.begin(9600);
    delay(500);
    count = 0;
    accelCount = 0;
    timeElapsed = 0;
}


void loop() {
    if (Serial.available() > 0) {
      if (Serial.read() > -1) {
        count++;
      }
    }
    if (timeElapsed > 10000) {
        OpenLog.print("UTS: ");
        OpenLog.print(timeElapsed);
        OpenLog.print("CPM: ");
        OpenLog.println(count * 6);
        timeElapsed = 0;
        count = 0;
    }
    accel_arr[accelCount] = getAccel();
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
