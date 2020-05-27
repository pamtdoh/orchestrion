#include <Arduino.h>
#include <TM1637Display.h>
#include <OneButton.h>

#define CLKTEMP 9
#define CLKBAR 8
#define DIOTEMP 3
#define DIOBAR 2

#define PIEZO 5

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

boolean isMetr = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(PIEZO, OUTPUT);

  sBtn.attachClick(singleslow);
  sBtn.attachLongPressStop(longslow);
  fBtn.attachClick(singlefast);
  fBtn.attachLongPressStop(longfast);
  pBtn.attachClick(singleclick);

  
  displayTempo.setBrightness(0x0f);
  displayTemp();
  displayBar.setBrightness(0x0f);
  displayB();

}

void loop() {
  displayBar.showNumberDec(bar, false, 3, 1);
  // put your main code here, to run repeatedly:

  if (!isMetr) {
    Serial.println("Setting tempo");
    Serial.println(tempo);
    sBtn.tick();
    fBtn.tick();
    pBtn.tick();
    delay(10);
  } else {
    Serial.println("isMetr");
    if (beat == 0) {
      tone(PIEZO, 1000, duration);
      delay(duration*8);
      bar++;
      beat++;
    } else if (beat == 4) {
      tone(PIEZO, 600, duration);
      delay(duration*8);
      bar++;
      beat = 1;
    } else {
      tone(PIEZO, 600, duration);
      delay(duration*8);
      beat++;
    }
    pBtn.tick();
    Serial.println(beat);
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
  displayBar.showNumberDec(bar, false, 1, 3);
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
  isMetr = !isMetr;  
}
