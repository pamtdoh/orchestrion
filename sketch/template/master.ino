#include "slave.h"
#include <Wire.h>

class WireRouter {
public:
    WireRouter() {
        Wire.begin();
    }
    void send(byte compId, byte* cmd) {
        getSlave(compId)->send(cmd);
    }
    void mapSlave(byte compId, Slave* slave) {
        slaveMap[compId] = slave;
    };
    Slave* getSlave(byte compId) {
        return slaveMap[compId];
    };
private:
    Slave* slaveMap[MAX_COMPONENTS];
};

WireRouter wireRouter = WireRouter();

void setup() {
    Serial.begin(115200);
    I2C_Slave* a1 = new I2C_Slave(1);
    wireRouter.mapSlave(0, a1);
    wireRouter.mapSlave(1, a1);
    wireRouter.mapSlave(2, new I2C_Slave(2));
}

void loop() {
}

// Handler when recieved command from Serial Com
// Pass through the command to the correct slave
void serialEvent() {
    while (Serial.available() < CMD_LEN);
    byte cmd[CMD_LEN];
    Serial.readBytes(cmd, CMD_LEN);

    byte compId = cmd[1];
    wireRouter.send(compId, cmd);
}