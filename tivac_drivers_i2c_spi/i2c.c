/**
 * @file i2c.c
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions implementation to configure and use the I2C .
 * @version 0.1
 * @date 2021-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/**********************************************************************************/
/*                          Includes Headers                                      */
/**********************************************************************************/
#include "micro_config.h"

/**********************************************************************************/
/*                          Functions Definitaions                                */
/**********************************************************************************/
/**
 * @brief  This function initializes the I2C.
 * @param  Config_Ptr : Pointer to a struct contains
 *                      (I2C num, baudrate)
 * @return void
 */
void I2C_Init(const I2C_Config *I2C){
    switch(I2C->module)         /* Selecting the I2C module */
    {
    case m0:
        SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;                  /* Enabling the I2C module */
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;                /* Enabling the GPIO module */
        while ((SYSCTL_PRGPIO_R & SYSCTL_RCGC2_GPIOB) == 0);    /* Waiting for the GPIO module to be ready */
        GPIO_PORTB_AFSEL_R |= 0xC0;                             /* Enabling the alternate function */
        GPIO_PORTB_ODR_R   |= P6 | P7;                          /* Enable open drain on pins 6, 7 */
        GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&(~(GPIO_PCTL_PB6_M|GPIO_PCTL_PB7_M)))+(GPIO_PCTL_PB7_T0CCP1|GPIO_PCTL_PB6_T0CCP0);
        GPIO_PORTB_DEN_R   |= P6 | P7;                          /* Enable Digital on pins 6, 7 */
        GPIO_PORTB_AMSEL_R &= (~P7) & (~P6);                    /* Disable Analog on pins 6, 7 */
        switch(I2C->function)   /* Selecting the I2C function */
        {
        case Master:
            I2C0_MCR_R = I2C_MCR_MFE;                               /* I2C Master Function Enable  */
            break;
        case Slave:
            I2C0_MCR_R = I2C_MCR_SFE;                               /* I2C Slave Function Enable  */
            break;
        case Loopback:
            I2C0_MCR_R = I2C_MCR_LPBK;                               /* I2C Loopback  */
            break;
        case Glitch_filter:
            I2C0_MCR_R = I2C_MCR_GFE;                               /* I2C Glitch Filter Enable  */
            break;
        }
        if(I2C->mode == High_speed_mode){               /* Selecting the I2C mode */
            I2C0_MTPR_R |= I2C_MTPR_HS;                             /* I2C High Speed Mode  */
        }
        else{
            I2C0_MTPR_R &= ~I2C_MTPR_HS;                            /* I2C Standard Speed Mode  */
        }
        uint8_t TPR = (SysClk/(2*(6+4)*I2C->mode))-1;               /* Calculating the TPR value */
        I2C0_MTPR_R = (I2C0_MTPR_R & (~0x7F)) | (TPR & 0x7F);       /* Setting the TPR value */
        I2C0_MSA_R = ((I2C->address&0xFF)<<1) | (I2C->operation &0x01); /* Setting the I2C address */
    break;

    }

}
/**
 * @brief  This function sends a start condition to the I2C.
 * @param  I2C_Config* I2C : Pointer to a struct contains I2C configurstion (I2C num, baudrate)
 * @param  uint_8 data : data to be sent
 * @return void
 */
void I2C_Trasmit_byte(const I2C_Config *I2C, uint8_t data){
    I2C0_MSA_R = ((I2C->address&0xFF)<<1) | (I2C->operation &0x01);     /* Setting the I2C address */
    I2C0_MDR_R = data;                                                  /* Setting the data to be sent */
    I2C0_MCS_R |= I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;           /* Sending the start condition */
    while(I2C0_MCS_R & I2C_MCS_BUSY);                                   /* Waiting for the bus to be ready */
}
