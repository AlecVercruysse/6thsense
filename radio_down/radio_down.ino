#include <Arduino.h>
/*
 * Code for the Digi 9XTend OEM RF module connected to an arduino mega
 *
 * 9XTend manual: https://www.sparkfun.com/datasheets/Wireless/Zigbee/xtend-productmanual.pdf
 *
 * Using mega pins 18 (rx) and 19 (tx) for UART communication (Serial1 in code).
 * Currently using default serial packet protocol (8 bits, no parity, 1 stop bit), but consider changing
 *
 * Written by Alec Vercruysse
 *
 */

final int setupmode = true //normal operation requires setupmode to be false


void setup() {
    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module

    if (setupmode) {
        Serial.println("Entering command mode with string: \"+++\"")
        Serial1.write("+++");// "+++" enters command mode
        Serial.println("System response: " + Serial1.readString())

        Serial.println("Entering command mode with string: \"+++\"")
        Serial1.write("+++");// "+++" enters command mode
        Serial.println("System response: " + Serial1.readString())
    }

}

void loop() {
    if (!setupmode) {
        //main loop:
    }
}