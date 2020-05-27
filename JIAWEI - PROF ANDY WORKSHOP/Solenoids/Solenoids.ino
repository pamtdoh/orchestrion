void setup() {
    pinMode(1,OUTPUT);
    digitalWrite(1, LOW);

}

void loop() {

    digitalWrite(1, HIGH);
    delay(1000);
    digitalWrite(1, LOW);
    delay(1000);

}
