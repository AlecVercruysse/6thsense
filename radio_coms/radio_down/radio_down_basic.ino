#include <Arduino.h>

int shutdownpin = 22;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
}

void loop() {
    while (true) {
        for (int i = 0; i < 255; i++) {
            Serial.write(i);
            Serial1.write(i);
            delay(200);
        }
    }
}