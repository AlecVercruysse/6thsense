/**
 * Code for the local microSD card logger. Sets up all csv headers and includes
 * methods to log a data array. Final file should look like a csv for easy processing.
 * Note that a file called "logger.txt" needs to be present in the root directory
 * of the SD card for this code to work.
 */

#ifndef INC_6THSENSE_SD_LOGGER_H
#define INC_6THSENSE_SD_LOGGER_H

#include <SD.h>
File myFile;

void setupLogger() {
    const int chipSelect = 53;
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the SD library functions will not work.
    pinMode(chipSelect, OUTPUT);

    if (!SD.begin(chipSelect)) {
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("SD init done.");

    myFile = SD.open("logger.txt", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile) {
        Serial.print("Writing headers to logger.txt...");

        //column heads
        myFile.println("Time, GPS, Pressure, Outside_temp, Humidity, Inside_temp, Spectrometer, UVB, Methane, Solar");
        // close the file:
        myFile.close();
        Serial.println("done.");
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening the logger.txt");
    }
}

void logDataToSD(String data[], int num_items) {
    myFile = SD.open("logger.txt", FILE_WRITE);
    if (myFile) {
        for (int i = 0; i < num_items; i++) {
            myFile.print(data[i]);
            if (i < num_items - 1) {
                myFile.print(", ");
            }
        }
        myFile.println(" ");
        myFile.close();
    } else {
        Serial.println("error opening logger.txt");
    }
}

#endif //INC_6THSENSE_SD_LOGGER_H
