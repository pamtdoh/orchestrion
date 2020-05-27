#include <Servo.h>

Servo servoL, servoR;
#define relay1 13
#define relay2 12

int buttonState = 0;
int play = 1;

int songLength = 25;
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
