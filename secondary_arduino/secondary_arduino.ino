#include <SoftwareSerial.h>
#include <elapsedMillis.h>
#include <Wire.h>
#include <RTClib.h>

SoftwareSerial OpenLog(4, 5);
elapsedMillis timeElapsed;
int count;

void setup() {
  Serial1.begin(9600);
  OpenLog.begin(9600);
  Wire.begin();
  delay(500);
  count = 0;
  timeElapsed = 0;
}


void loop() {
  if (Serial1.available() > 0) {
    if (Serial1.read() > -1) {
      count++;
    }
  }
  if (timeElapsed > 10000) {
    OpenLog.print("UTS: ");
    OpenLog.print(RTC.now().unixtime());
    OpenLog.print("CPM: ");
    OpenLog.println(count*6);
    timeElapsed = 0;
    count = 0;
  }
}
