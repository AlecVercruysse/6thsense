#include "tx_geiger.h"
#include "sd_logger_geiger.h"
#include "sensors_geiger.h"
#include <SoftwareSerial.h>
#include "RTClib/RTClib.h"
#include "RTClib/RTClib.cpp"

/*
 * Index | data item
 * 0 | Time
 * 1 | Geiger
 */
#define numObs 2
String data_arr[numObs];

SoftwareSerial OpenLog(4, 5);
RTC_DS1307 rtc;


void setup() {
  Serial1.begin(9600);
  OpenLog.begin(9600);
  setupSensors();
  delay(500);
}


void loop() {

  data_arr[0] = getClock();

  data_arr[1] = getGeiger();

  sendDataAsBytes(data_arr, numObs);
  logDataToSD(data_arr, numObs);
}
