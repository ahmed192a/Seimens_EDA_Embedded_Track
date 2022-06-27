/**
 * @file i2c.h
 * @author Ahmed Moahmed (ahmed.moahmed.eng.25@gmail.com)
 * @brief  This file contains the functions diclaration to configure and use the I2C.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef I2C_H_
#define I2C_H_



//Four I2C modes
typedef enum{
    Master_transmit,        //!< Master transmit
    Master_receive,         //!< Master receive
    Slave_transmit,         //!< Slave transmit
    Slave_receive           //!< Slave receive
}Modes;             

//Four transmission speeds:
typedef enum{
    Standard = 100000,              //!< Standard (100 Kbps)
    Fast_mode = 400000,             //!< Fast-mode (400 Kbps)
    Fast_mode_plus = 1000000,       //!< Fast-mode plus (1 Mbps)
    High_speed_mode = 333*10000     //!< High-speed mode (3.33 Mbps)
}Transmissiom_speed;

// Four modules of i2c
typedef enum{
    m0,
    m1,
    m2,
    m3
}Modules;

// Four functions of i2c
typedef enum{
    Glitch_filter,      //!< Glitch filter
    Slave,              //!< Slave
    Master,             //!< Master
    Loopback            //!< Loopback
}Functions;            

typedef struct{
    Modes mode;                 //!< I2C mode
    Transmissiom_speed speed;   //!< I2C transmission speed
    Modules module;             //!< I2C module
    Functions function;         //!< I2C function
    //TODO: create union for address and operation
    uint8_t address;            //!< I2C address
    uint8_t operation;          //!< I2C operation

}I2C_Config;


/*********************************************************************************/
/*                              Function Diclaration                             */
/*********************************************************************************/
void I2C_Init(const I2C_Config *I2C);
void I2C_Trasmit_byte(const I2C_Config *I2C, uint8_t data);

#endif /* I2C_H_ */
