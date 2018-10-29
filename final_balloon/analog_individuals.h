/**
 * code for most individual experiments
**/

#ifndef INC_6THSENSE_ANALOG_INDIVIDUALS_H
#define INC_6THSENSE_ANALOG_INDIVIDUALS_H

#define uv_pin A7
#define methane_pin A5
#define solar_pin A3

void setupIndividuals() {
    pinMode(uv_pin, INPUT);
    pinMode(methane_pin, INPUT);
    pinMode(solar_pin, INPUT);
}

String getUV() {
    return String(analogRead(uv_pin));
}

String getMethane() {
    return String(analogRead(methane_pin));
}

String getSolar() {
    return String(analogRead(solar_pin));
}

#endif //INC_6THSENSE_ANALOG_INDIVIDUALS_H
