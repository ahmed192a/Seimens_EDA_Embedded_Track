/**
 * @file main.c
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the main function.
 * @version 0.1
 * @date 2022-06-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "tm4c123gh6pm.h"
#include "microconfig.h"
/**
 * @brief  This function is the main function of the program.
 * 
 * @return int 
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
