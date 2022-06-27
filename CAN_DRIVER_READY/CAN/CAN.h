/**
 * @file CAN.h
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions definition to configure and use the CAN.
 * @version 0.1
 * @date 2022-04-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef CAN2_CAN_H_
#define CAN2_CAN_H_
#include "stdint.h"
#include "../tm4c123gh6pm.h"


/*******************************************************************************
 *                           SRUCTS & ENUMS                                    *
 *******************************************************************************/
/**
 * @brief  enum to select the CAN module
 */
typedef enum
{
    CAN_0,
    CAN_1
}CAN_type_t;

/**
 * @brief enum for CAN message ID
 */
typedef enum
{
    Standard,
    Extended
}MID_type_t;

/**
 * @brief  struct to configure the message of the CAN
 */
typedef struct
{
    uint32_t AcceptID;              /* Acceptance Filtering */
    uint32_t MessageID;             /* Message ID */
    MID_type_t ID_type;             /* MID type */
    uint8_t DLC;                    /* Size of the data frame */
    uint8_t MessageObjectID;        /* Number of Message Object*/
}CAN_MSGConfig_t;

/**
 * @brief struct to configure the bitrate of the CAN
 */
typedef struct
{
    uint8_t TSEG2;                  /* TSEG2 = Phase2 - 1 */
    uint8_t TSEG1;                  /* TSEG1 = Prop + Phase1 - 1 */
    uint8_t SJW;                    /* SJW - 1 */
    uint8_t BRP;                    /* BAUD RATE PRESCALER */

}BitRate_Config_t;

/**
 * @brief struct to hold the CAN configuration
 */
typedef struct
{
    CAN_MSGConfig_t MsgConfig;
    CAN_type_t CAN;
    BitRate_Config_t BitRate;
}Can_ConfigType_t;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void CAN_Init(Can_ConfigType_t *Config);
void Can_Set_Transmit(Can_ConfigType_t *Config);
void Can_Transmit(Can_ConfigType_t *Config, const uint8_t *Data);
void Can_Set_Receive(Can_ConfigType_t *Config);
void Can_Receive(Can_ConfigType_t *Config,  uint8_t *Data);
void CAN0_setCallBack(void(*a_ptr)(void));
void CAN1_setCallBack(void(*a_ptr)(void));


#endif /* CAN2_CAN_H_ */
