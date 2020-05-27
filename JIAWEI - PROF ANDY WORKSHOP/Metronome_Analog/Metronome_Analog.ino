#include <Arduino.h>
#include <TM1637Display.h>
#include <OneButton.h>

#define CLKTEMP 6
#define CLKBAR 5
#define DIOTEMP 3
#define DIOBAR 2

#define LED_1 9
#define LED_2 10
#define LED_4 11
#define LED_8 12
#define SIG 13

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };

TM1637Display displayTempo(CLKTEMP, DIOTEMP);
TM1637Display displayBar(CLKBAR, DIOBAR);

int tempo = 60;
int bar = 0;
int beat = 0;
double duration = (1000*60)/(tempo*8);

OneButton sBtn(A1, true);
OneButton fBtn(A2, true);
OneButton pBtn(A3, true);

boolean isSetting = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_8, OUTPUT);
  pinMode(SIG, OUTPUT);
  pinMode(A4, OUTPUT);

  digitalWrite(LED_1, LOW);  
  digitalWrite(LED_2, LOW);  
  digitalWrite(LED_4, LOW);
  digitalWrite(LED_8, LOW);
  digitalWrite(SIG, LOW);
  digitalWrite(A4, LOW);
  
  sBtn.attachClick(singleslow);
  sBtn.attachLongPressStop(longslow);
  fBtn.attachClick(singlefast);
  fBtn.attachLongPressStop(longfast);
  pBtn.attachClick(singleclick);
  pBtn.attachDoubleClick(doubleclick);

  
  displayTempo.setBrightness(0x0f);
  displayTemp();
  displayBar.setBrightness(0x0f);
  displayB();

}

void loop() {
  // put your main code here, to run repeatedly:

  if (isSetting) {
    Serial.println("Setting tempo");
    Serial.println(tempo);
    sBtn.tick();
    fBtn.tick();
    pBtn.tick();
    delay(10);
  } else {
    digitalWrite(SIG, HIGH);
    delay(100);
    digitalWrite(SIG, LOW);
    Serial.println("Sending tempo");
    Serial.println(tempo);
    
    int o = tempo%10;
    int t = (tempo/10)%10;
    int h = tempo/100;
    int digits[] = {o, t, h};

    for (int i = 0; i <= 2; i++) {
      switch(digits[i]) {
        case 0:
          digitalWrite(LED_1, LOW);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, LOW);
          break;
        case 1:
          digitalWrite(LED_1, HIGH);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, LOW);
          break;        
        case 2:
          digitalWrite(LED_1, LOW);
          digitalWrite(LED_2, HIGH);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, LOW);
          break;        
        case 3:
          digitalWrite(LED_1, HIGH);
          digitalWrite(LED_2, HIGH);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, LOW);
          break;        
        case 4:
          digitalWrite(LED_1, LOW);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, HIGH);
          digitalWrite(LED_8, LOW);
          break;        
        case 5:
          digitalWrite(LED_1, HIGH);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, HIGH);
          digitalWrite(LED_8, LOW);
          break;
        case 6:
          digitalWrite(LED_1, LOW);
          digitalWrite(LED_2, HIGH);
          digitalWrite(LED_4, HIGH);
          digitalWrite(LED_8, LOW);
          break;
        case 7:
          digitalWrite(LED_1, HIGH);
          digitalWrite(LED_2, HIGH);
          digitalWrite(LED_4, HIGH);
          digitalWrite(LED_8, LOW);
          break;
        case 8:
          digitalWrite(LED_1, LOW);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, HIGH);
          break;
        case 9:
          digitalWrite(LED_1, HIGH);
          digitalWrite(LED_2, LOW);
          digitalWrite(LED_4, LOW);
          digitalWrite(LED_8, HIGH);
          break;
      }
      delay(1000);
    }
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
    digitalWrite(LED_4, LOW);
    digitalWrite(LED_8, LOW);
    
    isSetting = true;
    displayB();
    displayBar.setSegments(SEG_DONE);
    delay(2000);
    displayB();
  }
  

}

void displayTemp() {  
  uint8_t data[] = {0x0,0x0,0x0,0x0};
  displayTempo.setSegments(data);
  if (tempo > 99) {
    displayTempo.showNumberDec(tempo, false, 3, 1);
  } else if (tempo > 9 && tempo < 100) {
    displayTempo.showNumberDec(tempo, false, 2, 2);
  } else if (tempo > 0 && tempo < 10) {
    displayTempo.showNumberDec(tempo, false, 1, 3);
  }
}
void displayB() {  
  uint8_t data[] = {0x0,0x0,0x0,0x0};
  displayBar.setSegments(data);
}

void slow() {
  if(tempo <= 10) {
    tempo = 290+tempo;
  } else {
    tempo -= 10;
  }
  displayTemp();     
}

void fast() {
  if(tempo >= 291) {
    tempo = tempo-290;
  } else {
    tempo += 10;
  }
  displayTemp();
}

 
void longslow() {                                // what happens when button is double-clicked
  slow();
}
 
void singleslow(){                                 // what happens when the button is clicked
  if (tempo == 1) {
    tempo = 300;    
  } else {
    tempo--;
  }
  displayTemp();
}

void longfast() {                                // what happens when button is double-clicked
  fast();
}
 
void singlefast(){                                 // what happens when the button is clicked
  if (tempo == 300) {
    tempo = 1;    
  } else {
    tempo++;
  }
  displayTemp();
}

void singleclick() {
  isSetting = !isSetting;  
}

void doubleclick() {
  digitalWrite(A4, HIGH);
  delay(100);
  digitalWrite(A4, LOW);
  Serial.print("isMetr");
}
