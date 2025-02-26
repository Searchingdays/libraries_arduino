/* document code extensively */

# define __AVR_ATmega328P__

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

/* implementation of two wire protocol, in avr microcontroller atmega328p*/

// variable to store address of peripheral

uint8_t padd;

// variable to store data(1 byte)

uint8_t data;

// array to store and refer to status codes in twsr

uint8_t statuscode[7] = {0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38};

char *statusmsg[] = {
"start transmit", 
"repeated start", 
"sla_w ack received", 
"sla_w nack received", 
"data ack received", 
"data nack received", 
"arbitration lost"
};

// function setting  address and data byte

void set(uint8_t address, uint8_t data)
{
    data = data;
    padd = address;
}



int main()
{
    set(0b00000000, 22);


    while(1)
    {
        transmit();

    };

    

}