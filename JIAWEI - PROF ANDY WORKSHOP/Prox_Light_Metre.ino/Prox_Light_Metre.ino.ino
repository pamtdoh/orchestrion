int trigPin = 13;
int echoPin = 12;
float flightTime_microsec, flightTime_sec;
float dist_cm;
float soundspeed_cmps;
float soundspeed_mps = 343.5;

const int analogPin = A1;
int ledPins[]={2,3,4,5,6,7,8,9};
int totPinNo = sizeof(ledPins)/sizeof(int);

void setup(){
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  for(int thisLed = 0; thisLed < totPinNo; thisLed++){
    pinMode(ledPins[thisLed], OUTPUT);
  }
}


void loop(){
  //int sensorReading = analogRead(analogPin);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  flightTime_microsec = pulseIn(echoPin, HIGH);
  flightTime_sec = flightTime_microsec*pow(10,-6);

  soundspeed_cmps = soundspeed_mps*100;
  dist_cm = soundspeed_cmps*(flightTime_sec/2);

  int ledLevel = map(dist_cm, 0, 10, 0, totPinNo);
  
  Serial.println(dist_cm);
  delay(10);
  
  for(int thisLed = 0; thisLed < totPinNo; thisLed++){
    if (thisLed < ledLevel){
      digitalWrite(ledPins[thisLed], HIGH);
    }

    else{
      digitalWrite(ledPins[thisLed], LOW);
    }
  }
}
