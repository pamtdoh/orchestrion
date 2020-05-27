#include <Arduino.h>
#include <TM1637Display.h>
#include <OneButton.h>

#define CLK 9
#define DIO 8

TM1637Display displayTempo(CLK, DIO);

int fin = 100;

double tempo = 0;

boolean isMetr = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, INPUT);

  digitalWrite(13, LOW);


  for(int i = 14; i<=53; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }  

  displayTempo.setBrightness(0x0f);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  double duration = (15000/tempo);
      
  int SIG = digitalRead(6);
  int PLAY = digitalRead(12);
  int PREVSIG = 0;

  if(PREVSIG == 0 && SIG == 1) {
    delay(200);
    int o = readBinary();
    delay(1200);
    int t = readBinary();
    delay(1200);
    int h = readBinary();

    tempo = o + t*10 + h*100;
  }

  if(PREVSIG == 0 && PLAY == 1) {
    delay(50);
    isMetr = !isMetr;
  }

  if(isMetr) {
    for(int bar = 1; bar <= fin; bar++) {
      for (int beat = 1; beat <= 4; beat++) {
        for (int quat = 1; quat <= 4; quat++){
          high();
          delay(duration);
          low();
          delay(duration);
          if(quat==1 || quat == 2) {
            displayTemp(bar);
          } else {
            displayClear();
          }
          if(digitalRead(12) == HIGH && PREVSIG == LOW) exit;
        }
      }

    }
  }
  
  Serial.println(tempo);
  Serial.print(duration);
  displayTemp(tempo);
}

int readBinary() {
  int Bin1 = digitalRead(2);
  int Bin2 = digitalRead(3);
  int Bin4 = digitalRead(4);
  int Bin8 = digitalRead(5);

  if (Bin8==0 && Bin4==0 && Bin2==0 && Bin1==0) {
    return 0;
  } else if (Bin8==0 && Bin4==0 && Bin2==0 && Bin1==1) {
    return 1;
  } else if (Bin8==0 && Bin4==0 && Bin2==1 && Bin1==0) {
    return 2;
  } else if (Bin8==0 && Bin4==0 && Bin2==1 && Bin1==1) {
    return 3;
  } else if (Bin8==0 && Bin4==1 && Bin2==0 && Bin1==0) {
    return 4;
  } else if (Bin8==0 && Bin4==1 && Bin2==0 && Bin1==1) {
    return 5;
  } else if (Bin8==0 && Bin4==1 && Bin2==1 && Bin1==0) {
    return 6;
  } else if (Bin8==0 && Bin4==1 && Bin2==1 && Bin1==1) {
    return 7;
  } else if (Bin8==1 && Bin4==0 && Bin2==0 && Bin1==0) {
    return 8;
  } else if (Bin8==1 && Bin4==0 && Bin2==0 && Bin1==1) {
    return 9;
  }
}


void displayTemp(int val) {  
  uint8_t data[] = {0x0,0x0,0x0,0x0};
  displayTempo.setSegments(data);
  if (val > 99) {
    displayTempo.showNumberDec(val, false, 3, 1);
  } else if (val > 9 && val < 100) {
    displayTempo.showNumberDec(val, false, 2, 2);
  } else if (val > 0 && val < 10) {
    displayTempo.showNumberDec(val, false, 1, 3);
  }
}

void displayClear() {  
  uint8_t data[] = {0x0,0x0,0x0,0x0};
  displayTempo.setSegments(data);
}

void low() {
  for (int i = 14; i <= 53; i++ ) {
    digitalWrite(i, LOW);
  }
}

void high() {
  for (int i = 14; i <= 53; i++ ) {
    digitalWrite(i, HIGH);
  }
}
