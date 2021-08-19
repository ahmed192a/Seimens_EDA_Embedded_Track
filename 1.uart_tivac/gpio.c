/*
 * gpio.c
 *
 *  Created on: Jun 3, 2021
 *      Author: ah132
 */

#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "systick.h"
#include "stdint.h"
/*******************************************************************************
 *                       Global variable                                       *
 *******************************************************************************/
volatile unsigned char g_Interrupt_Flag = 0;

/*******************************************************************************
 *                       Function Declaration                                  *
 *******************************************************************************/
void GPIOF_Init(){
    volatile uint32_t delay ;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // activate port F
    delay = 1;
    GPIO_PORTF_CR_R     |= BLUE_LED;
    GPIO_PORTF_AFSEL_R  &=~BLUE_LED;
    GPIO_PORTF_PCTL_R   &=~BLUE_LED;
    GPIO_PORTF_AMSEL_R  &=~BLUE_LED;
    GPIO_PORTF_DIR_R    |= BLUE_LED;
    GPIO_PORTF_DEN_R    |= BLUE_LED;

}
void Led_on(){
    GPIO_PORTF_DATA_R |= BLUE_LED;
}
void Led_off(){
    GPIO_PORTF_DATA_R &=~ BLUE_LED;
}


void SW1_Int_Interrupt(void)    //PORTF_Enable_Interrupt
{
    //__enable_irq();
    // Enable Switch on PF4
    volatile uint32_t delay ;
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // activate port F
    delay = 1;
    GPIO_PORTF_CR_R |= 0X10;
    GPIO_PORTF_AFSEL_R  &=~0x10;
    GPIO_PORTF_PCTL_R   &=~0x10;
    GPIO_PORTF_AMSEL_R  &=~0x10;
    GPIO_PORTF_DIR_R    &=~0x10;
    GPIO_PORTF_DEN_R    |= 0X10;
    GPIO_PORTF_PUR_R    |= 0x10;
    // Enable External interrupt on PF4
    GPIO_PORTF_IS_R  &=~ 0x10;        // falling edge or 0xff for rising edge
    GPIO_PORTF_IBE_R &=~ 0x10;        /*to low level or falling edge   ,  0xFF        to high level or rising edge  */
    GPIO_PORTF_IEV_R &=~ 0x10;        /*to low level or falling edge*/
    GPIO_PORTF_ICR_R |=  0x10;        /* pin_num*/
    GPIO_PORTF_IM_R  |=  0x10;        /* unmask interrupt */
    //NVIC_IP
    NVIC_PRI7_R = (3<<5);
    NVIC_EN0_R |=(1<<30);
}
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
void GPIOF_Handler(void)
{
    g_Interrupt_Flag ^= 0x1;
//    arrived = 0;
//    r_distance=0;
//    k=0;
    GPIO_PORTF_ICR_R |= 0x10; /* clear the interrupt flag */
    delay_ms();
}


