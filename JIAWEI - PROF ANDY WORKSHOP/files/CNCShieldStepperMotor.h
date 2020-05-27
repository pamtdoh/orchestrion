class CNCShieldStepperMotor {
public:
  CNCShieldStepperMotor(byte num) {
    if (num < 0 || num > 3)
      return;
    
    motorNum = num;
    directionPin = (num == 0) ? 5 : ((num == 1) ? 6 : ((num == 2) ? 7 : 13));
    directionSet = (num == 1) ? B01000000 : ((num == 2) ? B10000000 : B00100000);
    directionClr = ~directionSet;
    stepPin      = (num == 0) ? 2 : ((num == 1) ? 3 : ((num == 2) ? 4 : 12));
    stepSet      = (num == 0) ? B00000100 : ((num == 1) ? B00001000 : B00010000);
    stepClr      = ~stepSet;
    port         = (num == 3) ? &PORTB : &PORTD;
    
    pinMode(directionPin, OUTPUT);
    pinMode(stepPin,      OUTPUT);
  }

  void move(short dir, unsigned long speed) {
    if (speed == 0) {
      cnt = 0;
      active = false;
    } else {
      if (dir > 0)
        *port &= directionClr;
      else if (dir < 0)
        *port |= directionSet;

      steps = speed / TIMERINTERVAL + 0.5;
      
      if (cnt >= steps || cnt == 0) {
        cnt = 0;
        *port |= stepSet;
        *port &= stepClr;
      }
      
      active = true;
    }
  }

  void clk() {
    if (active) {
      if (++cnt == steps) {
        cnt = 0;
        *port |= stepSet;
        *port &= stepClr;
      }
    }
  }

  static void setup(void) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
  }
  
  static void release(void) {
    digitalWrite(enablePin, HIGH);
  }

protected:
  static const byte enablePin = 8;
  byte directionPin, directionSet, directionClr, stepPin, stepSet, stepClr;
  byte motorNum = -1;
  unsigned short steps = 0, cnt = 0;
  bool active = false;

  volatile uint8_t *port;
};
