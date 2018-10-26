/**
 * code for most individual experiments
**/

#ifndef INC_6THSENSE_ANALOG_INDIVIDUALS_H
#define INC_6THSENSE_ANALOG_INDIVIDUALS_H

#define uv_pin A7

void setupIndividuals() {
    pinMode(uv_pin, INPUT);
}

String getUV() {
    return String(analogRead(uv_pin));
}


#endif //INC_6THSENSE_ANALOG_INDIVIDUALS_H
