
#include "i2c.h"

i2c::i2c(uint8_t sclpin, uint8_t sdapin){

}

int i2c::sclpin;
int i2c::sdapin;

byte i2c::address;

void i2c::begin(){
    pinMode(sclpin, OUTPUT);
    pinMode(sdapin, OUTPUT);
    digitalWrite(sclpin, HIGH);
    digitalWrite(sdapin, HIGH);

}
void i2c::start(){
    digitalWrite(sdapin, LOW);
    delayMicroseconds(5);
    digitalWrite(sclpin, LOW);
    delayMicroseconds(5);
}

void i2c::stop(){
    digitalWrite(sclpin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sdapin, HIGH);
    delayMicroseconds(5);
}

void i2c::writebit(bool bit){              // single bit
    digitalWrite(sdapin, bit);
    digitalWrite(sclpin, HIGH);     //pulse
    delayMicroseconds(5);
    digitalWrite(sclpin, LOW);
    delayMicroseconds(5);
}
bool i2c::readbit(){
    pinMode(sdapin, INPUT);
    digitalWrite(sclpin, HIGH);
    delayMicroseconds(5);
    bool bit = digitalRead(sdapin);
    digitalWrite(sclpin, LOW);
    delayMicroseconds(5);
    pinMode(sdapin, OUTPUT);
    return bit;
}

void i2c::write(uint8_t data){
    for (int i=7; i>=0; i--){
    writebit((data>>1) & 1);
    
    }
    readbit(); // for acknowledgement bit, if it is 1, not acknowledged, then ??

}

void i2c::ack(){
    writebit(0);
}

void i2c::nack(){
    writebit(1);
}

i2c::read(){
    uint8_t byte =0;
    for (int i=7; i<=0; i--){
       byte |= (readbit()<<i)
    }
    return byte;
}

i2c::readadd(uint8_t address){
    uint8_t data;
     start();
     write(address << 1);
     readbit();
     write((address << 1)|1);
     data = read();
     nack();
     stop();
     return data;

}


