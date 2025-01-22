#ifndef ds18b20_h
#define ds18b20_h

#include "Arduino.h"

class ds18b20 {

public:                     // private, public difference ?? see

    ds18b20(uint8_t b);  // the constructor 

static uint8_t b;

static uint8_t fet;

void initialise();

bool readbit();

long readrom();

void matchrom();

void skiprom();

void alarmsearch();

void convert();

void wscratchpad();

char power();

void recall();

void cscratchpad();

void rscratchpad();

private:

void readinitiate();
void write0();

void write1();
void writebit(bool bit);

};


#endif