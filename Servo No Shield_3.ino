//Include the Servo Library
#include <Servo.h>

//Defining servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servoPos = 0;
void setup() {
  //Define servo signal inputs (Digital PWM 2,3,4)
  servo1.attach(8);

}

void loop() {
   //scan from 0 to 180 degree
  for (servoPos = 0; servoPos <30; servoPos++)
  {
    servo1.write(servoPos);
    delay(5);
  }

  //scan back from 180 to 0 degree
  for (servoPos = 30; servoPos >0; servoPos--)
  {
    servo1.write(servoPos);
    delay(10);
  }

}
