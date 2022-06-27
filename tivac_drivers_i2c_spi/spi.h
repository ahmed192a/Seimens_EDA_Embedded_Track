/**
 * @file spi.h
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions diclaration to configure and use the SPI.
 * @version 0.1
 * @date 2022-06-27
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef _spi_h
#define _spi_h

#include "micro_config.h"

 /*******************************************************************************
  *                      Functions Prototypes                                   *
  *******************************************************************************/

void SPI_EnableChipSelect(void); 
void SPI_DisableChipSelect(void) ; 
void init_SSI0(void) ; 
uint8_t SSI_Write(uint8_t data) ; 
uint8_t SSI_Read(void) ; 


#endif 

