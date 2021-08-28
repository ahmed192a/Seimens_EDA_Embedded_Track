/*
 * i2c.h
 *
 *  Created on: Aug 26, 2021
 *      Author: ah132
 */

#ifndef I2C_H_
#define I2C_H_



//Four I2C modes
//– Master transmit
//– Master receive
//– Slave transmit
//– Slave receive

typedef enum{
    Master_transmit,
    Master_receive,
    Slave_transmit,
    Slave_receive
}Modes;

//Four transmission speeds:
//– Standard (100 Kbps)
//– Fast-mode (400 Kbps)
//– Fast-mode plus (1 Mbps)
//– High-speed mode (3.33 Mbps)

typedef enum{
    Standard = 100000,
    Fast_mode = 400000,
    Fast_mode_plus = 1000000,
    High_speed_mode = 333*10000
}Transmissiom_speed;

// Four modules of i2c
typedef enum{
    m0,
    m1,
    m2,
    m3
}Modules;


typedef enum{
    Glitch_filter,
    Slave,
    Master,
    Loopback
}Functions;

typedef struct{
    Modes mode;
    Transmissiom_speed speed;
    Modules module;
    Functions function;
    // create union for address and operation
    uint8_t address;
    uint8_t operation;

}I2C_Config;



#endif /* I2C_H_ */
