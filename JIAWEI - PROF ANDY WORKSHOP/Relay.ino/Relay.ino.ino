#define RELAY_1 4
#define RELAY_2 7
#define RELAY_3 8
#define RELAY_4 12

void setup() {
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_1, OUTPUT);

  digitalWrite(RELAY_4, HIGH);
  digitalWrite(RELAY_3, LOW);
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, LOW);
  delay(1000);
}

void loop() {
  TIK_1();
  delay(1000);
  TIK_2();
  delay(1000);
}

void TIK_1() {
  if (digitalRead(RELAY_3)==HIGH){
    digitalWrite(RELAY_3, LOW);
  } else {
    digitalWrite(RELAY_3, HIGH);
  }  
  
  if (digitalRead(RELAY_4)==HIGH){
    digitalWrite(RELAY_4, LOW);
  } else {
    digitalWrite(RELAY_4, HIGH);
  }
}

void TIK_2() {
  if (digitalRead(RELAY_1)==HIGH){
    digitalWrite(RELAY_1, LOW);
  } else {
    digitalWrite(RELAY_1, HIGH);
  }  
  
  if (digitalRead(RELAY_2)==HIGH){
    digitalWrite(RELAY_2, LOW);
  } else {
    digitalWrite(RELAY_2, HIGH);
  }
}
