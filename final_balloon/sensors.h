/**
 * GPS, RTC, and barometer methods
 */

#include <SoftwareSerial.h>

#include <Wire.h>
#include "RTClib/RTClib.h"
#include "RTClib/RTClib.cpp"

//copied from: http://www.doctormonk.com/2012/05/sparkfun-venus-gps-and-arduino.html
SoftwareSerial gpsSerial(10, 11); // RX, TX (TX not used)
const int sentenceSize = 80;

char sentence[sentenceSize];

int ThermistorPin = A1;
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

int pressure = A0;
double pressvalue = 0;
double pressunits = 0;
double altitude = 0;
const double PRESSURE_SEALEVEL = 101325;
const double PRESSURE_11K = 22629.5;
const double PRESSURE_20K = 5473.875;


RTC_DS1307 rtc; //i2c address 0x68?

void setupSensors()
{
  gpsSerial.begin(9600);

  // not needed. This method simply calls Wire.begin(), which is called by the BME sensor.
  //rtc.begin();
}

void getField(char* buffer, int index)
{
  int sentencePos = 0;
  int fieldPos = 0;
  int commaCount = 0;
  while (sentencePos < sentenceSize)
  {
    if (sentence[sentencePos] == ',')
    {
      commaCount ++;
      sentencePos ++;
    }
    if (commaCount == index)
    {
      buffer[fieldPos] = sentence[sentencePos];
      fieldPos ++;
    }
    sentencePos ++;
  }
  buffer[fieldPos] = '\0';
}


String getGPS()
{
  bool gotSentence = false;
  while(!gotSentence)
  {
    static int i = 0;
    if (gpsSerial.available())
    {
      char ch = gpsSerial.read();
      if (ch != '\n' && i < sentenceSize)
      {
        sentence[i] = ch;
        i++;
      }
      else
      {
        sentence[i] = '\0';
        i = 0;
        char field[20];
        getField(field, 0);
        if (strcmp(field, "$GPRMC") == 0)
        {
          gotSentence = true;
          getField(field, 3);  // number
          String lat_num = String(field);
          getField(field, 4); // N/S
          String lat_direction = String(field);
          
          getField(field, 5);  // number
          String long_num = String(field);
          getField(field, 6);  // E/W
          String long_direction = String(field);
          
          return String("Lat: " + lat_num + lat_direction + " Long: " + long_num + long_direction);
        }
      }
    }
  }
  Serial.println(gotSentence);
}

String getClock()
{
  DateTime now = rtc.now();
  String hour = String(now.hour(), DEC);
  String minute = String(now.minute(), DEC);
  String second = String(now.second(), DEC);
  String total = String(hour + ":" + minute + ":" + second);
  return total;
}

String getTemp()
{
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 
  return String(Tc);
}

String getPressure()
{
  //take the average of 10 readings
  int sumPressureVoltages = 0;
  for (int i = 0; i < 10; i++) {
    sumPressureVoltages += analogRead(pressure);
  }
  pressvalue = ((double) sumPressureVoltages) / 10;
  // gets pressure in kPa
  pressunits = (0.2554 * (pressvalue)) - 25.295;
  return String(pressunits);
}

//estimation
double getAltitude()
{
  double presskpa = getPressure().toDouble();
  double pressurePa = presskpa * 1000;
  if(PRESSURE_11K < pressurePa)
  {
    altitude = (1 - pow(pressurePa / PRESSURE_SEALEVEL, 1 / 5.255816)) * 44329;
  }
  else if(PRESSURE_11K > pressurePa && pressurePa > PRESSURE_20K)
  {
    altitude = 10999 - 6341.4 * log(pressurePa / (PRESSURE_SEALEVEL * 0.22361));
  }
  else
  {
    altitude = (pow(pressurePa / PRESSURE_SEALEVEL, 1 / -34.16319) - 0.988626) * 198903;
  }
  return altitude;
}
