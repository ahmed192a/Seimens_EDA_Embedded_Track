/******************************************************************************
 *
 * Module: CAN
 *
 * File Name: CAN.c
 *
 * Description: Source file for CAN driver
 *
 * Author: Ahmed Mohamed
 *
 *******************************************************************************/


/*******************************************************************************
 *                          Library includes                                   *
 *******************************************************************************/
#include "CAN.h"

/*******************************************************************************
 *                           Local Macros                                      *
 *******************************************************************************/
#define NULL_PTR            ((void*)0)

/* CAN0 PORTB PINS */
#define CAN0_Rx_PB4          0x10
#define CAN0_Tx_PB5          0x20

/* CAN1 PORTA PINS */
#define CAN1_Rx_PA0          0x01
#define CAN1_Tx_PA1          0x02

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*can0_callBackPtr)(void) = NULL_PTR;
static volatile void (*can1_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                          Function Definitions                               *
 *******************************************************************************/

/******************************************************************************
 * Description : Initialize CAN BIT RATE and its GPIO PINS
 * Arguments   : Can_ConfigType_t *
 * Return      : none
 *****************************************************************************/
void CAN_Init(Can_ConfigType_t *Config)
{
    switch(Config->CAN)
    {

    case CAN_0:
        SYSCTL_RCGC0_R |= SYSCTL_RCGC0_CAN0;                                    /* Enable Clock For CAN 0 */

        /********************  Activate Port B for CAN 0  *************************/
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;                                   /* Activate Clock for PORT B */
        while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) == 0);                      /* Wait until the SYSCTL is enabled */
        GPIO_PORTB_CR_R |= (CAN0_Rx_PB4 | CAN0_Tx_PB5);                         /* Allow changes to PB4, PB5 in Commit Register*/
        GPIO_PORTB_AMSEL_R &= ~(CAN0_Rx_PB4 | CAN0_Tx_PB5);                     /* Disable Analog Function For PB4 & PB5 */
        GPIO_PORTB_DEN_R |= (CAN0_Rx_PB4 | CAN0_Tx_PB5);                        /* Enable Digital Select For PB4 & PB5 */
        GPIO_PORTB_DIR_R |= CAN0_Tx_PB5;                                        /* Direction Output for PB5 and Input for PB4 */
        GPIO_PORTB_AFSEL_R |= (CAN0_Rx_PB4 | CAN0_Tx_PB5);                      /* Enable Alternative Function Select For PB4 & PB5*/
        /* Use PB4 and CAN0 RX & PB5 as CAN0 TX  */
        GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & (GPIO_PCTL_PB4_M | GPIO_PCTL_PB5_M)) +(GPIO_PCTL_PB4_CAN0RX | GPIO_PCTL_PB5_CAN0TX);

        /************************* CONFIGURE BIT RATE *****************************/
        CAN0_CTL_R |= CAN_CTL_INIT;                                             /* Enable CAN0 Configure mode */
        while(CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY);                                 /* Wait while CAN0 Module is BUSY */

        /* Enabling Loop-back test mode (what will be sent will also be received)*/
        CAN0_CTL_R |= CAN_CTL_TEST;
        CAN0_TST_R |= CAN_TST_LBACK;


        CAN0_CTL_R |= CAN_CTL_CCE;                                              /* Setting the CCE bit to edit CANBIT Register*/
        CAN0_BIT_R |= Config->BitRate.BRP | (Config->BitRate.SJW<<6);
        CAN0_BIT_R |= (Config->BitRate.TSEG1<<8) | (Config->BitRate.TSEG2<<12);
        CAN0_CTL_R &= ~(CAN_CTL_INIT | CAN_CTL_CCE);                            /* Clear INIT BIT */
        break;

    case CAN_1:
        SYSCTL_RCGC0_R |= SYSCTL_RCGC0_CAN1;                                    /* Enable Clock For CAN 1 */

        /********************  Activate Port B for CAN 0  *************************/
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;                                   /* Activate Clock for PORT A */
        while ((SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0) == 0);                      /* Wait until the SYSCTL is enabled */
        GPIO_PORTA_CR_R |= (CAN1_Rx_PA0 | CAN1_Tx_PA1);                         /* Allow changes to PB4, PB5 in Commit Register*/
        GPIO_PORTA_AMSEL_R &= ~(CAN1_Rx_PA0 | CAN1_Tx_PA1);                     /* Disable Analog Select For PA0 & PA1 */
        GPIO_PORTA_DEN_R   |= (CAN1_Rx_PA0 | CAN1_Tx_PA1);                      /* Enable Digital Select For PA0 & PA1 */
        GPIO_PORTA_DIR_R |= CAN1_Tx_PA1;                                        /* Direction Output for PB5 and Input for PB4 */
        GPIO_PORTA_AFSEL_R |= (CAN1_Rx_PA0 | CAN1_Tx_PA1);                      /* Enable Alternative Function Select For PA0 & PA1 */
        /* Use PA0 and CAN1 RX & PA1 as CAN1 TX  */
        GPIO_PORTA_PCTL_R  =(GPIO_PORTA_PCTL_R & (GPIO_PCTL_PA0_M | GPIO_PCTL_PA1_M)) + (GPIO_PCTL_PA0_CAN1RX | GPIO_PCTL_PA1_CAN1TX);

        /************************* CONFIGURE BIT RATE *****************************/
        CAN1_CTL_R |= CAN_CTL_INIT;                                             /* Enable CAN1 Configure mode */
        while(CAN1_IF1CRQ_R & CAN_IF1CRQ_BUSY);                                 /* Wait while CAN0 Module is BUSY */

        CAN1_CTL_R |= CAN_CTL_CCE;                                              /* Setting the CCE bit to edit CANBIT Register*/
        CAN1_BIT_R |= Config->BitRate.BRP | (Config->BitRate.SJW<<6);
        CAN1_BIT_R |= (Config->BitRate.TSEG1<<8) | (Config->BitRate.TSEG2<<12);
        CAN1_CTL_R &= ~(CAN_CTL_INIT | CAN_CTL_CCE);                            /* CLEAR INIT BIT */
        break;
    }
}


