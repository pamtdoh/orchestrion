int analogA0 = A0;
//int ledPin = 8;
String stringOne = "Sensor Reading: ";

const int analogA1 = A1;
int ledPins[]={2,3,4,5,6,7,8,9};
int totPinNo = sizeof(ledPins)/sizeof(int);

void setup() {
 //pinMode(ledPin, OUTPUT);
 Serial.begin(9600);
 for(int thisLed = 0; thisLed < totPinNo; thisLed++){
    pinMode(ledPins[thisLed], OUTPUT);
 }
}

void loop() {
  int sensorReadingA0 = analogRead(analogA0);
  Serial.println(stringOne + sensorReadingA0);

  int sensorReadingA1 = analogRead(analogA1);
  int ledLevel = map(sensorReadingA1, 0, 1023, 0, totPinNo);

  if (sensorReadingA0 > 20) {
    for(int thisLed = 0; thisLed < totPinNo; thisLed++){
      if (thisLed < ledLevel){
        digitalWrite(ledPins[thisLed], HIGH);
      }

      else{
        digitalWrite(ledPins[thisLed], LOW);
      }
    }
  }
  else
  {
    for(int thisLed = 0; thisLed < totPinNo; thisLed++){
      digitalWrite(ledPins[thisLed], LOW);      
    }
  }
}
