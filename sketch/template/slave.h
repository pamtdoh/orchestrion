#include <Wire.h>

constexpr int CMD_LEN = 4;
constexpr int MAX_COMPONENTS = 8;

class Slave {
public:
    virtual void send(byte* data) = 0;
};

class I2C_Slave : public Slave {
public:
    I2C_Slave(int address) : address(address) {}
    void send(byte* cmd) {
        Wire.beginTransmission(address);
        Wire.write(cmd, CMD_LEN);
        Wire.endTransmission();
    }
private:
    int address;
};
