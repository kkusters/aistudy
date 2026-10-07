// CH_I2C.H

#ifndef _CH_I2C_H
#define _CH_I2C_H

#include "ch_define.h"

#define IIC_CHANNEL_0_SELECTED 0
#define IIC_CHANNEL_1_SELECTED 1

#define BAUDRATE_100KHZ 0
#define BAUDRATE_400KHZ 1

#define IIC_ONE_SUB_ADDRESS 0
#define IIC_TWO_SUB_ADDRESSES 1

#define IIC_READY 0
#define IIC_ERROR 0xFF

extern unsigned char iic_status;

unsigned char IIC_Write(unsigned char address,           // addres = address from iic component
                        unsigned char sub_address_0,     // sub_addres_0 = sub address from iic component
                        unsigned char sub_address_1,     // sub_addres_1 = sub address from iic component
                        unsigned char two_sub_addresses, // two_sub_address = 0 only 1 sub address, 1 2 sub addresses
                        unsigned char *data,        // data = pointer to array with data to send
                        unsigned char nr);               // nr = number of bytes to send
unsigned char IIC_Read(unsigned char address,           // addres = address from iic component
                       unsigned char sub_address_0,     // sub_addres_0 = sub address from iic component
                       unsigned char sub_address_1,     // sub_addres_1 = sub address from iic component
                       unsigned char two_sub_addresses, // two_sub_address = 0 only 1 sub address, 1 2 sub addresses
                       unsigned char *data,        // data = pointer to array with data to send
                       unsigned char nr);               // nr = number of bytes to send

#endif //  _CH_I2C_H