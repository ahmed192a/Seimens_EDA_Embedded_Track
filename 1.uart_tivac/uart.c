/**
 * @file uart.c
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions implementation to configure and use the UART .
 * @version 0.1
 * @date 2022-06-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "uart.h"


/*******************************************************************************
 *                      Function Declaration                                   *
 *******************************************************************************/
/*******************************************************************************
 * @brief  This function initializes the UART.
 * @param  Config_Ptr : Pointer to a struct contains
 *                      (no. of data bits, parity mode, UART num, baudrate)
 * @return void
 *******************************************************************************/
void UART_init(const UART_ConfigType *Config_Ptr)
{
    double uart_clk = F_CPU / (16 * Config_Ptr->baudrate);
    switch (Config_Ptr->uart_num){
        case  u0: /* UART0 -> portA */

            SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;       /* Enable UART0 clock */
            SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;       /* Enable PORTA clock */

            UART0_CTL_R &= ~UART_CTL_UARTEN;            /* Disable UART0 enable */

            /* Adjust baudrate */
            UART0_IBRD_R = (uint32_t)uart_clk;
            UART0_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) *64 + 0.5) ;

            /* Adjust BIT_Data number (number of bits to be sent or received)*/
            if(Config_Ptr->bit_data == FIVE_BITS)
            {
                UART0_LCRH_R = (UART0_LCRH_R & UART_LCRH_WLEN_M )| UART_LCRH_WLEN_5;
            }
            else if(Config_Ptr->bit_data == SIXTH_BITS)
            {
                UART0_LCRH_R = (UART0_LCRH_R & UART_LCRH_WLEN_M )| UART_LCRH_WLEN_6;
            }
            else if(Config_Ptr->bit_data == SEVEN_BITS)
            {
                UART0_LCRH_R = (UART0_LCRH_R & UART_LCRH_WLEN_M )| UART_LCRH_WLEN_7;
            }
            else if(Config_Ptr->bit_data == EIGHT_BITS)
            {
                UART0_LCRH_R = (UART0_LCRH_R & UART_LCRH_WLEN_M )| UART_LCRH_WLEN_8;
            }

            /* Enable the FIFO */
            UART0_LCRH_R |= UART_LCRH_FEN;

            /* Enable RXE, TXE, UARTEN */
            UART0_CTL_R |= UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE;



            /* Enable PA0, PA1 as U0RX, U0TX  */
            GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&(~(GPIO_PCTL_PA0_M|GPIO_PCTL_PA1_M)))+(GPIO_PCTL_PA0_U0RX|GPIO_PCTL_PA1_U0TX);
            GPIO_PORTA_AFSEL_R |= P0 | P1;                          /* Enable the alternate functions on pin 0, 1 */
            GPIO_PORTA_DEN_R   |= P0 | P1;                          /* Enable Digital on pins 0, 1 */
            GPIO_PORTA_AMSEL_R &= (~P0) & (~P1);                    /* Disable Analog on pins 0, 1 */
			
						
            UART0_IM_R |= UART_IM_RXIM;         // Enable RX Interrupt for UART0
            NVIC_EN0_R |= NVIC_EN0_UART0_M;    // Enable Interrupt for UART0
			/********************************** Enabel the I Bit *********************************/
			Enable_Interrupts();
            break;

    }

}



/*******************************************************************************
 * @brief  This function sends a data to another device. (max 1 byte)
 * @param  data : Data to be sent
 * @param  uart_num : UART number
 * @return void
 *******************************************************************************/
void UART_sendByte(const uint8_t data, UART_NUMBER uart_num)
{
    if(uart_num == u0)
    {
        while((UART0_FR_R &UART_FR_TXFF) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART0_DR_R = data;
    }

}


/*******************************************************************************
 * @brief Function to receive a data from another device
 * 
 * @param uart_num : UART number
 * @return uint8_t  : the received data or 0 if no data is received
 *******************************************************************************/
uint8_t UART_recieveByte(UART_NUMBER uart_num)
{
    if(uart_num == u0)
    {
        while((UART0_FR_R&UART_FR_RXFE) != 0);      // wait until RXFE is 0
        return((char)(UART0_DR_R&UART_DR_DATA_M));
    }
    return 0;
}


/*******************************************************************************
 * @brief  This function is the ISR for UART0.
 * @param  void
 * @return void
 * @note   This function is called when UART0 receives a character.
 *          It checks if the received character is small alphabet and if it is then it sends the
 *          capital character and if it's anything else it sends the successive character.
 *
 *******************************************************************************/
void UART0_Handler(void){
    UART0_ICR_R |= UART_ICR_RXIC;
    uint8_t c;
    c = ((char)(UART0_DR_R&UART_DR_DATA_M));
    if(c>= 'a' && c <= 'z'){
        UART_sendByte(c + 32, u0);
    }
    else{
        UART_sendByte(c + 1, u0);
    }
}
