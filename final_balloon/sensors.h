#include <SoftwareSerial.h>

#include <Wire.h>
#include "RTClib.h"
 

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
int pressvalue = 0;
int pressunits = 0;

RTC_DS1307 rtc;

void setupSensors()
{
  gpsSerial.begin(9600);
  rtc.begin();
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

/**
 * Todo: doesn't return if no gps data available. Need to implement a timeout or something.
 */
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
        gotSentence = true;
        sentence[i] = '\0';
        i = 0;
        char field[20];
        getField(field, 0);
        if (strcmp(field, "$GPRMC") == 0)
        {
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
        else
        {
          return String("GPS error :(");
        }
      }
    }
  }
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
  pressvalue = analogRead(pressure);
  pressunits = 0.2544 * (pressvalue) - 26.24;
  return String(pressunits);
}
