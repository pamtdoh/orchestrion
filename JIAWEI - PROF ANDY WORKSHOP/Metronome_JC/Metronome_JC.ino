#include <JC_Button.h>

#include <Arduino.h>
#include <TM1637Display.h>
#include <JC_Button.h>

#define CLKTEMP 9
#define CLKBAR 8
#define DIOTEMP 3
#define DIOBAR 2

#define PLAY 12
#define SLOW 11
#define FAST 10
#define PIEZO 5

const uint8_t SEG_DONE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };

TM1637Display displayTempo(CLKTEMP, DIOTEMP);
TM1637Display displayBar(CLKBAR, DIOBAR);

int tempo = 100;
int beat = 1;
int duration = (1000*60)/(tempo*8);

Button sBtn(SLOW);
Button fBtn(FAST);
Button pBtn(PLAY);

const unsigned long
  LONG_PRESS(1000);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sBtn.begin();
  fBtn.begin();
  pBtn.begin();
  pinMode(PIEZO, OUTPUT);
    
  displayTempo.setBrightness(0x0f);
  displayTemp();
  displayBar.setBrightness(0x0f);
  displayB();

}

void loop() {
  displayBar.showNumberDec(beat, false, 3, 1);
  displayTemp();
  // put your main code here, to run repeatedly:

  boolean isMetr = false;

  if (sBtn.wasReleased()) {
    if (tempo == 1) {
      tempo = 300;    
    } else {
      tempo--;
    }
    displayTemp();
  } else if (sBtn.pressedFor(LONG_PRESS)) {
    slow();
  }
  
  if (fBtn.wasReleased()) {
    if (tempo == 300) {
      tempo = 1;    
    } else {
      tempo++;
    }
    displayTemp();
  } else if (fBtn.pressedFor(LONG_PRESS)) {
    fast();
  }

  if (pBtn.wasReleased()) {
    isMetr = true;
  }

  

  while (isMetr) {
    if (beat == 1) {
      tone(PIEZO, 1000, duration);
      delay(duration*8);
      beat++;
    } else if (beat == 4) {
      tone(PIEZO, 600, duration);
      delay(duration*8);
      beat = 1;
    } else {
      tone(PIEZO, 600, duration);
      delay(duration*8);
      beat++;
    }
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
  displayTempo.showNumberDec(tempo, false, 1, 3);
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
