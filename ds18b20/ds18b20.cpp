#include "ds18b20.h"

ds18b20::ds18b20(uint8_t b){

}

void ds18b20::initialise(){
    digitalWrite(ds18b20::b, LOW);
    delayMicroseconds(500);
    pinMode(ds18b20::b, INPUT);
    delayMicroseconds(65);               // wait for sensor to pull it low as the presence pulse
    if (digitalRead(ds18b20::b) == HIGH){
        printf("error in initialisation");
        return; 
    }
    delayMicroseconds(240);
    else{
        printf("initialised..");
    }
    pinMode(ds18b20::b, OUTPUT);
}

void ds18b20::write1(){
    digitalWrite(ds18b20::b, LOW);
    delayMicroseconds(10);
    pinMode(b, INPUT);              // released, will be pulled up by the external resistors
}

void ds18b20::write0(){
    digitalWrite(ds18b20::b, LOW);
    delayMicroseconds(60);
}

void ds18b20::readinitiate(){
    digitalWrite(b,LOW);
    delayMicroseconds(3);   // master must pull low for at leat 1 microsecond
    pinMode(ds18b20::b, INPUT);
}

void ds18b20::writebit(bool bit){
    digitalWrite(ds18b20::b, bit);
    delayMicroseconds(3);         // for sensor to be able to read

}

bool ds18b20::readbit(){
    ds18b20::readinitiate();
    bool bit = digitalRead(ds18b20::b);
    return bit;
}

long ds18b20::readrom(){                         // call initialise function first
    uint8_t byte = 0b00110011;
    for(int i=0; i<=7; i++){
        this->writebit((byte>>i) | 0);         // least significant bit first
    }
    ds18b20::readinitiate();
    uint8_t fbyte =0;
    int sn=0;
    uint8_t crc= 0;

    for(int i=0; i<= 7; i++){
        //fbyte << i;
        fbyte = fbyte | (readbit()<<i) ;
    }
    if (fbyte == 0x28){
        printf("family byte matched");

    }
    else{
        printf("error reading family byte");
        return fbyte;
    }

    for (int i=0; i<=47; i--){
        sn = ((readbit()<<i) | sn);
    }

    for (int i=0; i<= 7; i++){
        crc = ((readbit()<<i) | crc);
    }
    

}

void ds18b20::matchrom(){
    uint8_t byte = 0x55; // 55h hexadecimal value
    for(int i=7; i>=0; i--){
        ds18b20::writebit((byte>>i) | 0);
    }


}

void ds18b20::skiprom(){
    uint8_t byte = 0xCC;      // CCh hexadecimal value
    for(int i=7; i>=0; i--){            // if there is a single device on the bus this can be used to skip reading the 64 bit address and move on to the functions
        ds18b20::writebit((byte>>i) | 0);
    }
}

void ds18b20::alarmsearch(){
    uint8_t byte = 0xEC;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    ds18b20::writebit((byte>>i) | 0);
    }

}

// 3. function commands from here on

void ds18b20::convert(){
    uint8_t byte = 0x44;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    ds18b20::writebit((byte>>i) | 0);
    }
    if (power() == "e"){
        pinMode(b, INPUT);
        while (ds18b20::readbit() == 0){
            printf( "conversion in progress");    
            delayMicroseconds(5);            // keep checking the bit after every 5 microseconds
        }
        printf("conversion complete");


    }
    else{    // during parasite power bus is being pulled high by the pullup.
        pinMode(ds18b20::fet, HIGH);
        delay(750);      // t conversion 
        pinMode(ds18b20::fet, LOW); // turn on the mosfet switch to strong pull up the bus.
    }
}

void ds18b20::wscratchpad(){
    uint8_t byte = 0x4E;      // this command is to be followed by write modes
    for(int i=0; i<=7; i++){            
    ds18b20::writebit((byte>>i) | 0);
    }

}

void ds18b20::rscratchpad(){
    uint8_t byte = 0xBE;      // this command is to be followed by reading
    for(int i=7; i>=0; i--){            
    ds18b20::writebit((byte>>i) | 0);
    }
    ds18b20::readinitiate(); 
}

void ds18b20::cscratchpad(){
    uint8_t byte = 0x48;
        for(int i=7; i>=0; i--){            
    ds18b20::writebit((byte>>i) | 0);
    }

    if (ds18b20::power() == "p"){
        pinMode(ds18b20::fet, HIGH);
        delay(12);                 // at least 10ms needed to write
    }
}

void ds18b20::recall(){
    uint8_t byte = 0xB8;
    for(int i=7; i>=0; i--){            
    ds18b20::writebit((byte>>i) | 0);
    }
}

char ds18b20::power(){
    uint8_t byte = 0xB4;
    for(int i=7; i>=0; i--){            
    writebit((byte>>i) | 0);
    }
    ds18b20::readinitiate();
    if (digitalRead(ds18b20::b) == 0){
        return "p";
    }
    else{
        return "e";
    }

}


