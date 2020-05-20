#ifndef HY_I2C_H
#define HY_I2C_H

#define GPIO_TWI0_CLK   5
#define GPIO_TWI0_DATA  6


#define GPIO_TWI1_CLK   29
#define GPIO_TWI1_DATA  30



#define CW6301_Write_PS         (0x12 >> 1)  //write modr address
#define CW6301_Read_PS          (0x13 >> 1)  //read modr address

#define CW6301_OUT1                 0x0A
#define OUT1_Vol                 (0x80|0x37)  //2.7mV

#define CW6301_OUT2                 0x0B
#define OUT2_Vol                 (0x80|0x00)  //3.2mV

#define CW6301_OUT3                 0x0C
#define OUT3_Vol                 (0x80|0x1C)  //1.8mV

#define CW6301_OUT4                 0x0D
#define OUT4_Vol                 (0x80|0x1C)  //1.8mV

#define System_address      0x00
#define Charge_address      0x01
#define Interrupt_address   0x02


HY_initI2C();


#endif