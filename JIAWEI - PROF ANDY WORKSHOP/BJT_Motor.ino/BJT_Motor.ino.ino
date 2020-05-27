void setup() {
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  analogWrite(7, 128);
  analogWrite(8, 128);
}
