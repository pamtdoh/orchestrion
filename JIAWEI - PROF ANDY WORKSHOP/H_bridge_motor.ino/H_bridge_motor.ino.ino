void setup() {
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  delayMicroseconds(5000);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);  
  delayMicroseconds(5000);
}
