#include <Arduino.h>
#include "../../../../../../usr/share/arduino-1.8.7/hardware/arduino/avr/cores/arduino/HardwareSerial.h"
#include <QueueArray.h>

int shutdownpin = 22;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
    Serial.write('\aa');
}

int last6[6] = {1, 2, 3, 4, 5, 6};
int idx = 0;

byte[] current
void loop() {
    int incoming_data = Serial1.read();
    last6[idx] = incoming_data;
    i = (i + 1) % 6;

    /**if (incoming_data != -1) {
        Serial.println(incoming_data);
    }
    /**/

}