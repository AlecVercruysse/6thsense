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

RTC_DS1307 rtc;

void setupSensors()
{
  // const int chipSelect = 53;
  
  //Serial.begin(9600); Serial already initiated in main sketch
  gpsSerial.begin(9600);
  rtc.begin();

  // Serial.print("Initializing SD card...");
  // // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // // Note that even if it's not used as the CS pin, the hardware SS pin 
  // // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // // or the SD library functions will not work. 
  //  pinMode(chipSelect, OUTPUT);
 
  // if (!SD.begin(chipSelect)) {
  //   Serial.println("initialization failed!");
  //   return;
  // }
  // Serial.println("initialization done.");
 
  // // open the file. note that only one file can be open at a time,
  // // so you have to close this one before opening another.
  // myFile = SD.open("logger.txt", FILE_WRITE);
 
  // // if the file opened okay, write to it:
  // if (myFile) {
  //   Serial.print("Writing to logger.txt...");
  //   myFile.println("Time, Temp, Longitude, Latitude,");
  // // close the file:
  //   myFile.close();
  //   Serial.println("done.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening the logger.txt");
  // }
}

//void runLogger()
//{
//  static int i = 0;
//  if (gpsSerial.available())
//  {
//    char ch = gpsSerial.read();
//    if (ch != '\n' && i < sentenceSize)
//    {
//      sentence[i] = ch;
//      i++;
//    }
//    else
//    {
//     sentence[i] = '\0';
//     i = 0;
//    }
//  }
//}

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
  static int i = 0;
  if (gpsSerial.available())
  {
    char ch = gpsSerial.read();
    while (ch != '\n' && i < sentenceSize)
    {
      char ch = gpsSerial.read();
      sentence[i] = ch;
      i++;
    }
    sentence[i] = '\0';
    i = 0;
    char field[20];
    getField(field, 0);
    if (strcmp(field, "$GPRMC") == 0)
    {
      myFile.print("Lat: ");
      getField(field, 3);  // number
      String lat_num = (String) field;
      getField(field, 4); // N/S
      String lat_direction = (String) field;

      myFile.print(" Long: ");
      getField(field, 5);  // number
      String long_num = (String) field;
      getField(field, 6);  // E/W
      String long_direction = (String) field;

      return String("Lat: " + lat_num + lat_direction + " Long: " + long_num + long_direction);
    }
  }
  else
  {
    Serial.println("No GPS Connection");
    return String("no gps");
  }
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    // myFile = SD.open("logger.txt", FILE_WRITE);
   
    // // if the file opened okay, write to it:
    // if (myFile) {
    //   Serial.print("Writing to logger.txt...");
      
    //   myFile.print("Lat: ");
    //   getField(field, 3);  // number
    //   myFile.print(field);
    //   getField(field, 4); // N/S
    //   myFile.print(field);
      
    //   myFile.print(" Long: ");
    //   getField(field, 5);  // number
    //   myFile.print(field);
    //   getField(field, 6);  // E/W
    //   myFile.println(field);

    //   //displayTemp();
      
    //   // close the file:
    //   myFile.close();
    //   Serial.println("done with logging gps on sd card.");
    // } else {
    //   // if the file didn't open, print an error:
    //   Serial.println("error opening logger.txt");
    // }
//    Serial.print("Lat: ");
//    getField(field, 3);  // number
//    Serial.print(field);
//    getField(field, 4); // N/S
//    Serial.print(field);
//    
//    Serial.print(" Long: ");
//    getField(field, 5);  // number
//    Serial.print(field);
//    getField(field, 6);  // E/W
//    Serial.println(field);
}

String getClock()
{
  DateTime now = rtc.now();
  String hour = String(now.hour(), DEC);
  String minute = String(now.minute(), DEC);
  String second = String(now.second(), DEC);
  String total = String(hour + ":" + minute + ":" + second);
  return total;

  // myFile = SD.open("logger.txt", FILE_WRITE);
   
  // // if the file opened okay, write to it:
  // if (myFile) {
  //   myFile.print(now.hour(), DEC);
  //   myFile.print(':');
  //   myFile.print(now.minute(), DEC);
  //   myFile.print(':');
  //   myFile.print(now.second(), DEC);
  //   myFile.print(', ');
  //   myFile.close();
  //   Serial.println("Done logging time!");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening logger.txt for clock");
  // }
}

String getTemp()
{
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Tf = (Tc * 9.0)/ 5.0 + 32.0; 

//  Serial.print(Tc);
//  Serial.print(" C,"); 
  return String(Tc);
  // myFile = SD.open("logger.txt", FILE_WRITE);
   
  // // if the file opened okay, write to it:
  // if (myFile) {
  //   myFile.print(Tc);
  //   myFile.print(","); 
  //   myFile.close();
  //     Serial.println("done with logging temp on sd card.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening logger.txt");
  // }
  
}

String getPressure()
{
  pressvalue = analogRead(pressure);
  return String(pressvalue);
}
