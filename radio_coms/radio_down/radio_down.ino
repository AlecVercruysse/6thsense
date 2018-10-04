//#include <Arduino.h>
//#include "../../../../../../usr/share/arduino-1.8.7/hardware/arduino/avr/cores/arduino/HardwareSerial.h"

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
/**
int setupmode = false; //normal operation requires setupmode to be false. Setupmode currently does not work
int shutdownpin = 22;
int rssipin = 24; //also doubles as config pin

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);// for blinking internal LED
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module

    if (setupmode) {
        pinMode(rssipin, OUTPUT);
        Serial.println("Entering command mode by asserting config pin low and pulsing power (1s):");
        digitalWrite(rssipin, LOW);
        digitalWrite(shutdownpin, LOW);
        delay(2000);
        digitalWrite(shutdownpin, HIGH);
        Serial.println("Ok we should be in");
        Serial.println("Getting current Destination Address, Response:");
        char command[] = {'A','T','D','T','\r'};
        Serial1.write(command);
        char response[10];
        delay(100);
        while (Serial1.available() > 0) {
            Serial.println("response recieved!");
            Serial1.readBytes(response, 10);
            Serial.println(response);
        }
        Serial.println("Done");
    }
    else {
        //establish connection:
        bool connectionEstablished = false;
        while (!connectionEstablished) {
            Serial.println("sending hello-msg \"6thsense-balloon\"...");
            Serial1.write("6thsense-balloon");
            Serial.println("sent.");
            Serial.println("waiting for response...");
            while (Serial1.available() < 12) {
                delay(5000);
                Serial.println("No reply. Resending...");
                Serial1.write("6thsense-balloon");
            }
            int responseSize = Serial1.available();
            char response[responseSize + 1];
            Serial1.readBytes(response, responseSize);
            response[responseSize] = '\0';
            Serial.print("Response Recieved of size ");
            Serial.print(responseSize, HEX);
            Serial.println("!\nResponse: ");
            Serial.println(response);
            if (strncmp(response, "6thsense-base", 12) == 0) {
                connectionEstablished = true;
            } else {
                Serial.println("not 6thsense-base. Continuing search.");
            }
        }
        Serial.print("starting transmissions...");
    }
}

void loop() {
    if (!setupmode) {
        //main loop:
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("sending data...");

        int imgSize = 5;
        char img[] = {0x01, 0x11, 0xe8, 0xff};

        Serial1.write("imgsize=" + imgSize);
        Serial1.write("img=");
        Serial1.write(img);
        Serial1.write("stop");
    }
}
*/