const int analogPin = A1;

int ledPins[]={
  2,3,4,5,6,7,8,9
};

int totPinNo = sizeof(ledPins)/sizeof(int);

void setup(){
  for(int thisLed = 0; thisLed < totPinNo; thisLed++){
    pinMode(ledPins[thisLed], OUTPUT);
  }
}


void loop(){
  int sensorReading = analogRead(analogPin);
  int ledLevel = map(sensorReading, 0, 1023, 0, totPinNo);

  for(int thisLed = 0; thisLed < totPinNo; thisLed++){
    if (thisLed < ledLevel){
      digitalWrite(ledPins[thisLed], HIGH);
    }
    else{
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
}
