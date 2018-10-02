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

int setupmode = false ; //normal operation requires setupmode to be false
int shutdownpin = 22;
int configpin = 24;
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);// for blinking internal LED
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module
    
    if (setupmode) {
        Serial.println("Entering command mode by asserting config pin low and pulsing power (1s):");
        digitalWrite(configpin, LOW);
        digitalWrite(shutdownpin, LOW);
        delay(2000);
        digitalWrite(shutdownpin, HIGH);
        Serial.println("Ok we should be in");
        Serial.println("Getting current Destination Address [ATDT <Enter>]. Response:");
        delay(100);
        char command[] = {'A','T','D','T','\r'};
        Serial1.write(command);
        char response[10];
        while (Serial1.available() > 0) {
          Serial1.readBytes(response, 10);
          Serial.println(response);
        }
        Serial.println("Done");
        /**/

        /**
        Serial.println("Entering command mode with string: \"+++\"")
        Serial1.write("+++");// "+++" enters command mode
        Serial.println("System response: " + Serial1.readString())
         /*TODO: edit with updated command sequence using above format*/
    }
}

void loop() {
    if (!setupmode) {
        //main loop:

        Serial.println("Hello World!");
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);

        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        Serial.println("Starting communication");
        for (int i = 0; i < 1000; i++){
          Serial1.write(i);
          delay(200);
        }
    }
}
