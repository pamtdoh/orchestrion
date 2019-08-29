#include "component.h"
#include "Tone.h"
#include <Wire.h>
#include <SPI.h>

unsigned int freqs[] = {
    8, 8, 9, 9, 10, 10, 11, 12,
    12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 23, 24, 25, 27, 29, 30,
    32, 34, 36, 38, 41, 43, 46, 48,
    51, 55, 58, 61, 65, 69, 73, 77,
    82, 87, 92, 97, 103, 110, 116, 123,
    130, 138, 146, 155, 164, 174, 184, 195,
    207, 220, 233, 246, 261, 277, 293, 311,
    329, 349, 369, 391, 415, 440, 466, 493,
    523, 554, 587, 622, 659, 698, 739, 783,
    830, 880, 932, 987, 1046, 1108, 1174, 1244,
    1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975,
    2093, 2217, 2349, 2489, 2637, 2793, 2959, 3135,
    3322, 3520, 3729, 3951, 4186, 4434, 4698, 4978,
    5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902,
    8372, 8869, 9397, 9956, 10548, 11175, 11839, 12543
};

class SerialPrinter : public Component {
public:
    void note_on(byte note) {
        Serial.print("on" + String((int) note));
    }
    void note_off(byte note) {
        tone.stop("off" + String((int) note));
    }
private:
};

void setup() {
    Serial.begin(115200);

    pinMode(MISO, OUTPUT);
    SPCR |= _BV(SPE);
    SPCR |= _BV(SPIE);

    Component::map(0, new SerialPrinter());
}

void loop() {
}

void note_off(byte* data) {
    byte compId = data[0];
    byte note_val = data[1];
    Component::get(compId)->note_off(note_val);
}

void note_on(byte* data) {
    byte compId = data[0];
    byte note_val = data[1];
    Component::get(compId)->note_on(note_val);
}

void (*cmds[])(byte* data) = { note_off, note_on };

ISR(SPI_STC_vect) {
    byte c = SPDR;
    Serial.print((int) SPDR);
}
