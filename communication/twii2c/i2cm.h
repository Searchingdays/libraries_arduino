#ifndef i2cm_h
#define i2cm_h

/* the microcontroller being used */

#define __AVR_ATmega328P__

#include "avr/io.h"
#include "avr/interrupt.h"
#include "stdio.h"
#include "stdlib.h"


class i2cm 

{
    public:

    /* functions */

    i2cm(uint8_t sclpin, uint8_t sdapin);
    void setaddress(uint8_t add);
    void senddata(uint8_t data);
    void transmit();
    uint8_t receive();
    void split(int data);

    // for debugging and monitoring

    void checktwsr();


    /* constants to be used by everyone*/

    uint8_t masteraddress;
    uint8_t data;
    uint8_t statuscode[7] = {0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38};

    char *statusmsg[7] = {
"start transmit", 
"repeated start", 
"sla_w ack received", 
"sla_w nack received", 
"data ack received", 
"data nack received", 
"arbitration lost"
};

     uint8_t slaveadd;



};

#endif