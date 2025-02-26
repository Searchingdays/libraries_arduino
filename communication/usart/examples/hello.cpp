extern "C" {
    #include "usart.h"
#include "util/delay.h"
}

using namespace std;

int main()
{
    usart comm1(usart::ASYNC);

    comm1.settx(true);

    comm1.setuserdata(8);

    comm1.setframevars(5,1,1);

    comm1.init();

    comm1.transmit();

    // while(1)

    // {

    // comm1.transmit(8);

    // _delay_ms(500);

    // }

    return 0;
}

