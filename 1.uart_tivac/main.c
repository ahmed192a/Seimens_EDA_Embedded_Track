#include "tm4c123gh6pm.h"
#include "microconfig.h"
/**
 * main.c
 */


int main(void)
{
    PLL_Init();           /* Set the internal frequency to 80MHz */

    /************ configuration of UART 0 for PC communication ******************/
    UART_ConfigType uart0={EIGHT_BITS,DISABLED,ONE_STOP_BIT,u0,9600};
    UART_init(&uart0);

    while(1){

    }
	return 0;
}
