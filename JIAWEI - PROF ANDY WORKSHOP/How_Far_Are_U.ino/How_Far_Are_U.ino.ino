int trigPin = 13;
int echoPin = 12;
float flightTime_microsec, flightTime_sec;
float dist_cm;
float soundspeed_cmps;
float soundspeed_mps = 343.5;

void setup(){
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}


void loop(){
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  flightTime_microsec = pulseIn(echoPin, HIGH);
  flightTime_sec = flightTime_microsec*pow(10,-6);

  soundspeed_cmps = soundspeed_mps*100;
  dist_cm = soundspeed_cmps*(flightTime_sec/2);

  Serial.println(dist_cm);
  delay(500);
}
