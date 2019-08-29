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
    I2C_Slave* a1 = new I2C_Slave(1);
    I2C_Slave* a2 = new I2C_Slave(2);
    I2C_Slave* a3 = new I2C_Slave(3);
    I2C_Slave* a4 = new I2C_Slave(4);
    I2C_Slave* a5 = new I2C_Slave(5);
    I2C_Slave* a6 = new I2C_Slave(6);
    Serial.begin(115200);
    wireRouter.mapSlave(0, a1);
    wireRouter.mapSlave(1, a1);
    wireRouter.mapSlave(2, a2);
    wireRouter.mapSlave(3, a3);
    wireRouter.mapSlave(4, a3);
    wireRouter.mapSlave(5, a3);
    wireRouter.mapSlave(6, a3);
    wireRouter.mapSlave(7, a4);
    wireRouter.mapSlave(8, a5);
    wireRouter.mapSlave(9, a5);
    wireRouter.mapSlave(10, a6);
    wireRouter.mapSlave(11, a6);
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