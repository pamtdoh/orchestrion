#define TIMERINTERVAL 12 // Has to be a 4*n and not larger than 1024

#include "CNCShieldStepperMotor.h"

#define MOTORS 4

#if MOTORS < 1
  #error Connect at least 1 Motor!
#else
  CNCShieldStepperMotor xMotor(0);
  #if MOTORS > 1
    CNCShieldStepperMotor yMotor(1);
    #if MOTORS > 2
      CNCShieldStepperMotor zMotor(2);
      #if MOTORS > 3
        CNCShieldStepperMotor aMotor(3);
      #endif
    #endif
  #endif
#endif

//unsigned int cnt = 0;

void setup() {
  Serial.begin(9600);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2  = 0;
  OCR2A = TIMERINTERVAL / 4;
  TCCR2B |= (1 << CS22);   
  TIMSK2 |= (1 << OCIE2A);
  
  sei();
}

ISR(TIMER2_COMPA_vect) {
  TCNT2 = 0;
  xMotor.clk();
#if MOTORS > 1
  yMotor.clk();
  #if MOTORS > 2
  zMotor.clk();
    #if MOTORS > 3
  aMotor.clk();
    #endif
  #endif
#endif
}

void loop() {
  if (Serial.available() > 2) {
    char c = Serial.read();
    unsigned short value = (((unsigned short) Serial.read()) << 8) | ((unsigned short) Serial.read());

    if (c == 'X')
      xMotor.move(value/abs(value), value);
#if MOTORS > 1
    else if (c == 'Y')
      yMotor.move(value/abs(value), value);
  #if MOTORS > 2
    else if (c == 'Z')
      zMotor.move(value/abs(value), value);
    #if MOTORS > 3
    else if (c == 'A')
      aMotor.move(value/abs(value), value);
    #endif
  #endif
#endif
    //else if (c == 'P')
    //    delay(value);
    else if (c == 'E') {
      if (value == 0)
        CNCShieldStepperMotor::release();
      else
        CNCShieldStepperMotor::setup();
    }
  }
}
