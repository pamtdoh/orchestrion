#include "component.h"

constexpr int relay_length = 6;
class Relay : public Component {
public:
    void note_on(byte note) {
        int num = note % relay_length;
        if (num < relay_length) {
            digitalWrite(pins[num], LOW);
            last_trig[num] = millis();
        }
    }
    void note_off(byte note) {
    }

    Relay() {
        for (int i = 0; i < relay_length; i++) {
            pinMode(pins[i], OUTPUT);
            digitalWrite(pins[i], HIGH);
        }
    }
    void update() {
        for (int i = 0; i < relay_length; i++) {
            if (last_trig[i] && millis() - last_trig[i] > 10) {
                digitalWrite(pins[i], HIGH);
                last_trig[i] = 0;
            }
        }
    }
private:
    int pins[relay_length] = { 4, 5, 6, 7, 8, 9 };
    unsigned long last_trig[relay_length] = {0};
};


// i2c address
constexpr int ADDRESS = 2;

Relay* relay = new Relay();

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    Component::map(2, relay);
}

void loop() {
    relay->update();
}

/* Listen for command */
void wireEvent(int available) {
    while (Wire.available() < CMD_LEN);
    byte buf[CMD_LEN];
    Wire.readBytes(buf, CMD_LEN);

    byte op = buf[0];
    byte* data = buf + 1;
    cmds[op](data);
}