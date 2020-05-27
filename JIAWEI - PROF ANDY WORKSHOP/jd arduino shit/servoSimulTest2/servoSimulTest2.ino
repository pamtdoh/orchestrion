//zoomkat 10-09-14 serial servo test
//type servo position 0 to 180 in serial monitor
//opposite side servos rotate together
// Powering a servo from the arduino usually *DOES NOT WORK*.

String readString;
#include <Servo.h> 
Servo myservo1;  // create servo object to control a servo 
Servo myservo2;

void setup() {
  Serial.begin(9600);
  myservo1.attach(8);
  myservo2.attach(9);
  Serial.println("servo-test"); // so I can keep track of what is loaded
}

void loop ()
{
  if (Serial.available ())
  {
    char c = Serial.read () ;
    if (c == '\n')   // assume newline is separator
    {
      int n = readString.toInt () ;
      myservo1.write (n) ;
      myservo2.write (180-n) ;
      readString = "" ;
    }
    else
      readString += c ;
  }
  //... any other stuff running in parallel
}
