# include "i2cm.h"

i2cm::i2cm(uint8_t sclpin , uint8_t sdapin) 
{

}

void i2cm::setaddress(uint8_t addm)
{
    masteraddress = addm;
}

void i2cm::transmit()

// function for master transmit mode

{

// start condition

TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

//wait for hardware to set twint flag

while(!(TWCR & (1<<TWINT)))
{
}

// read the status register

// masking last three bits

for (int i=0; i< 7; i++)
{

if((TWSR & 0b11111000) == statuscode[i])

{
   printf("%s\n", statusmsg[i]);
}

}

// for master transmit mode, set the slave address and write(sda low) to twdr (twi data register)

TWDR = (slaveadd<<1) | 0 ;

// clear the twint bit and set twen to start transmission

TWCR = (1<<TWINT) | (1<<TWEN);

//wait for status flag to be set

while(!(TWCR & (1<<TWINT)))
{
}

// read the status register

// 0xF8 = 11111000

if ((TWSR & 0xF8) != 0x18)
{
printf("error here");
return;
}

// load data

TWDR = data;

// start transmission 

TWCR = (1<<TWINT) | (1<<TWEN);

// wait for status flag

while(!(TWCR & (1<<TWINT)))
{
}

// read the status register

if ((TWSR & 0xF8) != 0x28)
{
printf("gone");
return ;
}

// stop 

TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);

//read status register 

return;

}

uint8_t i2cm::receive()
/* Master transmit mode : 

Possible status codes in twsr are - 

0x08 : start condition has been transmitted
0x10 : repeated start condition has been transmitted
0x38 : arbitration lost in SLA+R or NOT ACK bit 
0x40 : SLA + R has been transmitted, ACK has been received
0x48 : SLA + R has been transmitted, NOT ACK has been received
0x50 : data byte received, ACK has been returned 
0x58 : data byte received, NOT ACK returned  */
{

    uint8_t data;
// start condition

TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

//wait for hardware to set twint flag

while(!(TWCR & (1<<TWINT)))
{
}

// read the status register

// masking last three bits

for (int i=0; i< 7; i++)
{

if((TWSR & 0b11111000) == statuscode[i])

{
   printf("%s\n", statusmsg[i]);
}

}

// for master receive mode, set the slave address and read(sda high) to twdr (twi data register)

TWDR = (slaveadd<<1) | 1 ;

// clear the twint bit and set twen to start transmission

TWCR = (1<<TWINT) | (1<<TWEN);

//wait for status flag to be set

while(!(TWCR & (1<<TWINT)))
{
}

// read the status register

// 0xF8 = 11111000

if ((TWSR & 0xF8) != 0x40)
{
printf("error here");
return;
}

// the data will be received in the twdr and can be read when the twint flag is set by the hardware

// wait for status flag

while(!(TWCR & (1<<TWINT)))
{
}

// read the data. the data arrives as most significant bit first 

for (int i=7; i>=0; i--)
{
    data = data | (TWDR<<i);

}

// master will inform the slave transmitter by sending a nack (leaving the sda high )

/* the bus interface unit also contains a register containing the (N)ACK bit to be transmitted or received. This
(N)ACK register is not directly accessible by the application software. However, when receiving, it can be set or cleared by
manipulating the TWI control register (TWCR) */

TWCR = (1<<TWINT) | (0<<TWEA) | (0<<TWSTA) | (0<<TWSTO);

// wait for status flag

while(!(TWCR & (1<<TWINT)))
{
}

if ((TWSR & 0xF8) != 0x58)
{
printf("error here");
return;
}

// stop condition and release the bus

TWCR = (1<<TWINT) | (1<<TWSTO);

return data;

}



void checktwsr()
{
    
}

