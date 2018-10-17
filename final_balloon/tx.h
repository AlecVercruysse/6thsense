#define shutdownpin 22

void setupTx() {
    pinMode(shutdownpin, OUTPUT);
    digitalWrite(shutdownpin, HIGH);
    Serial1.begin(9600);
}

void tBA(String n, byte *arr) {
    for (int i = 0; i < n.length(); i++) {
        byte b = int(n.charAt(i));
        arr[i] = b;
    }
}

void txArr(byte *buffer, int size) {
    for (int i = 0; i < size; i++) {
        Serial.write(buffer[i]);
        Serial1.write(buffer[i]);
    }
}

void sendDataAsBytes(String data[], int num_items) {
    byte start_sequence[] = {42, 42, 42, 42, 42, 42};
    byte next_data_sequence[] = {45, 45, 45, 45, 45, 45};

    txArr(start_sequence, 6);
    for(int i = 0; i < num_items; i++) {
        delay(100);
        byte value[data[i].length()];
        tBA(data[i], value);
        txArr(value, data[i].length());
        if (i < num_items - 1) {
            delay(100);
            txArr(next_data_sequence, 6);
        }
    }
    Serial.println(" ");
}