/******************************************************************************
 * Description : Initialise the transmit message object
 * Arguments   : Can_ConfigType_t *
 * Return      : none
 *****************************************************************************/
void Can_Set_Transmit(Can_ConfigType_t *Config){
    switch(Config->CAN)
    {
    case CAN_0:

        while(CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY);                            /* wait while CAN0 Module is BUSY */

        CAN0_IF1CMSK_R |= CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_DATAB | CAN_IF1CMSK_DATAA | CAN_IF1CMSK_ARB;
        if (Config->MsgConfig.ID_type == Standard)
        {
            CAN0_IF1ARB2_R &= ~ CAN_IF1ARB2_XTD;                                /* Clear XTD Bit to Indicate Standard ID */
            CAN0_IF1ARB2_R |= (0x000007FF & Config->MsgConfig.MessageID) <<2 ;         /* Set message ID */
        }
        else if (Config->MsgConfig.ID_type == Extended)
        {
            CAN0_IF1ARB2_R |= CAN_IF1ARB2_XTD;                                  /* SET XTD Bit to Indicate Extended ID */

            /* Set message ID */
            CAN0_IF1ARB1_R |= (uint16_t) Config->MsgConfig.MessageID;
            CAN0_IF1ARB2_R |= ( CAN_IF2ARB2_ID_M & ( (Config->MsgConfig.MessageID)>>16) );

        }
        CAN0_IF1ARB2_R = CAN_IF1ARB2_DIR | CAN_IF1ARB2_MSGVAL;

        CAN0_IF1MCTL_R |= CAN_IF1MCTL_RMTEN;                                    /* Set RMTEN BIT to Set TXRQST on Receiving a matching Frame */
        CAN0_IF1MCTL_R |= ( CAN_IF1MCTL_DLC_M &  Config->MsgConfig.DLC);        /* Specify the size of the data frame */

        /* Selects one of the 32 message objects in the message RAM for data transfer */
        CAN0_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
        break;
    case CAN_1:

        while(CAN1_IF1CRQ_R & CAN_IF1CRQ_BUSY);                            /* wait while CAN1 Module is BUSY */

        CAN1_IF1CMSK_R |= CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_DATAB | CAN_IF1CMSK_DATAA | CAN_IF1CMSK_ARB;
        if (Config->MsgConfig.ID_type == Standard)
        {
            CAN1_IF1ARB2_R &= ~ CAN_IF1ARB2_XTD;                                /* Clear XTD Bit to Indicate Standard ID */
            CAN1_IF1ARB2_R |= (0x000007FF & Config->MsgConfig.MessageID) <<2 ;         /* Set message ID */
        }
        else if (Config->MsgConfig.ID_type == Extended)
        {
            CAN1_IF1ARB2_R |= CAN_IF1ARB2_XTD;                                  /* SET XTD Bit to Indicate Extended ID */

            /* Set message ID */
            CAN1_IF1ARB1_R |= (uint16_t) Config->MsgConfig.MessageID;
            CAN1_IF1ARB2_R |= ( CAN_IF2ARB2_ID_M & ( (Config->MsgConfig.MessageID)>>16) );

        }
        CAN1_IF1ARB2_R = CAN_IF1ARB2_DIR | CAN_IF1ARB2_MSGVAL;

        CAN1_IF1MCTL_R |= CAN_IF1MCTL_RMTEN;                                    /* Set RMTEN BIT to Set TXRQST on Receiving a matching Frame */
        CAN1_IF1MCTL_R |= ( CAN_IF1MCTL_DLC_M &  Config->MsgConfig.DLC);        /* Specify the size of the data frame */

        /* Selects one of the 32 message objects in the message RAM for data transfer */
        CAN1_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
        break;
    }

}



