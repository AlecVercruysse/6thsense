#include <Arduino.h>
#include <math.h>       /* log10 */
#include <stdlib.h>     /* atoi */

int shutdownpin = 22;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
}

void tBA(String n, byte *arr) {
    for (int i = 0; i < n.length(); i++) {
        byte b = int(n.charAt(i));
        arr[i] = b;
    }
}

void txArr(byte *buffer, int size) {
    for (int i = 0; i < size; i++) {
        Serial.write(buffer[i]);
        Serial1.write(buffer[i]);
    }
}

void sendDataAsBytes() {
    byte start_sequence[] = {42, 42, 42, 42, 42, 42};
    byte next_data_sequence[] = {45, 45, 45, 45, 45, 45};

    //example data to send down:
    String gps_lat = "23425634N";
    String gps_long = "6586755E";
    byte lat_arr[gps_lat.length()];
    byte long_arr[gps_long.length()];
    tBA(gps_lat, lat_arr);
    tBA(gps_long, long_arr);

    //send sequence:
    txArr(start_sequence, 6);
    delay(100);
    txArr(lat_arr, gps_lat.length());
    delay(100);
    txArr(next_data_sequence, 6);
    delay(100);
    txArr(long_arr, gps_long.length());
    delay(100);
}

void loop() {
    /**
    //This code works, sending byte values
    while (true) {
        for (byte i = 0; i < 255; i++) {
            Serial.write(i);
            Serial1.write(i);
            delay(200);
        }
    }
     /**/
    sendDataAsBytes();
    delay(1000);
}

