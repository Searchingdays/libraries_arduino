#include "ds1820b.h"

ds18b20::ds18b20(uint8_t b){

}

void ds18b20::initialise(){
    digitalWrite(b, LOW);
    delayMicroseconds(500);
    pinMode(b, INPUT);
    delayMicroseconds(65);               // wait for sensor to pull it low as the presence pulse
    if (digitalRead(b) == HIGH){
        printf("error in initialisation")

    }
    delayMicroseconds(240);
    pinMode(b, OUTPUT);
}

void ds18b20::write1(){
    digitalWrite(b, LOW);
    delayMicroseconds(10);
    pinMode(b, INPUT);              // released, will be pulled up by the external resistors
}

void ds18b20::write0(){
    digitalWrite(b, LOW);
    delayMicroseconds(60);
}

void ds18b20::readinitiate(){
    digitalWrite(b,LOW);
    delayMicroseconds(3);
    pinMode(b, INPUT);
}

void writebit(bool bit){
    digitalWrite(b, bit);
    delayMicroseconds(3);         // for sensor to be able to read

}

bool ds18b20::readbit(){
    readinitialise();
    bool bit = digitalRead(b);
    return bit;
}

void readrom(){                         // call initialise function first
    uint8_t 33h = 0b00110011;
    for(int i=7; i>=0; i--){
        writebit((33h>>i) | 0);
    }
    readinitialise();
    uint8_t fbyte;
    int sn;
    uint8_t crc;
    for(int i=7; i>=0; i--){
        fbyte = ((readbit()<<i) | 0);
    }
    if (fbyte == 0x28){
        printf("family byte matched");

    }
    else{
        printf("error reading family byte");
        return fbyte;
    }
    for (int i=47; i>=0; i--){
        sn = ((readbit()<<i) | 0);
    }
    

}

void matchrom(){
    uint8_t byte = 0x55; // 55h hexadecimal value
    for(int i=7; i>=0; i--){
        writebit((byte>>i) | 0);
    }


}

void skiprom(){
    uint8_t byte = 0xCC;      // CCh hexadecimal value
    for(int i=7; i>=0; i--){            // if there is a single device on the bus this can be used to skip reading the 64 bit address and move on to the functions
        writebit((byte>>i) | 0);
    }
}

void alarmsearch(){
    uint8_t byte = 0xEC;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }

}

// 3. function commands from here on

void convert(){
    uint8_t byte = 0x44;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }
    if (power() == "e"){
        pinMode(b, INPUT);
        while (readbit() == 0){
            printf( "conversion in progress");    
            delayMicroseconds(5);            // keep checking the bit after every 5 microseconds
        }
        printf("conversion complete");


    }
    else{    // during parasite power bus is being pulled high by the pullup.
        pinMode(fet, HIGH);
        delay(750);      // t conversion 
        pinMode(fet, LOW); // turn on the mosfet switch to strong pull up the bus.
    }
}

void wscratchpad(data){
    uint8_t byte = 0x4E;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }

    int data;
    for (int i=0; i<= 7; i++){
    writebit((byte>>i) | 0);
    }

}

void rscratchpad(){
    uint8_t byte = 0xBE;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }
    readinitialise(); 
}

void cscratchpad(){
    uint8_t byte = 0x48;
        for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }

    if (power() == "p"){
        spull();
        delay(12); // at least 10ms needed to write
    }
}

void recall(){
    uint8_t byte = 0xB8;
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }
}

char power(){
    uint8_t byte = 0xB4;
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }
    readinitialise();
    if (digitalRead(b) == 0){
        return "p";
    }
    else{
        return "e";
    }

}