/******************************************************************************
 * Description : This function sends the data on the TX pin.
 * Arguments   : Can_ConfigType_t * , uint8_t *
 * Return      : none
 *****************************************************************************/
void Can_Transmit(Can_ConfigType_t *Config, const uint8_t *DataByte)
{

    switch(Config->CAN)
    {

    case CAN_0:

        CAN0_IF1CMSK_R &= ~(CAN_IF1CMSK_ARB | CAN_IF1CMSK_CONTROL); //Clear ARB and CONTROL bits in CMSK reg
        CAN0_IF1CMSK_R |= CAN_IF1CMSK_WRNRD;                                    /* Set the WRNRD bit to specify a write */
        CAN0_IF1CMSK_R |= CAN_IF1CMSK_DATAB | CAN_IF1CMSK_DATAA;                /* Specify which bits to transfer using the DATAA and DATAB bits*/

        switch( Config->MsgConfig.DLC)
        {

        case 0x0:
            /* EMPTY DATA FRAME */
            break;
        case 0x1:
            CAN0_IF1DA1_R =  DataByte[0];
            break;
        case 0x2:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            break;
        case 0x3:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2];
            break;
        case 0x4:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
            break;
        case 0x5:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
            CAN0_IF1DB1_R =  DataByte[4];
            break;
        case 0x6:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
            CAN0_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
            break;
        case 0x7:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
            CAN0_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
            CAN0_IF1DB2_R =  DataByte[6];
            break;
        case 0x8:
        default:
            CAN0_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
            CAN0_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
            CAN0_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
            CAN0_IF1DB2_R =  DataByte[6] | (DataByte[7]<<8);
            break;
        }

        CAN0_IF1CMSK_R |= CAN_IF1CMSK_NEWDAT | CAN_IF1CMSK_TXRQST; //Set the NEWDAT and TXRQST bit in CMSK reg

        /* Selects one of the 32 message objects in the message RAM for data transfer */
        CAN0_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
        while(CAN0_IF1CRQ_R & CAN_IF1CRQ_BUSY);                            /* Wait for BUSY bit to clear */

        break;

        case CAN_1:

            CAN1_IF1CMSK_R &= ~(CAN_IF1CMSK_ARB | CAN_IF1CMSK_CONTROL); //Clear ARB and CONTROL bits in CMSK reg
            CAN1_IF1CMSK_R |= CAN_IF1CMSK_WRNRD;                                    /* Set the WRNRD bit to specify a write */
            CAN1_IF1CMSK_R |= CAN_IF1CMSK_DATAB | CAN_IF1CMSK_DATAA;                /* Specify which bits to transfer using the DATAA and DATAB bits*/

            switch( Config->MsgConfig.DLC)
            {

            case 0x0:
                /* EMPTY DATA FRAME */
                break;
            case 0x1:
                CAN1_IF1DA1_R =  DataByte[0];
                break;
            case 0x2:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                break;
            case 0x3:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2];
                break;
            case 0x4:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
                break;
            case 0x5:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
                CAN1_IF1DB1_R =  DataByte[4];
                break;
            case 0x6:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
                CAN1_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
                break;
            case 0x7:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
                CAN1_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
                CAN1_IF1DB2_R =  DataByte[6];
                break;
            case 0x8:
            default:
                CAN1_IF1DA1_R =  DataByte[0] | (DataByte[1]<<8);
                CAN1_IF1DA2_R =  DataByte[2] | (DataByte[3]<<8);
                CAN1_IF1DB1_R =  DataByte[4] | (DataByte[5]<<8);
                CAN1_IF1DB2_R =  DataByte[6] | (DataByte[7]<<8);
                break;
            }

            CAN1_IF1CMSK_R |= CAN_IF1CMSK_NEWDAT | CAN_IF1CMSK_TXRQST; //Set the NEWDAT and TXRQST bit in CMSK reg

            /* Selects one of the 32 message objects in the message RAM for data transfer */
            CAN1_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
            while(CAN1_IF1CRQ_R & CAN_IF1CRQ_BUSY);                            /* Wait for BUSY bit to clear */
            break;
    }
}





