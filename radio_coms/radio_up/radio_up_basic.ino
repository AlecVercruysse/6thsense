#include <Arduino.h>
#include "../../../../../../usr/share/arduino-1.8.7/hardware/arduino/avr/cores/arduino/HardwareSerial.h"

int shutdownpin = 22;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
}

void loop() {
    int incoming_data = Serial1.read();
    if (incoming_data != -1) {
        Serial.println(incoming_data);
    }
}