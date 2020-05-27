int analogPin = A0;
int ledPin = 8;
String stringOne = "Sensor Reading: ";

void setup() {
 pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
}

void loop() {
  int sensorReading = analogRead(analogPin);
  Serial.println(stringOne + sensorReading);

  if (sensorReading > 0) {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}
