/*
 * uart.h
 *
 *  Created on: May 27, 2021
 *      Author: ah132
 */


#ifndef UART_H_
#define UART_H_




typedef enum
{
    FIVE_BITS,SIXTH_BITS, SEVEN_BITS, EIGHT_BITS
}UART_BIT_DATA;

typedef enum
{
    DISABLED, EVEN_PARITY = 2, ODD_PARITY
}UART_PARITY_MODE;

typedef enum
{
    ONE_STOP_BIT, TWO_STOP_BIT
}UART_STOP_BIT;


typedef enum
{
    u0, u1_B, u1_C, u2, u3, u4, u5, u6, u7
}UART_NUMBER;

typedef struct
{
    UART_BIT_DATA bit_data;
    UART_PARITY_MODE parity;
    UART_STOP_BIT stop;
    UART_NUMBER uart_num;
    uint32_t baudrate;
}UART_ConfigType;

void UART_init(const UART_ConfigType *Config_Ptr);
void UART_sendByte(const uint8_t data, UART_NUMBER num);
uint8_t UART_recieveByte(UART_NUMBER num);
void Uart_Init(void);
void UART0_str(char * string);
uint8_t UART0_RxString(char *ptr_string);
char UART0_RxChar(void);
void UART0_Tx(char c);
void UART_sendString(char * string, UART_NUMBER uart_num);
uint8_t UART_receiveString(char *ptr_string , UART_NUMBER uart_num);

#endif /* UART_H_ */
