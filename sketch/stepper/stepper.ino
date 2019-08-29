#include "component.h"
#include <Wire.h>

unsigned int periods[] = {
    65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
    65535, 65535, 65535, 64793, 61156, 57724, 54484, 51426,
    48540, 45815, 43244, 40817, 38526, 36364, 34323, 32396,
    30578, 28862, 27242, 25713, 24270, 22908, 21622, 20408,
    19263, 18182, 17161, 16198, 15289, 14431, 13621, 12856,
    12135, 11454, 10811, 10204, 9631, 9091, 8581, 8099,
    7645, 7215, 6810, 6428, 6067, 5727, 5405, 5102,
    4816, 4545, 4290, 4050, 3822, 3608, 3405, 3214,
    3034, 2863, 2703, 2551, 2408, 2273, 2145, 2025,
    1911, 1804, 1703, 1607, 1517, 1432, 1351, 1276,
    1204, 1136, 1073, 1012, 956, 902, 851, 804,
    758, 716, 676, 638, 602, 568, 536, 506,
    478, 451, 426, 402, 379, 358, 338, 319,
    301, 284, 268, 253, 239, 225, 213, 201,
    190, 179, 169, 159, 150, 142, 134, 127,
    119, 113, 106, 100, 95, 89, 84, 80
};

// x -> dir: 5, step: 2
// y -> dir: 6, step: 3
// x -> dir: 7, step: 4
// a -> dir: 13, step: 12

// int dir_pins[] = {5, 6, 7, 13};
// int step_pins[] = {2, 3, 4, 12};

class StepperMotor : public Component {
public:
    void note_on(byte note) {
        period = periods[note];
    }
    void note_off(byte note) {
        period = 0;
    }

    StepperMotor(int step_pin, int dir_pin) : step_pin(step_pin), dir_pin(dir_pin) {
        pinMode(step_pin, OUTPUT);
        pinMode(dir_pin, OUTPUT);
    }
    void update() {
        if (period && micros() - last_step > period) {
            last_step = micros();
            digitalWrite(step_pin, HIGH);
            digitalWrite(step_pin, LOW);
        }
    }
private:
    int step_pin;
    int dir_pin;
    unsigned int period = 0;
    unsigned long last_step = micros();
};

int enable_pin = 8;
constexpr int stepper_count = 4;
StepperMotor* steppers[stepper_count] = {
    new StepperMotor(2, 5),
    new StepperMotor(3, 6),
    new StepperMotor(4, 7),
    new StepperMotor(12, 13)
};


// Adress to differentiate from other slaves
constexpr int ADDRESS = 3;

void setup() {
    Serial.begin(115200);

    Wire.begin(ADDRESS);
    Wire.onReceive(wireEvent);

    // set enable pin to LOW for CNC Shield
    pinMode(enable_pin, OUTPUT);
    digitalWrite(enable_pin, LOW);

    Component::map(3, steppers[0]);
    Component::map(4, steppers[1]);
    Component::map(5, steppers[2]);
    Component::map(6, steppers[3]);
}

void loop() {
    for (int i = 0; i < stepper_count; i++) {
        steppers[i]->update();
    }
}

void wireEvent(int available) {
    while (Wire.available() < CMD_LEN);
    byte buf[CMD_LEN];
    Wire.readBytes(buf, CMD_LEN);

    byte op = buf[0];
    byte* data = buf + 1;
    cmds[op](data);
}
