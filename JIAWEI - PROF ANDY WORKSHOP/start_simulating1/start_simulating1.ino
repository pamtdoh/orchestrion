#include <Servo.h>

Servo servoDoh, servoRe, servoMe;

int buttonState = 0;
int play = 0;

void setup()
{
  servoDoh.attach(9);
  pinMode(8, OUTPUT);  
  servoRe.attach(7);
  pinMode(6, OUTPUT);  
  servoMe.attach(5);
  pinMode(4, OUTPUT);
  
  pinMode(2, INPUT);
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  
  pinMode(3, OUTPUT);
  
  servoDoh.write(0);  
  servoRe.write(0);  
  servoMe.write(0);
}

void loop()
{
  buttonState = digitalRead(2);
  // print out the state of the button
  Serial.println(buttonState);
  if (buttonState == HIGH) {    
    digitalWrite(0, HIGH);
    delay(100); // Wait for 10000 millisecond(s)
    digitalWrite(13, LOW);
    play = 1;
  }
  if (play == 1) {  
  	tapDoh();
  	tapDoh();
  	restDoh();
  	tapRe();
  	restRe();
    play = 0;
  }
  delay(10); // Delay a little bit to improve simulation performance

}

void tapDoh(){
  servoDoh.write(90);
  digitalWrite(8, HIGH);
  delay(200); // Wait for 200 millisecond(s)
  tone(3, 440, 100); // play tone 59 (B4 = 494 Hz)
  servoDoh.write(0);
  digitalWrite(8, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}

void restDoh(){
  servoDoh.write(0);
}

void tapRe(){
  servoRe.write(90);
  digitalWrite(6, HIGH);
  delay(200); // Wait for 200 millisecond(s)
  tone(3, 494, 100); // play tone 59 (B4 = 494 Hz)
  servoRe.write(0);
  digitalWrite(6, LOW);
  delay(1000); // Wait for 1000 millisecond(s)
}


void restRe(){
  servoRe.write(0);
}
