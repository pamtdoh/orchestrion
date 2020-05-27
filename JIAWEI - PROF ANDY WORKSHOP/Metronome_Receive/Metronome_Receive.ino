int tempo = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  int SIG = digitalRead(6);
  int PREVSIG = 0;

  if(PREVSIG == 0 && SIG == 1) {
    delay(500);
    int o = readBinary();
    delay(500);
    int t = readBinary();
    delay(500);
    int h = readBinary();

    tempo = o + t*10 + h*100;
  }
  Serial.println(tempo);
  
}

int readBinary() {
  int B1 = digitalRead(2);
  int B2 = digitalRead(3);
  int B4 = digitalRead(4);
  int B8 = digitalRead(5);

  if (B8==0 && B4==0 && B2==0 && B1==0) {
    return 0;
  } else if (B8==0 && B4==0 && B2==0 && B1==1) {
    return 1;
  } else if (B8==0 && B4==0 && B2==1 && B1==0) {
    return 2;
  } else if (B8==0 && B4==0 && B2==1 && B1==1) {
    return 3;
  } else if (B8==0 && B4==1 && B2==0 && B1==0) {
    return 4;
  } else if (B8==0 && B4==1 && B2==0 && B1==1) {
    return 5;
  } else if (B8==0 && B4==1 && B2==1 && B1==0) {
    return 6;
  } else if (B8==0 && B4==1 && B2==1 && B1==1) {
    return 7;
  } else if (B8==1 && B4==0 && B2==0 && B1==0) {
    return 8;
  } else if (B8==1 && B4==0 && B2==0 && B1==1) {
    return 9;
  }
}
