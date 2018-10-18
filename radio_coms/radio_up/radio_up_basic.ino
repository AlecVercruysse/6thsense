#include <Arduino.h>

int shutdownpin = 22;

String line;
char incoming_data;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
}

void loop() {
    /**
    incoming_data = Serial1.read();
    do {
        if (incoming_data != -1) {
            line.concat(incoming_data);
        }
    } while (!(line.length() > 6 && line.charAt(line.length() - 1) == '*'));
    Serial.println(line);
    line = "";
    /**/
    char inc = Serial1.read();
    if (inc != -1){
      Serial.print(inc);
    }
    /**/
}
