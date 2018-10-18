#include <Wire.h>
#include "spectrometer/spectrometer.h"
#include "tx.h"
#include "sd_logger.h"
#include "sensors.h"
#include "humidity.h"

/*
 * Index | data item
 * 0 | Time
 * 1 | GPS
 * 2 | Pressure
 * 3 | Temperature (C)
 * 4 | Spectrometer data
 */
#define numObs 7
String data_arr[numObs];
const double PRESSURE_TO_CUT_DOWN = 93.675;
double pressure_value = 100.0;
String pressure_string = "";
int cut_down_pin = 30;

void setup() {
  
  //start serial comms with computer
  Serial.begin(9200);
  delay(4000);
  
  pinMode(cut_down_pin, OUTPUT);

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

  pressure_string = getPressure();
  data_arr[2] = pressure_string;
  pressure_value = pressure_string.toDouble();

  data_arr[3] = getBMETemp();

  data_arr[4] = getBMEHumidity();

  data_arr[5] = getTemp();

  data_arr[6] = "spectrometer placeholder";//checkSpectrometer();

  sendDataAsBytes(data_arr, numObs);
  logDataToSD(data_arr, numObs);

  //change this to altitude later
  if (pressure_value < PRESSURE_TO_CUT_DOWN)
  {
    digitalWrite(cut_down_pin, HIGH);
  }
}
