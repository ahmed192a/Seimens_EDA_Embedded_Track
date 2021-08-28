#include "spi.h"

#define	CS	(1U<<3)		       /* Chip Select(PF.3)*/

/*******************************************************************************
 *                       Function Declaration                                  *
 *******************************************************************************/

/******************************************************************************
  * Description : SPI_EnableChipSelect 
  * Arguments   : none
  * Return      : none
  *****************************************************************************/

void SPI_EnableChipSelect(void)
{
	 GPIO_PORTF_DATA_R &= ~CS ;
}

/******************************************************************************
  * Description : SPI_DisableChipSelect
  * Arguments   : none
  * Return      : none
  *****************************************************************************/
void SPI_DisableChipSelect(void)
{
	 GPIO_PORTF_DATA_R |= CS ;
}

/******************************************************************************
  * Description : Initialize SSI0 Module, Powers ON, Configures Pins, Sets up Clock, FIFO length = 1 byte
  * Arguments   : none
  * Return      : none
  *****************************************************************************/

void init_SSI0(void)
{
    SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R0;
	//SYSCTL_RCGCSSI |= (1<<0) ; 			                               /*Enable SSI0 Clock*/
    //SYSCTL_RCGCGPIO_R =
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R5 ;	      /*Enable GPIOA & GPIOF Clock*/
	while (!(SYSCTL_PRGPIO_R & (SYSCTL_PRGPIO_R5|SYSCTL_PRGPIO_R0))) ;    /* wait for GPIO A,F to be peripheral ready*/


	GPIO_PORTA_AFSEL_R |=  P2 | P3  | P4  | P5;
	GPIO_PORTA_PCTL_R  |=  GPIO_PCTL_PA2_SSI0CLK| GPIO_PCTL_PA3_SSI0FSS| GPIO_PCTL_PA4_SSI0RX| GPIO_PCTL_PA5_SSI0TX; 		/* Pin Mux selection for SSI0*/
	GPIO_PORTA_AMSEL_R &= ~(P2 | P3  | P4  | P5) ; 		    			/* Disable Analog mode select for 2,3,4,5*/
	GPIO_PORTA_DEN_R   |=  P2 | P3  | P4  | P5;   		                /* Digital Enable for 2,3,4,5*/
    GPIO_PORTA_DIR_R   &= ~P4 ;

	/* GPIOF Configuraion for CS */
	GPIO_PORTF_DIR_R   |=  CS;
	GPIO_PORTF_DEN_R   |=  CS;
	GPIO_PORTF_AMSEL_R &= ~CS;

	 
	SSI0_CR1_R &= ~SSI_CR1_SSE ; 					                        /* Disable SSI0*/
	SSI0_CR1_R &= ~SSI_CR1_MS ; 					                        /* Master of Puppets*/
	
	SSI0_CC_R = (SSI0_CC_R & ~(0X0F)) + 0x00 ;		                    /* SYSCLK => Avoid modifying reserved bits*/
	
	/** Kick up the clock **/ 
	SSI0_CPSR_R = (SSI0_CPSR_R & ~(0xFF))+ 0xc8 ;		                /* 5MHz CLK = 5000000/10 ; BR=SysClk/(CPSDVSR * (1 + SCR)) 0x7D*/
  
	/** Enable SPI ***/ 
	SSI0_CR0_R =  SSI_CR0_DSS_8; 			                           /* SCR = 0 ; SPHA = 0 ; SPOL = 0 ; Sample on rising , shift on falling ; Freescale SPI Mode*/
	SSI0_CR1_R |= SSI_CR1_SSE ; 	                                           /* Enable SSI*/
}


/******************************************************************************
  * Description : Transmits a byte of Data
  * Arguments   : Data to be transmitted (1 byte) 
  * Return      : Transmitted Byte (required in SD Driver
  *****************************************************************************/

uint8_t SSI_Write(uint8_t data) 
{
	volatile uint8_t rcv_data ;
  	while((SSI0_SR_R & SSI_SR_TNF)==0);  			 /* wait till Tx FIFO FULL*/
	SSI0_DR_R = data;
	while((SSI0_SR_R & SSI_SR_RNE)==0);				 /* wait till Receiver Not Empty*/
	rcv_data = SSI0_DR_R ;  					 /** Read DR to clear flags and avoid bugs in any SPI Driver **/
  	return data;
}


/******************************************************************************
  * Description : Receive a byte of Data, Transmits out 0xFF (Required in SD Inteface)
  * Arguments   : Data received (1 byte)
  * Return      : none
  *****************************************************************************/
uint8_t SSI_Read(void) 
{
  	while((SSI0_SR_R & SSI_SR_TNF)==0)	;  		    /* wait till Tx FIFO FULL*/
	SSI0_DR_R = 0xFF ;
	while((SSI0_SR_R & SSI_SR_RNE)==0) ;			/* wait till Receiver Not Empty*/
	return (uint8_t)SSI0_DR_R ;
}


