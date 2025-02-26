
#include "usart.h"

/* the usart0 can also be used in master spi mode 

The set up for the usart has 3 parts- 
Clock generation
transmitter
receiver */

/* Clock generation

The UMSELn bit in USART control and status register C (UCSRnC) selects between asynchronous and synchronous
operation. Double speed (asynchronous mode only) is controlled by the U2Xn found in the UCSRnA register. When using
synchronous mode (UMSELn = 1), the data direction register for the XCKn pin (DDR_XCKn) controls whether the clock
source is internal (master mode) or external (slave mode). The XCKn pin is only active when using synchronous mode*/

/* Frame format
It consists of start bit, data bits, optional parity bit and one or two stop bit.
The USART character size (UCSZn2:0) bits select the number of data bits in the frame. The USART parity mode (UPMn1:0)
bits enable and set the type of parity bit. The selection between one or two stop bits is done by the USART stop bit select
(USBSn) bit. 
Registers used - UCSR0B, UCSR0C*/

usart::usart(clockmode m)
{
  baud = 9600;
  clockfreq = F_CPU;
  mode = m;
}


void usart::setclock(uint16_t br, uint16_t cf)

/* When synchronous mode is used (UMSELn = 1), the XCKn pin will be used as either clock input (Slave) or clock output
(master)*/

{
    if (mode == ASYNC)
    {
        UCSR0C = (0<<UMSEL00) | (0<<UMSEL01) | (0<<UCPOL0);

    }

    else if (mode == SLAVE_SYNC)
    {
        UCSR0C = (0<<UMSEL00) | (1<<UMSEL01) | (0<<UCPOL0);
        
    }

    else if (mode == MASTER_SYNC)
    // in the pinout diagram pd4 corresponds to xck USART external clock input/output
    {
        UCSR0C = (0<<UMSEL00) | (1<<UMSEL01) | (0<<UCPOL0);

        DDRD |= (1<<DDD4);
    }
    

    else if (mode == DOUBLE_ASYNC)
    {
        UCSR0C |= (0<<UMSEL00) | (0<<UMSEL01) | (0<<UCPOL0);
        UCSR0A |= (1<<U2X0);

    }
    
    baud = br;
    clockfreq = cf;

    
}

void usart::settx(bool x)

// to enable or not to enable transmission

{
    txen = x;
}

void usart::setrx(bool x)
{
    rxen = x;

}

void usart::txrx()

// the private function doing the actual enabling of tx and rx
{
    if (txen == true)
    {
        UCSR0B |= (1<<TXEN0);
    }

    if (rxen == true)
    {
        UCSR0B |= (1<<RXEN0);
    }

}


// function to take the frame format from the user and assign them to private variables
void usart::setframevars( uint8_t data, uint8_t parity, uint8_t stop)
{
    databits = data;
    paritybit = parity;
    stopbit = stop; 

}

void usart::frameformat()
{
    if (stopbit == 1)
    {
        UCSR0C |= (0<<USBS0);
    }

    else if (stopbit == 2)
    {
        UCSR0C |= (1<<USBS0);
    }

    if (databits == 5 )
    {
        UCSR0B |= (0<<UCSZ02);
        UCSR0C |= (0<<UCSZ01) | (0<<UCSZ00);
    }
    else if (databits == 6 )
    {
        UCSR0B |= (0<<UCSZ02);
        UCSR0C |= (0<<UCSZ01) | (1<<UCSZ00);
    }
    else if (databits == 7 )
    {
        UCSR0B |= (0<<UCSZ02);
        UCSR0C |= (1<<UCSZ01) | (0<<UCSZ00);
    }
    else if (databits == 8 )
    {
        UCSR0B |= (0<<UCSZ02);
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
    }
    else if (databits == 9 )
    {
        UCSR0B |= (1<<UCSZ02);
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
    }









}

/* the init function will */

void usart::init()
{
    if (mode == ASYNC)
    {
        divisor = 16;
    }

    else 
    {
        divisor = 8;
    }

    // only polling driven usart, not interrupt driven, for now

    uint16_t ubb = (clockfreq/(divisor*baud)) - 1;
    UBRR0H = (unsigned char)(ubb>>8);
    UBRR0L = (unsigned char)ubb;

}

void usart::transmit()
{
    /* data transmission is initiated by loading 
    transmit buffer with the data*/

    // wait for data register empty flag
    
    while(!(UCSR0A & (1<<UDRE0)))
    {

    }

    UDR0 = userdata;
}

void usart::setuserdata(int x)

// this function needs to be expanded to accept different kinds of data types such as strings, char  
{
    if (x & 0x100)
    {
        bit9 == true;
        // upload the userdata removing the most significant bit
        userdata = x<<1;
    }

    else{
        userdata = x;
    }
}

uint8_t usart::getuserdata( )
{
    return userdata;

}

