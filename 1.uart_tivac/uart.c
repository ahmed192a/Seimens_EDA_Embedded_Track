/*
 * uart.c
 *
 *  Created on: May 27, 2021
 *      Author: ah132
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "tm4c123gh6pm.h"                    // Device header
#include "uart.h"



void UART_init(const UART_ConfigType *Config_Ptr)
{
    //printf("clock = %ld\n",SysCtlClockGet()); //to get system freq
    double uart_clk = 80000000 / (16 * Config_Ptr->baudrate); /* 16 MHz -> 80 MHz */

    if(Config_Ptr->uart_num == u0) /* UART0 -> portA */
    {
        SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;       /* Enable UART0 clock */
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;       /* Enable PORTA clock */

        //ClEAR_BIT(UART0->CTL, 0);   /* Disable UART0 enable */
        UART0_CTL_R &= ~(1<<0);     /* Disable UART0 enable */

        /* Adjust baudrate */
        UART0_IBRD_R = (uint32_t)uart_clk;
        UART0_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) *64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART0_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART0_LCRH_R |= (1<<5);
            UART0_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART0_LCRH_R &= ~(1<<5);
            UART0_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART0_LCRH_R |= (1<<5);
            UART0_LCRH_R |= (1<<6);
        }
        UART0_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART0_CTL_R |= (1<<0) | (1<<8) | (1<<9);

        // GPIO_PORTA_PCTL_R |= (1<<0) | (1<<1);
        GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
        GPIO_PORTA_AFSEL_R |= (1<<0) | (1<<1);
        GPIO_PORTA_DEN_R |= (1<<0) | (1<<1);
        GPIO_PORTA_AMSEL_R &= (~(1<<0)) &(~(1<<1));
    }

    else if(Config_Ptr -> uart_num == u1_B)
    {
        SYSCTL_RCGCUART_R |= (1<<1); /* activate UART1 */
        SYSCTL_RCGCGPIO_R |= (1<<1); /* activate PORTB */

        UART1_CTL_R &= ~(1<<0);     /* Disable UART1 enable */

        /* Adjust baudrate */
        UART1_IBRD_R = (uint32_t)uart_clk;
        UART1_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) * 64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART1_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART1_LCRH_R |= (1<<5);
            UART1_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART1_LCRH_R &= ~(1<<5);
            UART1_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART1_LCRH_R |= (1<<5);
            UART1_LCRH_R |= (1<<6);
        }
        UART1_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART1_CTL_R |= (1<<0) | (1<<8) | (1<<9);



        GPIO_PORTB_AFSEL_R |= (1<<0) | (1<<1);
        GPIO_PORTB_DEN_R |= (1<<0) | (1<<1);
        GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)+0x00000011;
        GPIO_PORTB_AMSEL_R &= (~(1<<0)) & (~(1<<1));
    }

    else if(Config_Ptr -> uart_num == u1_C)
    {
        SYSCTL_RCGCUART_R |= (1<<1); /* activate UART1 */
        SYSCTL_RCGCGPIO_R |= (1<<2); /* activate PORTC */

        UART1_CTL_R &= ~(1<<0);     /* Disable UART1 enable */

        /* Adjust baudrate */
        UART1_IBRD_R = (uint32_t)uart_clk;
        UART1_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) * 64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART1_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART1_LCRH_R |= (1<<5);
            UART1_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART1_LCRH_R &= ~(1<<5);
            UART1_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART1_LCRH_R |= (1<<5);
            UART1_LCRH_R |= (1<<6);
        }
        UART1_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART1_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
        GPIO_PORTC_AFSEL_R |= (1<<4) | (1<<5);
        GPIO_PORTC_DEN_R |= (1<<4) | (1<<5);
        GPIO_PORTC_AMSEL_R &= (~(1<<4)) & (~(1<<5));
    }
    else if(Config_Ptr -> uart_num == u3)
    {
        SYSCTL_RCGCUART_R |= (1<<3); /* activate UART3 */
        SYSCTL_RCGCGPIO_R |= (1<<2); /* activate PORTC */

        UART3_CTL_R &= ~(1<<0);     /* Disable UART3 enable */

        /* Adjust baudrate */
        UART3_IBRD_R = (uint32_t)uart_clk;
        UART3_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) * 64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART3_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART3_LCRH_R |= (1<<5);
            UART3_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART3_LCRH_R &= ~(1<<5);
            UART3_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART3_LCRH_R |= (1<<5);
            UART3_LCRH_R |= (1<<6);
        }
        UART3_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART3_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0x00FFFFFF)+0x11000000;
        GPIO_PORTC_AFSEL_R |= (1<<6) | (1<<7);
        GPIO_PORTC_DEN_R |= (1<<6) | (1<<7);
        GPIO_PORTC_AMSEL_R &= (~(1<<6)) & (~(1<<7));
    }

    else if(Config_Ptr -> uart_num == u4)
    {
        SYSCTL_RCGCUART_R |= (1<<4); /* activate UART4 */
        SYSCTL_RCGCGPIO_R |= (1<<2); /* activate PORTC */

        UART4_CTL_R &= ~(1<<0);     /* Disable UART4 enable */

        /* Adjust baudrate */
        UART4_IBRD_R = (uint32_t)uart_clk;
        UART4_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) * 64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART4_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART4_LCRH_R |= (1<<5);
            UART4_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART4_LCRH_R &= ~(1<<5);
            UART4_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART4_LCRH_R |= (1<<5);
            UART4_LCRH_R |= (1<<6);
        }
        UART4_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART4_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00110000;
        GPIO_PORTC_AFSEL_R |= (1<<4) | (1<<5);
        GPIO_PORTC_DEN_R |= (1<<4) | (1<<5);
        GPIO_PORTC_AMSEL_R &= (~(1<<4)) & (~(1<<5));
    }

    else if(Config_Ptr -> uart_num == u5)
    {
        SYSCTL_RCGCUART_R |= (1<<5); /* activate UART5 */
        SYSCTL_RCGCGPIO_R |= (1<<4); /* activate PORTE */

        UART5_CTL_R &= ~(1<<0);     /* Disable UART5 enable */

        /* Adjust baudrate */
        UART5_IBRD_R = (uint32_t)uart_clk;
        UART5_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) *64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART5_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART5_LCRH_R |= (1<<5);
            UART5_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART5_LCRH_R &= ~(1<<5);
            UART5_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART5_LCRH_R |= (1<<5);
            UART5_LCRH_R |= (1<<6);
        }
        UART5_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART5_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFF00FFFF)+0x00110000;
        GPIO_PORTE_AFSEL_R |= (1<<5) | (1<<4);
        GPIO_PORTE_DEN_R |= (1<<5) | (1<<4);
        GPIO_PORTE_AMSEL_R &= (~(1<<5)) & (~(1<<4));
    }

    else if(Config_Ptr -> uart_num == u6)
    {
        SYSCTL_RCGCUART_R |= (1<<6); /* activate UART6 */
        SYSCTL_RCGCGPIO_R |= (1<<3); /* activate PORTD */

        UART6_CTL_R &= ~(1<<0);     /* Disable UART6 enable */

        /* Adjust baudrate */
        UART6_IBRD_R = (uint32_t)uart_clk;
        UART6_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) *64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART6_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART6_LCRH_R |= (1<<5);
            UART6_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART6_LCRH_R &= ~(1<<5);
            UART6_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART6_LCRH_R |= (1<<5);
            UART6_LCRH_R |= (1<<6);
        }
        UART6_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART6_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0xFF00FFFF)+0x00110000;
        GPIO_PORTD_AFSEL_R |= (1<<5) | (1<<4);
        GPIO_PORTD_DEN_R |= (1<<5) | (1<<4);
        GPIO_PORTD_AMSEL_R &= (~(1<<5)) & (~(1<<4));
    }
    else
    {
        SYSCTL_RCGCUART_R |= (1<<7); /* activate UART7 */
        SYSCTL_RCGCGPIO_R |= (1<<4); /* activate PORTE */

        UART7_CTL_R &= ~(1<<0);     /* Disable UART7 enable */

        /* Adjust baudrate */
        UART7_IBRD_R = (uint32_t)uart_clk;
        UART7_FBRD_R = (uint32_t)((uart_clk - (uint32_t)uart_clk) *64 + 0.5) ;

        /* Adjust BIT_Data number */
        if(Config_Ptr->bit_data == FIVE_BITS)
        {
            UART7_LCRH_R = 0;
        }
        else if(Config_Ptr->bit_data == SIXTH_BITS)
        {
            UART7_LCRH_R |= (1<<5);
            UART7_LCRH_R &= ~(1<<6);
        }
        else if(Config_Ptr->bit_data == SEVEN_BITS)
        {
            UART7_LCRH_R &= ~(1<<5);
            UART7_LCRH_R |= (1<<6);
        }
        else if(Config_Ptr->bit_data == EIGHT_BITS)
        {
            UART7_LCRH_R |= (1<<5);
            UART7_LCRH_R |= (1<<6);
        }
        UART7_LCRH_R |= 0X10;
        /* Enable RXE, TXE, UARTEN */
        UART7_CTL_R |= (1<<0) | (1<<8) | (1<<9);


        GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFFF00)+0x00000011;
        GPIO_PORTE_AFSEL_R |= (1<<1) | (1<<0);
        GPIO_PORTE_DEN_R |= (1<<1) | (1<<0);
        GPIO_PORTE_AMSEL_R &= (~(1<<1)) & (~(1<<0));
    }


    /*
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0; // activate UART0
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 520;//27;                    // IBRD = int(50,000,000 / (16 * 115,200)) = int(27.1267)
  UART0_FBRD_R = 53;//8;                     // FBRD = int(0.1267 * 64 + 0.5) = 8
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA  */
}

