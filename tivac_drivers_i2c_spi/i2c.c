/*
 * i2c.c
 *
 *  Created on: Aug 26, 2021
 *      Author: ah132
 */

#include "micro_config.h"


void I2C_Init(const I2C_Config *I2C){
    switch(I2C->module){
    case m0:
        SYSCTL_RCGCI2C_R |= SYSCTL_RCGCI2C_R0;
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
        while ((SYSCTL_PRGPIO_R & SYSCTL_RCGC2_GPIOB) == 0);
        GPIO_PORTB_AFSEL_R |= 0xC0;
        GPIO_PORTB_ODR_R   |= P6 | P7;                          /* Enable open drain on pins 6, 7 */
        GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&(~(GPIO_PCTL_PB6_M|GPIO_PCTL_PB7_M)))+(GPIO_PCTL_PB7_T0CCP1|GPIO_PCTL_PB6_T0CCP0);
        GPIO_PORTB_DEN_R   |= P6 | P7;                          /* Enable Digital on pins 6, 7 */
        GPIO_PORTB_AMSEL_R &= (~P7) & (~P6);                    /* Disable Analog on pins 6, 7 */
        switch(I2C->function)
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
        if(I2C->mode == High_speed_mode){
            I2C0_MTPR_R |= I2C_MTPR_HS;
        }
        else{
            I2C0_MTPR_R &= ~I2C_MTPR_HS;
        }
        uint8_t TPR = (SysClk/(2*(6+4)*I2C->mode))-1;
        I2C0_MTPR_R = (I2C0_MTPR_R & (~0x7F)) | (TPR & 0x7F);
        I2C0_MSA_R = ((I2C->address&0xFF)<<1) | (I2C->operation &0x01);
    break;

    }

}

void I2C_Trasmit_byte(const I2C_Config *I2C, uint8_t data){
    I2C0_MSA_R = ((I2C->address&0xFF)<<1) | (I2C->operation &0x01);
    I2C0_MDR_R = data;
    I2C0_MCS_R |= I2C_MCS_RUN | I2C_MCS_START | I2C_MCS_STOP;
    while(I2C0_MCS_R & I2C_MCS_BUSY);
}
