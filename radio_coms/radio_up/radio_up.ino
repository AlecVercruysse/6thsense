//#include <Arduino.h>
//#include "../../../../../../usr/share/arduino-1.8.7/hardware/arduino/avr/cores/arduino/HardwareSerial.h"

/**
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
int shutdownpin = 22;
int rssipin = A0;

void setup() {
    pinMode(shutdownpin, OUTPUT);
    pinMode(rssipin, INPUT);
    digitalWrite(shutdownpin, HIGH);

    Serial.begin(9600); //9600 Baud communication with computer
    Serial1.begin(9600);//9600 Baud communication with RF module

    //establish connection:
    /**
    Serial.println("sending hello-msg \"6thsense-base\"...");
    Serial1.println("6thsense-base");
    Serial.println("sent.");
    Serial.println("waiting for response...");
    while (Serial1.available() == 0) {
        continue;
    }
    char response[100];
    Serial1.readBytes(response, 100);
    Serial.println("Response Recieved!\nResponse: ");
    Serial.println(response);
    Serial.print("getting transmissions...");
     /**

     Serial.println("Searching for 6thsense-balloon...");
     while(Serial1.available() < 1) {
         Serial.println("Searching...");
         delay(5000);
     }
     int responseSize = Serial1.available();
     char response[responseSize];
     Serial1.readBytes(response, responseSize);
     Serial.println("response received of length" + responseSize);
     Serial.println(response);
}

void loop() {
    // put your main code here, to run repeatedly:
    while (Serial1.available() == 0) {
        continue;
    }
    char imgsize[100];
    Serial1.readBytes(imgsize, 100);
    String imgsize_s = String(imgsize);
    imgsize_s = imgsize_s.substring(imgsize_s.indexOf('=') + 1);
    int imgSize_i = imgsize_s.toInt();
    Serial.println("imgSize= " + imgSize_i);
    char img[imgSize_i];
    while (Serial1.available() == 0) {
        continue;
    }
    readBytesUntilNext(img, imgSize_i);
    Serial.println("img= ");
    Serial.println(img);
}
/**/
/**
 * Kind of my own implementation of Serial.readBytesUntil, where the terminator is 'stop'
 * Will fill buffer and return when the stop is reached (stop not included) or buffer is full.
 * If Serial.readBytes() times out, it simply skips and goes to the next. Ensure buffer isn't too large.
 * Condition: buffer must be > 4.
 */
 /**
void readBytesUntilNext(char *buffer, int size) {
    char latest4[4];
    char n[1];
    char stop[4] = {'s', 't', 'o', 'p'};
    Serial1.readBytes(latest4, 4);
    if (strncmp(latest4, stop, 4) == 0) {
        return  buffer;
    }
    for (int i = 4; i < size + 4; i++) {
        buffer[i - 4] = latest4[i % 4];
        if (i < size) {
            Serial1.readBytes(n, 1);
            latest4[i % 4] = n[0];
        }
        if (latest4[i%4] == 's' && latest4[(i+1)%4] == 't' && latest4[(i+2)%4] == 'o' && latest4[(i+3)%4] == 'p') {
            return buffer;
        }
    }
    return buffer;
}
/**/