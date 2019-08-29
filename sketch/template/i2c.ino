#include "component.h"
#include <Wire.h>

class Instrument : public Component {
public:
    void note_on(byte note) {
    }
    void note_off(byte note) {
    }

    Instrument() {
    }
};

// i2c address
constexpr int ADDRESS = 1;

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    Component::map(0, new Instrument());
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