/******************************************************************************
 * Description : Initialize the Receive message object
 * Arguments   : Can_ConfigType_t *
 * Return      : none
 *****************************************************************************/

void Can_Set_Receive(Can_ConfigType_t *Config){
    //Initialise the Receive message object
    switch(Config->CAN){
    case CAN_0:
        while(CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY);//wait while CAN0 Module is BUSY

        CAN0_IF2CMSK_R |= CAN_IF2CMSK_WRNRD | CAN_IF2CMSK_MASK | CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_DATAB | CAN_IF2CMSK_DATAA;
        if (Config->MsgConfig.ID_type == Standard)
        {
            CAN0_IF2ARB2_R &= ~ CAN_IF2ARB2_XTD;                                /* Clear XTD Bit to Indicate Standard ID */
            CAN0_IF2MSK2_R &= ~ CAN_IF2MSK2_MXTD;                               /* Clear MXTD to Indicate That Acceptance Filtering is 11 BITS */

            CAN0_IF2MSK2_R |= (0x000007FF & Config->MsgConfig.AcceptID)  <<2 ;         /* Set Message Filtering ID */
            CAN0_IF2ARB2_R |= (0x000007FF & Config->MsgConfig.MessageID) <<2 ;         /* Set message ID */


        }
        else if (Config->MsgConfig.ID_type == Extended)
        {
            CAN0_IF2ARB2_R |= CAN_IF2ARB2_XTD;                                  /* SET XTD Bit to Indicate Extended ID */
            CAN0_IF2MSK2_R |= CAN_IF2MSK2_MXTD;                                 /* Set MXTD to Indicate That Acceptance Filtering is 29 BITS */

            /* Set Message Filtering ID */
            CAN0_IF2MSK1_R |= (uint16_t) Config->MsgConfig.AcceptID;
            CAN0_IF2MSK2_R |= ( CAN_IF2MSK2_IDMSK_M & ( (Config->MsgConfig.AcceptID)>>16) );

            /* Set message ID */
            CAN0_IF2ARB1_R |= (uint16_t) Config->MsgConfig.MessageID;
            CAN0_IF2ARB2_R |= ( CAN_IF2ARB2_ID_M & ( (Config->MsgConfig.MessageID)>>16) );

        }
        CAN0_IF2ARB2_R |= CAN_IF2ARB2_MSGVAL;
        CAN0_IF2MCTL_R = CAN_IF2MCTL_UMASK | CAN_IF2MCTL_EOB;
        CAN0_IF2MCTL_R |= ( CAN_IF2MCTL_DLC_M &  Config->MsgConfig.DLC);        /* Specify the size of the data frame */

        /* Selects one of the 32 message objects in the message RAM for data transfer */
        CAN0_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
        while(CAN0_IF2CRQ_R & CAN_IF1CRQ_BUSY);//wait while CAN0 Module is BUSY
        break;
    case CAN_1:
        while(CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY);//wait while CAN1 Module is BUSY

        CAN1_IF2CMSK_R |= CAN_IF2CMSK_WRNRD | CAN_IF2CMSK_MASK | CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_DATAB | CAN_IF2CMSK_DATAA;
        if (Config->MsgConfig.ID_type == Standard)
        {
            CAN1_IF2ARB2_R &= ~ CAN_IF2ARB2_XTD;                                /* Clear XTD Bit to Indicate Standard ID */
            CAN1_IF2MSK2_R &= ~ CAN_IF2MSK2_MXTD;                               /* Clear MXTD to Indicate That Acceptance Filtering is 11 BITS */

            CAN1_IF2MSK2_R |= (0x000007FF & Config->MsgConfig.AcceptID)  <<2 ;         /* Set Message Filtering ID */
            CAN1_IF2ARB2_R |= (0x000007FF & Config->MsgConfig.MessageID) <<2 ;         /* Set message ID */


        }
        else if (Config->MsgConfig.ID_type == Extended)
        {
            CAN1_IF2ARB2_R |= CAN_IF2ARB2_XTD;                                  /* SET XTD Bit to Indicate Extended ID */
            CAN1_IF2MSK2_R |= CAN_IF2MSK2_MXTD;                                 /* Set MXTD to Indicate That Acceptance Filtering is 29 BITS */

            /* Set Message Filtering ID */
            CAN1_IF2MSK1_R |= (uint16_t) Config->MsgConfig.AcceptID;
            CAN1_IF2MSK2_R |= ( CAN_IF2MSK2_IDMSK_M & ( (Config->MsgConfig.AcceptID)>>16) );

            /* Set message ID */
            CAN1_IF2ARB1_R |= (uint16_t) Config->MsgConfig.MessageID;
            CAN1_IF2ARB2_R |= ( CAN_IF2ARB2_ID_M & ( (Config->MsgConfig.MessageID)>>16) );

        }
        CAN1_IF2ARB2_R |= CAN_IF2ARB2_MSGVAL;
        CAN1_IF2MCTL_R = CAN_IF2MCTL_UMASK | CAN_IF2MCTL_EOB;
        CAN1_IF2MCTL_R |= ( CAN_IF2MCTL_DLC_M &  Config->MsgConfig.DLC);        /* Specify the size of the data frame */

        /* Selects one of the 32 message objects in the message RAM for data transfer */
        CAN1_IF1CRQ_R  |= (Config->MsgConfig.MessageObjectID & CAN_IF1CRQ_MNUM_M);
        while(CAN1_IF2CRQ_R & CAN_IF1CRQ_BUSY);//wait while CAN1 Module is BUSY
        break;
    }

}

