//#include <Wire.h> not needed: included elsewhere, in BME.cpp
#include "humidity.h"
#include "spectrometer/spectrometer.h"
#include "tx.h"
#include "sd_logger.h"
#include "sensors.h"
//#include "accelerometer/accelerometer.h"

/*
 * Index | data item
 * 0 | Time
 * 1 | GPS
 * 2 | Altitude 
 * 3 | Pressure
 * 4 | Outside Temperature (C)
 * 5 | Humidity (%)
 * 6 | Inside Payload temperature (C)
 * 7 | Spectrometer data
 */
#define numObs 8
String data_arr[numObs];
const double PRESSURE_TO_CUT_DOWN = 108;
double pressure_value = 100.0;
String pressure_string = "";
int cut_down_pin = 30;
String start_time = "8:32:15";
String current_time = "8:32:15";
double seconds_passed_to_cutdown = 7200.0;
String current_temp = "21.7";
double temp_to_cutdown = -10.3;
int num_times_cutdown_criteria_met = 0;

void setup() {
  
  //start serial comms with computer
  Serial.begin(9200);
  delay(4000);

  num_times_cutdown_criteria_met = 0;
  
  pinMode(cut_down_pin, OUTPUT);

  //setup tx and SD card logger;
  setupTx();
  setupLogger();

  // setup clock and the temp and GPS sensors and BME
  setupBME();
  setupSensors();

  //spectrometer setup (AV individual experiment)
  setupSpectrometer();

//  setupAccel();

  start_time = getClock();
  
}

void loop() {

  current_time = getClock();
  data_arr[0] = current_time;

  data_arr[1] = getGPS();

  data_arr[2] = getAltitude();

  pressure_string = getPressure();
  data_arr[3] = pressure_string;
  pressure_value = pressure_string.toDouble();

  String currentTemp = getBMETemp();
  data_arr[4] = getBMETemp();

  data_arr[5] = getBMEHumidity();

  data_arr[6] = getTemp();

  data_arr[7] = checkSpectrometer();

//  data_arr[8] = getAccel();

  sendDataAsBytes(data_arr, numObs);
  logDataToSD(data_arr, numObs);

  //change this to altitude later TODO: only cut down if the last couple measurements (e.g. 10+) are @threshold
  if (getTimeDifference(current_time, start_time) > seconds_passed_to_cutdown)
  {
    if (current_temp.toDouble() < temp_to_cutdown)
    {
        if (pressure_value < PRESSURE_TO_CUT_DOWN)
        {
          num_times_cutdown_criteria_met += 1;
          if (num_times_cutdown_criteria_met > 10)
          {
            digitalWrite(cut_down_pin, HIGH);
          }
        }
    }
  }
}

int getTimeDifference(String time1, String time2)
{
  return (getTimeInSeconds(time1) - getTimeInSeconds(time2));
}

double getTimeInSeconds(String theTime)
{
  int firstColon = theTime.indexOf(':');
  String hours = theTime.substring(0, firstColon);
  int secondColon = theTime.indexOf(':', firstColon + 1);
  String minutes = theTime.substring(firstColon + 1, secondColon);
  String seconds = theTime.substring(secondColon + 1);

  double hourSeconds = hours.toDouble() * 3600;
  double minuteSeconds = minutes.toDouble() * 60;

  return hourSeconds + minuteSeconds + seconds.toDouble();
}
