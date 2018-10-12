#include <Wire.h>
#include "spectrometer/spectrometer.h"
#include "tx.h"

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
  //spectrometer code (AC individual experiment)
  checkSpectrometer();
}
