#include "component.h"
#include <Wire.h>

class Printer : public Component {
public:
    void note_on(byte note) {
        String s = "off ";
        Serial.println(s + id + " " + int(note));
    }
    void note_off(byte note) {
        String s = "on ";
        Serial.println(s + id + " " + int(note));
    }

    Printer(int id) : id(id) {
    }
private:
    int id;
};


// i2c address
constexpr int ADDRESS = 3;

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    Component::map(0, new Printer(0));
    Component::map(1, new Printer(1));
}

CommandQueue queue;

void loop() {
    if (queue.available()) {
        byte buf[CMD_LEN];
        queue.pop(buf);

        byte comp_id = buf[0] >> 4;
        byte op = buf[0] & 0xF;
        byte data = buf[1];
        cmds[op](comp_id, data);
    }
}

void wireEvent(int available) {
    byte buf[BUFFER_LENGTH];
    Wire.readBytes(buf, available);
    for (int i = 0; i < available; i += CMD_LEN) {
        queue.push(buf + i);
    }
}
