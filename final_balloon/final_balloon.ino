#include <Wire.h>
#include "spectrometer/spectrometer.h"
#include "tx.h"
#include "sd_logger.h"
#include "logger.h"

/*
 * Index | data item
 * 0 | Time
 * 1 | GPS
 * 2 | Spectrometer data
 */
#define numObs 4
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
  setupSpectrometer();
  
}

void loop() {

  //get gps data and add it to the data_arr
  //TODO: luke
  data_arr[0] = getClock();
  data_arr[1] = getGPS();

//  data_arr[2] = getPressure();

  //spectrometer code (AV individual experiment)
  data_arr[2] = checkSpectrometer();

  data_arr[3] = getTemp();

  sendDataAsBytes(data_arr, numObs);
  logDataToSD(data_arr, numObs);
}
