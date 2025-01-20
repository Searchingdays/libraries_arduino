#ifndef i2c_h
#define i2c_h

#include <Arduino.h>

class i2c{
    
    bool readbit();
    void write(uint8_t address, uint8_t data);
    void writebit(bool data);
    uint8_t read();

    uint8_t sclpin;
    uint8_t sdapin;
};

#endif