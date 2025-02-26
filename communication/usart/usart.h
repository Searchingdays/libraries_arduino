#ifndef usart_h
#define usart_h

#define __AVR_ATmega328P__


#include "avr/interrupt.h"
#include "stdlib.h"
//#include "avr/iom328p.h"


/* the microcontroller being used */

#define F_CPU 16000000UL


class usart 
{

    public:

    typedef enum 
    {
        ASYNC,
        DOUBLE_ASYNC,
        SLAVE_SYNC,
        MASTER_SYNC

    } clockmode;

    usart(clockmode m);


    void settx(bool x);
    void setrx(bool x);

    void setuserdata(int x);

    void setframevars( uint8_t data, uint8_t parity, uint8_t stop);

    void init();
    void setclock(uint16_t br, uint16_t cf);
    void transmit();

    uint8_t getuserdata();


    clockmode mode;

    private:

    // these can now be accessed only by the set clock function

    uint16_t clockfreq;
    uint16_t baud;

    //variable for enbling transmission or not 
    bool txen;

    // variable for enabling receiving or not
    bool rxen;

    //variable deciding if there are 9 bit data or not
    bool bit9;

    // function enabling transmission and receiving
    void txrx();


    // function to set the frame format registers
    void frameformat();

    uint8_t stopbit;
    uint8_t databits;
    uint8_t paritybit;


    // userdata is initialised to zero
    uint8_t userdata;

    uint8_t divisor;

};

#endif