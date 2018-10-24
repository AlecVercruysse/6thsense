#include <SoftwareSerial.h>
#include <SD.h>

SoftwareSerial gpsSerial(10, 11); // RX, TX (TX not used)
const int sentenceSize = 80;

char sentence[sentenceSize];

void setupGPS() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
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
          
          String gps = String("Lat: " + lat_num + lat_direction + " Long: " + long_num + long_direction);
          return gps;
        }
      }
    }
  }
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
