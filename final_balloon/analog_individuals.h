/**
 * code for Vivian's, Caroline's, and John's individual experiments,
 * which involve reading analog input.
**/

#ifndef INC_6THSENSE_ANALOG_INDIVIDUALS_H
#define INC_6THSENSE_ANALOG_INDIVIDUALS_H

#define uv_pin A7
#define methane_pin A5
#define solar_pin A3
#define light_intensity_pin A4

void setupIndividuals() {
    pinMode(uv_pin, INPUT);
    pinMode(methane_pin, INPUT);
    pinMode(solar_pin, INPUT);
    pinMode(light_intensity_pin, INPUT);
}

String getUV() {
    return String(analogRead(uv_pin));
}

String getMethane() {
    return String(analogRead(methane_pin));
}

String getSolar() {
    return String(analogRead(solar_pin) * 5.0/1024.0);
}

String getLightIntensity() {
    float voltage = analogRead(light_intensity_pin) * 5.0/102.0;
    return String(voltage);
}
#endif //INC_6THSENSE_ANALOG_INDIVIDUALS_H
