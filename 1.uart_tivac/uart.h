/**
 * @file uart.h
 * @author Ahmed Mohamed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions declaration to configure and use the UART.
 * @version 0.1
 * @date 2022-06-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef UART_H_
#define UART_H_

#include "microconfig.h"
/* number of bits to be sent or received */
/**
 * @brief Configure the Data Length of the UART
 * 
 */
typedef enum
{
    FIVE_BITS,
    SIXTH_BITS, 
    SEVEN_BITS, 
    EIGHT_BITS
}UART_BIT_DATA;

/**
 * @brief Choose the parity of the transmission
 * 
 *  DISABLED: No parity bit is used.
 *  EVEN_PARITY: The parity bit is always 0.
 *  ODD_PARITY: The parity bit is always 1. 
 * 
 */
typedef enum
{
    DISABLED, 
    EVEN_PARITY = 2, 
    ODD_PARITY
}UART_PARITY_MODE;


/**
 * @brief Choose the Stop bit mode
 */
typedef enum
{
    ONE_STOP_BIT, 
    TWO_STOP_BIT
}UART_STOP_BIT;


/**
 * @brief choose the UART you want to work on
 * 
 */
typedef enum
{
    u0, 
    u1_B,
    u1_C, 
    u2, 
    u3, 
    u4, 
    u5, 
    u6, 
    u7
}UART_NUMBER;

/**
 * @brief  Initialize the UART configuration
 * UART_BIT_DATA: This is the data length of the UART.
 * UART_PARITY_MODE: This is the parity mode of the UART.
 * UART_STOP_BIT: This is the stop bit of the UART.
 * UART_NUMBER: This is the UART number.
 * Baudrate: This is the baudrate of the UART. 
 */
typedef struct
{
    UART_BIT_DATA bit_data;
    UART_PARITY_MODE parity;
    UART_STOP_BIT stop;
    UART_NUMBER uart_num;
    uint32_t baudrate;
}UART_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void UART_init(const UART_ConfigType *Config_Ptr);
void UART_sendByte(const uint8_t data, UART_NUMBER num);
uint8_t UART_recieveByte(UART_NUMBER num);
void UART0_Handler(void);

#endif /* UART_H_ */
