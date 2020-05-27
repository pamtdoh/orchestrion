
/* Made by Fabian_57 on may 30, 2017
   This code is open-source so downlaod it, try it, change it to
   fit in your project.
   you have to attach a touch input at the servo
   i used an sg90 servo (chap, easy to use)
   connected to pin 9 (you ca connect to any pin, but it has to
   be a PWM pin), GND---GND, Vin---5V
   up and down variables set to 90 and 120 are the angle that the
   servo has to be in order to tap or not. Those work in my
   project, so you have to change that in order to make it work
   properly. have fun!
*/
#include <Servo.h>
int up = 90;
int down = 180;
int baseInsertPos = 40;
int pinServo = 9;//this pin
int pinLed = 13;
Servo servo1;

void setup() {
  servo1.attach(pinServo);
  servo1.write(baseInsertPos);//pull up to insert the phone
  pinMode(pinLed, OUTPUT);

}

void loop() {
  touch();
  lift();
}

void lift(){
  servo1.write(180);
  delay(100);
  servo1.write(90);
  delay(2000);
}

void touch(){
  servo1.write(0);
  delay(100);
  servo1.write(90);
  delay(500);
}
