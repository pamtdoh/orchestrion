#include "component.h"
#include <Wire.h>


constexpr int glockenspiel_length = 8;
class Glockenspiel : public Component {
public:
    void note_on(byte note) {
        int num = note % 1;
        if (num < glockenspiel_length) {
            digitalWrite(pins[num], LOW);
            last_trig[num] = millis();
        }
    }
    void note_off(byte note) {
    }

    Glockenspiel() {
        for (int i = 0; i < glockenspiel_length; i++) {
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], HIGH);
        }
    }
    void update() {
        for (int i = 0; i < glockenspiel_length; i++) {
            if (last_trig[i] && millis() - last_trig[i] > 20) {
                digitalWrite(pins[i], HIGH);
                last_trig[i] = 0;
            }
        }
    }
private:
    int pins[glockenspiel_length] = { 2, 3, 4, 5, 6, 7, 8, 9 };
    unsigned long last_trig[glockenspiel_length] = {0};
};


// Adress to differentiate from other slaves
constexpr int ADDRESS = 4;

Glockenspiel* glockenspiel = new Glockenspiel();

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    Component::map(7, glockenspiel);
}

void loop() {
    glockenspiel->update();
}

void wireEvent(int available) {
    while (Wire.available() < CMD_LEN);
    byte buf[CMD_LEN];
    Wire.readBytes(buf, CMD_LEN);

    byte op = buf[0];
    byte* data = buf + 1;
    cmds[op](data);
}
