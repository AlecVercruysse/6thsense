#include <Wire.h>
#include "spectrometer/spectrometer.h"
#include "tx.h"

//amount of separate observations we're sending down
/*
 * Index | data item
 * 0 | GPS Lat
 * 1 | GPS Long
 * 2 | Spectrometer data
 */
String data_arr[3];

void setup() {

  //start serial comms with computer
  Serial.begin(9200);
  delay(4000);

  //setup tx;
  setupTx();

  //spectrometer setup (AV individual experiment)
  setupSpectrometer();
  
}

void loop() {

  //get gps data and add it to the data_arr
  //TODO: luke

  //spectrometer code (AV individual experiment)
  data_arr[2] = checkSpectrometer();
}
