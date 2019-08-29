#include "component.h"
#include "Tone.h"
#include <Wire.h>

unsigned int freqs[] = {
    8, 9, 9, 10, 10, 11, 12, 12,
    13, 14, 15, 15, 16, 17, 18, 19,
    21, 22, 23, 24, 26, 28, 29, 31,
    33, 35, 37, 39, 41, 44, 46, 49,
    52, 55, 58, 62, 65, 69, 73, 78,
    82, 87, 92, 98, 104, 110, 117, 123,
    131, 139, 147, 156, 165, 175, 185, 196,
    208, 220, 233, 247, 262, 277, 294, 311,
    330, 349, 370, 392, 415, 440, 466, 494,
    523, 554, 587, 622, 659, 698, 740, 784,
    831, 880, 932, 988, 1047, 1109, 1175, 1245,
    1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976,
    2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136,
    3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978,
    5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902,
    8372, 8870, 9397, 9956, 10548, 11175, 11840, 12544
};

class Buzzer : public Component {
public:
    void note_on(byte note) {
        tone.play(freqs[note]);
    }
    void note_off(byte note) {
        tone.stop();
    }

    Buzzer(int pin) : tone() {
        tone.begin(pin);
    }
private:
    int pin;
    Tone tone;
};


// i2c address
constexpr int ADDRESS = 6;

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    Component::map(10, new Buzzer(11));
    Component::map(11, new Buzzer(12));
}

void loop() {
}

void wireEvent(int available) {
    while (Wire.available() < CMD_LEN);
    byte buf[CMD_LEN];
    Wire.readBytes(buf, CMD_LEN);

    byte op = buf[0];
    byte* data = buf + 1;
    cmds[op](data);
}
