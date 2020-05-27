#include <Servo.h>


#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

Servo servoL, servoR;
#define relay1 13
#define relay2 12

int buttonState = 0;
int play = 1;

int songLength = sizeof(melody);
int switchPin = 9;
char notes[] = "t t t t l r l r l r r l r";
int beats[] = {1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,1,1,1,1,3,1 };

// A "1" represents a quarter-note, 2 a half-note
int tempo = 180;
//how fast to play the song

void setup()
{
  servoL.attach(9);
  pinMode(8, OUTPUT);  
  servoR.attach(7);
  pinMode(6, OUTPUT); 

  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);

  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);

  pinMode(2, INPUT);
  Serial.begin(9600);
  
  reset(); 
}

void loop()
{
  int i, duration;
  buttonState = digitalRead(2);
  if (buttonState == HIGH) {    
    play = 1;
  }

  if (play == 1) {  
    
    for (i = 0; i < songLength; i++)
    {
      duration = beats[i]*(60000/(4*tempo));

      if (notes[i] == ' ')
      {
        delay(duration);
      }
      else
      {
        tap(notes[i], duration);
        delay(duration/4);      
      }
      delay(tempo/10);
      // brief pause between notes
    }
    reset();
    play = 0;
  }
  delay(10); // Delay a little bit to improve simulation performance

}


void tap(char note, int duration){
 
  switch (note) {
    case 'l':
      servoL.write(82);
      delay(duration/2);        
      servoL.write(165);
      delay(duration/2);
      break;
    case 'r':
      servoR.write(75);
      delay(duration/2);
      servoR.write(0);
      delay(duration/2);
      break;
    case 't':
      if(digitalRead(relay1) == HIGH) {
        digitalWrite(relay1, LOW);
        delay(duration);
      } else {
        digitalWrite(relay1, HIGH);
        delay(duration);
      }
    default:
      // statements
      break;
  }
}

void reset(){
  servoL.write(165);
  servoR.write(0);
}
