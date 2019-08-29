#include <Wire.h>
// #include <SPI.h>

constexpr int CMD_LEN = 4;
constexpr int MAX_COMPONENTS = 16;

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

// class SPI_Slave : public Slave {
// public:
//     SPI_Slave(int ss) : ss(ss) {
//         SPI.begin();
//         SPI.setClockDivider(SPI_CLOCK_DIV8);
//         pinMode(ss, OUTPUT);
//         digitalWrite(ss, HIGH);
//     }
//     void send(byte* cmd) {
//         digitalWrite(ss, LOW);
//         SPI.transfer(cmd, CMD_LEN);
//         digitalWrite(ss, HIGH);
//     }
// private:
//     int ss;
// };
