/*
 * gpio.h
 *
 *  Created on: Jun 3, 2021
 *      Author: ah132
 */

#ifndef GPIO_H_
#define GPIO_H_
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define RED_LED     0x02
#define BLUE_LED    0x04
#define GREEN_LED   0x08
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Led_on();
void Led_off();
void GPIOF_Init();
void SW1_Int_Interrupt(void);
void GPIOF_Handler(void);



#endif /* GPIO_H_ */
