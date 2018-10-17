#include <Wire.h>
#include "spectrometer/spectrometer.h"
#include "tx.h"
#include "sd_logger.h"
#include "sensors.h"

/*
 * Index | data item
 * 0 | Time
 * 1 | GPS
 * 2 | Pressure
 * 3 | Temperature (C)
 * 4 | Spectrometer data
 */
#define numObs 5
String data_arr[numObs];

void setup() {
  
  //start serial comms with computer
  Serial.begin(9200);
  delay(4000);

  //setup tx and SD card logger;
  setupTx();
  setupLogger();

  // setup clock and the temp and GPS sensors
  setupSensors();

  //spectrometer setup (AV individual experiment)
  //setupSpectrometer();
  
}

void loop() {

  data_arr[0] = getClock();

  data_arr[1] = getGPS();

  data_arr[2] = getPressure();

  data_arr[3] = getTemp();

  data_arr[4] = "spectrometer placeholder";//checkSpectrometer();

  sendDataAsBytes(data_arr, numObs);
  logDataToSD(data_arr, numObs);
}