void UART_sendByte(const uint8_t data, UART_NUMBER num)
{
    if(num == u0)
    {
        while((UART0_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART0_DR_R = data;
    }
    else if(num == u1_B)
    {
        while((UART1_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART1_DR_R = data;
    }
    else if(num == u1_C)
    {
        while((UART1_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART1_DR_R = data;
    }
    else if(num == u3)
    {
        while((UART3_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART3_DR_R = data;
    }
    else if(num == u4)
    {
        while((UART4_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART4_DR_R = data;
    }
    else if(num == u5)
    {
        while((UART5_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART5_DR_R = data;
    }
    else if(num == u6)
    {
        while((UART6_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART6_DR_R = data;
    }
    else
    {
        while((UART7_FR_R &0x0020) != 0);   /* wait until fifo is not full, 0-> not full*/
        UART7_DR_R = data;
    }

}

uint8_t UART_recieveByte(UART_NUMBER num)
{
    if(num == u0)
    {
        while((UART0_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART0_DR_R&0xFF));
    }
    else if(num == u1_B)
    {
        while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART1_DR_R&0xFF));
    }
    else if(num == u1_C)
    {
        while((UART1_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART1_DR_R&0xFF));
    }
    else if(num == u3)
    {
        while((UART3_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART3_DR_R&0xFF));
    }
    else if(num == u4)
    {
        while((UART4_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART4_DR_R&0xFF));
    }
    else if(num == u5)
    {
        while((UART5_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART5_DR_R&0xFF));
    }
    else if(num == u6)
    {
        while((UART6_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART6_DR_R&0xFF));
    }
    else
    {
        while((UART7_FR_R&0x0010) != 0);      // wait until RXFE is 0
        return((char)(UART7_DR_R&0xFF));
    }
}
void UART0_Tx(char c)
{
  while((UART0_FR_R &(1<<5)) != 0);                 // waits the program till the previous transmission was completed
     =c;                                   // Set UART0 DATA REGISTER is equal to c
}

void UART0_str(char * string)
{
  while(*string)
  {
      UART0_Tx(*(string++));    // UART0_str is called with every increment in string pointer
  }
}

void UART_sendString(char * string, UART_NUMBER uart_num)
{
    while(*string)
    {
        UART_sendByte(*(string++), uart_num);
    }
}

char UART0_RxChar(void)
{
    while((UART0_FR_R & (1<<6)) == 0) ;
    return (char) UART0_DR_R ;
}

uint8_t UART0_RxString(char *ptr_string)
{
    char ch;
    uint8_t len = 0;
    while(1)
    {
        ch=UART0_RxChar();                  // Receive a char
        UART0_Tx(ch);                      // Echo back the received char

        if((ch=='\r') || (ch=='\n'))             // Read till enter key is pressed
        {
        if(len!=0)                 // Wait till atleast 1 char is received
        {
           ptr_string[len]=0;    // once enter key is pressed null terminate the string and break the loop
               break;
        }
        }
        else if((ch=='\b') && (len!=0))
        {
            len--;                     // If backspace is pressed then decrement the index to remove the old char
        }
        else
        {
            ptr_string[len]=ch;               //copy the char into string and increment the index
            len++;
        }
    }
  return len;
}

uint8_t UART_receiveString(char *ptr_string , UART_NUMBER uart_num)
{
    char ch;
    uint8_t len = 0;
    while(1)
    {
        ch = UART_recieveByte(uart_num);
        UART_sendByte(ch,uart_num);
        if((ch=='\r') || (ch=='\n'))             // Read till enter key is pressed
        {
        if(len!=0)                 // Wait till atleast 1 char is received
        {
           ptr_string[len]=0;    // once enter key is pressed null terminate the string and break the loop
               break;
        }
        }
        else if((ch=='\b') && (len!=0))
        {
            len--;                     // If backspace is pressed then decrement the index to remove the old char
        }
        else
        {
            ptr_string[len]=ch;               //copy the char into string and increment the index
            len++;
        }
    }
    return len;
}
