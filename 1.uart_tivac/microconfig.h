#ifndef MICROCONFIG_H_
#define MICROCONFIG_H_
#include "tm4c123gh6pm.h"
#include <string.h>
#include <stdint.h>
#include "PLL.h"
#include "uart.h"



#define F_CPU 80000000 /* Internal frequency 80 MHz */

/* Enable IRQ Interrupts by clearing the I bit in the PRIMASK */
#define Enable_Interrupts()  __asm("    CPSIE I")
/* Disable IRQ Interrupts by setting the I bit in the PRIMASK */
#define Disable_Interrupts() __asm("    CPSID I")


#endif /* MICROCONFIG_H_ */
