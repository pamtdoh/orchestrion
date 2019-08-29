/*
|| @author         Brett Hagman <bhagman@wiring.org.co>
|| @contribution   Fotis Papadopoulos <fpapadopou@gmail.com>
|| @url            http://wiring.org.co/
|| @url            http://roguerobotics.com/
||
|| @description
|| | A Software Digital Square Wave Tone Generation Library
|| |
|| | Written by Brett Hagman
|| | http://www.roguerobotics.com/
|| | bhagman@roguerobotics.com, bhagman@wiring.org.co
|| |
|| | This is a Wiring Framework (Arduino) library to produce square-wave
|| | tones on an arbitrary pin.
|| |
|| | You can make multiple instances of the Tone object, to create tones on
|| | different pins.
|| |
|| | The number of tones that can be generated at the same time is limited
|| | by the number of hardware timers available on the hardware.
|| | (e.g. ATmega328 has 3 available timers, and the ATmega1280 has 6 timers)
|| |
|| | A simplified (single tone) version of this library has been included
|| | in the Wiring Framework since Wiring 0025 and in the Arduino distribution
|| | since Arduino 0018.
|| |
|| #
||
|| @license Please see the accompanying LICENSE.txt file for this project.
||
|| @name Software PWM Library
|| @type Library
|| @target Atmel AVR 8 Bit
||
|| @version 1.0.0
||
*/

#ifndef _Tone_h
#define _Tone_h

#include <stdint.h>

/*
|| Definitions
*/

class Tone
{
  public:
    void begin(uint8_t tonePin);
    bool isPlaying();
    void play(uint16_t frequency, uint32_t duration = 0);
    void stop();

  private:
    static uint8_t _tone_pin_count;
    uint8_t _pin;
    int8_t _timer;
};

#endif

