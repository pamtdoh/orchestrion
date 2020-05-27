int tempo = 180;
int receivePin = 0;


void setup() {
  pinMode(0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  for(int i = 1; i<=53; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  Serial.begin(9600);
}

void loop() {

  int indexCounter=0;
  boolean prev=digitalRead(receivePin);


  //tone(piezoPin, 960 ,500);
  //delay(1000);

  while (1){
    if(prev==false && digitalRead(receivePin)==HIGH){
      prev=true;
      high();
      Serial.print("1 ");
      Serial.println(indexCounter);
    }
    else if (prev==true &&digitalRead(receivePin)==LOW){
      prev=false;
      low();
      Serial.print("2 ");
      Serial.println(indexCounter);
    }
  }
}  

void low() {
  for (int i = 1; i <= 53; i++ ) {
    digitalWrite(i, LOW);
  }
}

void high() {
  for (int i = 1; i <= 53; i++ ) {
    digitalWrite(i, HIGH);
  }
}