/******************************************************************************
 * Description : This function receives the data put on the RX pin.
 * Arguments   : Can_ConfigType_t * , uint8_t *
 * Return      : none
 *****************************************************************************/
void Can_Receive(Can_ConfigType_t *Config,  uint8_t *DataByte)
{
    switch(Config->CAN)
    {

    case CAN_0:

        CAN0_IF2CMSK_R = CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_MASK | CAN_IF2CMSK_DATAA | CAN_IF2CMSK_DATAB; //ARB, CONTROL, MASK, DATAA, DATAB bits set
        /* Selects one of the 32 message objects in the message RAM to receive from */
        CAN0_IF2CRQ_R |= (Config->MsgConfig.MessageObjectID & CAN_IF2CRQ_MNUM_M);
        while(CAN0_IF2CRQ_R & CAN_IF2CRQ_BUSY);//wait while CAN0 Module is BUSY

        if(CAN0_IF2MCTL_R & CAN_IF2MCTL_NEWDAT) //If NEWDAT bit is set -> there is new data in the data registers
        {
            DataByte[0] = ( 0x00FF & CAN0_IF2DA1_R );
            DataByte[1] = ((0xFF00 & CAN0_IF2DA1_R ) >> 8);
            DataByte[2] = ( 0x00FF & CAN0_IF2DA2_R );
            DataByte[3] = ((0xFF00 & CAN0_IF2DA2_R ) >> 8);
            DataByte[4] = ( 0x00FF & CAN0_IF2DB1_R );
            DataByte[5] = ((0xFF00 & CAN0_IF2DB1_R ) >> 8);
            DataByte[6] = ( 0x00FF & CAN0_IF2DB2_R );
            DataByte[7] = ((0xFF00 & CAN0_IF2DB2_R ) >> 8);

            CAN0_IF2CMSK_R |= CAN_IF2CMSK_NEWDAT;//Set NEWDAT in CMSK to Clear the NEWDAT bit in MCTL
        }

        if(CAN0_IF2MCTL_R & CAN_IF2MCTL_MSGLST) //If MSGLST bit is set -> there was a message lost
        {
            CAN0_IF2MCTL_R &=~ CAN_IF2MCTL_MSGLST; //Clear the MSGLST bit
        }

        /* Selects one of the 32 message objects in the message RAM to receive from */
        CAN0_IF2CRQ_R |= (Config->MsgConfig.MessageObjectID & CAN_IF2CRQ_MNUM_M);

        break;

    case CAN_1:


        CAN1_IF2CMSK_R = CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL | CAN_IF2CMSK_MASK | CAN_IF2CMSK_DATAA | CAN_IF2CMSK_DATAB; //ARB, CONTROL, MASK, DATAA, DATAB bits set
        /* Selects one of the 32 message objects in the message RAM to receive from */
        CAN1_IF2CRQ_R |= (Config->MsgConfig.MessageObjectID & CAN_IF2CRQ_MNUM_M);
        while(CAN1_IF2CRQ_R & CAN_IF2CRQ_BUSY);//wait while CAN1 Module is BUSY

        if(CAN1_IF2MCTL_R & CAN_IF2MCTL_NEWDAT) //If NEWDAT bit is set i.e. there is new data in the data registers
        {
            DataByte[0] = (0x00FF & CAN1_IF2DA1_R); //Get this new data.
            DataByte[1] = ((0xFF00 & CAN1_IF2DA1_R)>>8);
            DataByte[2] = (0x00FF & CAN1_IF2DA2_R);
            DataByte[3] = ((0xFF00 & CAN1_IF2DA2_R)>>8);
            DataByte[4] = (0x00FF & CAN1_IF2DB1_R);
            DataByte[5] = ((0xFF00 & CAN1_IF2DB1_R)>>8);
            DataByte[6] = (0x00FF & CAN1_IF2DB2_R);
            DataByte[7] = ((0xFF00 & CAN1_IF2DB2_R)>>8);

            CAN1_IF2CMSK_R |= CAN_IF2CMSK_NEWDAT;//Set NEWDAT in CMSK to Clear the NEWDAT bit in MCTL
        }

        if(CAN1_IF2MCTL_R & CAN_IF2MCTL_MSGLST) //If MSGLST bit is set i.e. there was a message lost
        {
            CAN1_IF2MCTL_R &=~ CAN_IF2MCTL_MSGLST; //Clear the MSGLST bit
        }

        /* Selects one of the 32 message objects in the message RAM to receive from */
        CAN1_IF2CRQ_R |= (Config->MsgConfig.MessageObjectID & CAN_IF2CRQ_MNUM_M);
        break;
    }


}






/************************************************************************************
                                 Interrupt Handler
 ************************************************************************************/
void CAN0_Handler(void)
{
    /* CALL BACK FN. */
    if(can0_callBackPtr != NULL_PTR)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*can0_callBackPtr)();
    }
}




void CAN1_Handler(void)
{
    if(can1_callBackPtr != NULL_PTR)
    {
        /* Call the Call Back function in the application after the edge is detected */
        (*can1_callBackPtr)();
    }
}

/************************************************************************************
                                 CALL BACK FUNCTIONS
 ************************************************************************************/
void CAN0_setCallBack(void(*a_ptr)(void))
{
    /* Save the address of the Call back function in a global variable */
    can0_callBackPtr = a_ptr;
}
void CAN1_setCallBack(void(*a_ptr)(void))
{
    /* Save the address of the Call back function in a global variable */
    can1_callBackPtr = a_ptr;
}
