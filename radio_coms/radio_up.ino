void setup() {
  // put your setup code here, to run once:
  int shutdownpin = 22;

  pinMode(shutdownpin, OUTPUT);
  digitalWrite(shutdownpin, HIGH);

  Serial.begin(9600); //9600 Baud communication with computer
  Serial1.begin(9600);//9600 Baud communication with RF module
}

void loop() {
  // put your main code here, to run repeatedly:
  char response[1];
  while (Serial1.available() > 0) {
    Serial1.readBytes(response, 1);
    Serial.println(response);
  }


}
