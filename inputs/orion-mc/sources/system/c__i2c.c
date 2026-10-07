// C__I2C.C

#include "ch_define.h"

#include "ch_alarm.h"
#include "ch_test.h"
#include "ch_i2c.h"

#if (CLKFREQ==16000000L)
  #define SET_IIC_ADR_FOR_100KHZ 0x0000
  #define SET_IIC_CFG_FOR_100KHZ 0x2700
  #define SET_IIC_ADR_FOR_400KHZ 0x0000
  #define SET_IIC_CFG_FOR_400KHZ 0x0900
#endif // (CLKFREQ==16000000L)
#if (CLKFREQ==32000000L)
  #define SET_IIC_ADR_FOR_100KHZ 0x0000
  #define SET_IIC_CFG_FOR_100KHZ 0x4F00
  #define SET_IIC_ADR_FOR_400KHZ 0x0000
  #define SET_IIC_CFG_FOR_400KHZ 0x1300
#endif // (CLKFREQ==32000000L)
#if (CLKFREQ==40000000L)
  #define SET_IIC_ADR_FOR_100KHZ 0x0000
  #define SET_IIC_CFG_FOR_100KHZ 0x6300
  #define SET_IIC_ADR_FOR_400KHZ 0x0000
  #define SET_IIC_CFG_FOR_400KHZ 0x1800
#endif // (CLKFREQ==40000000L)

#define IIC_CHANNEL_0 0x0011
#define IIC_CHANNEL_1 0x0022

// I2C0 is used for IC's on the CPU board used at high speed (400khz)
// EEPROM (options/setpoints)
// EEPROM (language)
// RTC
#define IIC0_SDA P9_0
#define IIC0_SDA_DP DP9_0
#define IIC0_SDA_ODP ODP9_0
#define IIC0_SDA_ALTSEL0 AS0P9_0
#define IIC0_SDA_ALTSEL1 AS1P9_0
#define IIC0_SCL P9_1
#define IIC0_SCL_DP DP9_1
#define IIC0_SCL_ODP ODP9_1
#define IIC0_SCL_ALTSEL0 AS0P9_1
#define IIC0_SCL_ALTSEL1 AS1P9_1

// I2C1 is on the connector for future options don't used it at high speed (speed <= 100khz)
#define IIC1_SDA P9_2
#define IIC1_SDA_DP DP9_2
#define IIC1_SDA_ODP ODP9_2
#define IIC1_SDA_ALTSEL0 AS0P9_2
#define IIC1_SDA_ALTSEL1 AS1P9_2
#define IIC1_SCL P9_3
#define IIC1_SCL_DP DP9_3
#define IIC1_SCL_ODP ODP9_3
#define IIC1_SCL_ALTSEL0 AS0P9_3
#define IIC1_SCL_ALTSEL1 AS1P9_3

// IIC_ST bits
#define IIC_ST_ADR_MASK      ((unsigned int)0x0001)
#define IIC_ST_AL_MASK       ((unsigned int)0x0002)
#define IIC_ST_SLA_MASK      ((unsigned int)0x0004)
#define IIC_ST_LRB_MASK      ((unsigned int)0x0008)
#define IIC_ST_BB_MASK       ((unsigned int)0x0010)
#define IIC_ST_IRQD_MASK     ((unsigned int)0x0020)
#define IIC_ST_IRQP_MASK     ((unsigned int)0x0040)
#define IIC_ST_IRQE_MASK     ((unsigned int)0x0080)
#define IIC_ST_CO_MASK       ((unsigned int)0x0700)
#define IIC_ST_CO_0_BYTES    ((unsigned int)0x0000)
#define IIC_ST_CO_1_BYTES    ((unsigned int)0x0100)
#define IIC_ST_CO_2_BYTES    ((unsigned int)0x0200)
#define IIC_ST_CO_3_BYTES    ((unsigned int)0x0300)
#define IIC_ST_CO_4_BYTES    ((unsigned int)0x0400)

// IIC_CON bits
#define IIC_CON_M10_MASK                    ((unsigned int)0x0001)
#define IIC_CON_RSC_MASK                    ((unsigned int)0x0002)
#define IIC_CON_MOD_MASK                    ((unsigned int)0x000C)
#define IIC_CON_MOD_DISABLE                 ((unsigned int)0x0000)
#define ICC_CON_MOD_SLAVE                   ((unsigned int)0x0004)
#define IIC_CON_MODE_SINGLE_MASTER          ((unsigned int)0x0008)
#define IIC_CON_MODE_MULTI_MASTER           ((unsigned int)0x000C)
#define IIC_CON_BUM_MASK                    ((unsigned int)0x0010)
#define IIC_CON_ACKDIS_MASK                 ((unsigned int)0x0020)
#define IIC_CON_INT_MASK                    ((unsigned int)0x0040)
#define IIC_CON_TRX_MASK                    ((unsigned int)0x0080)
#define IIC_CON_IGE_MASK                    ((unsigned int)0x0100)
#define IIC_CON_STP_MASK                    ((unsigned int)0x0200)
#define IIC_CON_CI_MASK                     ((unsigned int)0x0C00)
#define IIC_CON_CI_TRANSMIT_BUFFER_LENGTH_1 ((unsigned int)0x0000) 
#define IIC_CON_CI_TRANSMIT_BUFFER_LENGTH_2 ((unsigned int)0x0400) 
#define IIC_CON_CI_TRANSMIT_BUFFER_LENGTH_3 ((unsigned int)0x0800) 
#define IIC_CON_CI_TRANSMIT_BUFFER_LENGTH_4 ((unsigned int)0x0C00) 

unsigned char iic_status = IIC_ERROR;
//unsigned int iic_irqd_cnt = 0;
//unsigned int iic_irqe_cnt = 0;
//unsigned int iic_irqp_cnt = 0;
//unsigned int iic_data_cnt = 0;
//unsigned int iic_protocol_cnt = 0;
unsigned char iic_address;
unsigned char iic_sub_address_0;
unsigned char iic_sub_address_1;
unsigned char iic_two_sub_addresses;
unsigned char iic_number_of_bytes;
unsigned char *iic_data;

void I2C_Select_Channel(unsigned char channel, unsigned char baudrate)
{
  if (baudrate == BAUDRATE_100KHZ)
  {
    IIC_ADR = SET_IIC_ADR_FOR_100KHZ;
    IIC_CFG = (channel == IIC_CHANNEL_1_SELECTED) ? SET_IIC_CFG_FOR_100KHZ | IIC_CHANNEL_1 : SET_IIC_CFG_FOR_100KHZ | IIC_CHANNEL_0;
  }
  else // baudrate = BAUDRATE_400KHZ
  {
    IIC_ADR = SET_IIC_ADR_FOR_400KHZ;
    IIC_CFG = (channel == IIC_CHANNEL_1_SELECTED) ? SET_IIC_CFG_FOR_400KHZ | IIC_CHANNEL_1 : SET_IIC_CFG_FOR_400KHZ | IIC_CHANNEL_0;
  }
}

void IIC0_Generate_Stop_Condition(void)
{
  IIC0_SDA_ALTSEL0 = 0;
  IIC0_SDA_ALTSEL1 = 0;
  IIC0_SDA = 1;
  IIC0_SDA_DP = OUTPUT;
  IIC0_SDA_ODP = OPEN_DRAIN;

  IIC0_SCL_ALTSEL0 = 0;
  IIC0_SCL_ALTSEL1 = 0;
  IIC0_SCL = 1;
  IIC0_SCL_DP = OUTPUT;
  IIC0_SCL_ODP = OPEN_DRAIN;
  IIC0_SDA = 0;
  IIC0_SCL = 0;
  _nop(); _nop(); _nop(); _nop(); _nop();
  IIC0_SCL = 1;
  _nop(); _nop(); _nop(); _nop(); _nop();
  IIC0_SDA = 1;
}

void IIC1_Generate_Stop_Condition(void)
{
  IIC1_SDA_ALTSEL0 = 0;
  IIC1_SDA_ALTSEL1 = 0;
  IIC1_SDA = 1;
  IIC1_SDA_DP = OUTPUT;
  IIC1_SDA_ODP = OPEN_DRAIN;

  IIC1_SCL_ALTSEL0 = 0;
  IIC1_SCL_ALTSEL1 = 0;
  IIC1_SCL = 1;
  IIC1_SCL_DP = OUTPUT;
  IIC1_SCL_ODP = OPEN_DRAIN;
  IIC1_SDA = 0;
  IIC1_SCL = 0;
  _nop(); _nop(); _nop(); _nop(); _nop();
  IIC1_SCL = 1;
  _nop(); _nop(); _nop(); _nop(); _nop();
  IIC1_SDA = 1;
}

void I2C_Init(void)
{           
  IIC0_Generate_Stop_Condition();
  IIC1_Generate_Stop_Condition();
  
  P9LIN = 1; // inputs from P9 as special threshold (chapter 7.1)
  POCON9 &= 0xFFF0; // outputs form P9 strong driver, sharp edge mode (chapter 7.3)

  // Set port SDA i2c_0
  IIC0_SDA_ALTSEL0 = 1;
  IIC0_SDA_ALTSEL1 = 1;
  IIC0_SDA = 1;
  IIC0_SDA_DP = OUTPUT;
  IIC0_SDA_ODP = OPEN_DRAIN;

  // Set port SCL i2c_0
  IIC0_SCL_ALTSEL0 = 1;
  IIC0_SCL_ALTSEL1 = 0;
  IIC0_SCL = 1;
  IIC0_SCL_DP = OUTPUT;
  IIC0_SCL_ODP = OPEN_DRAIN;

  // Set port SDA i2c_1
#ifndef HARDWARE_TEST_0
/*
  IIC1_SDA_ALTSEL0 = 1;
  IIC1_SDA_ALTSEL1 = 0;
  IIC1_SDA = 1;
  IIC1_SDA_DP = OUTPUT;
  IIC1_SDA_ODP = OPEN_DRAIN;

  // Set port SCL i2c_1
  IIC1_SCL_ALTSEL0 = 1;
  IIC1_SCL_ALTSEL1 = 0;
  IIC1_SCL = 1;
  IIC1_SCL_DP = OUTPUT;
  IIC1_SCL_ODP = OPEN_DRAIN;
*/
#endif // HARDWARE_TEST_0
  
  I2C_Select_Channel(IIC_CHANNEL_0_SELECTED, BAUDRATE_400KHZ);
  IIC_CON = IIC_CON_IGE_MASK | IIC_CON_MODE_SINGLE_MASTER/* | IIC_CON_INT_MASK*/;
  IIC_ST = 0;
  IIC_DIC        =  IIC_DATA_EVENT_INT_LEVEL;   
  IIC_DIC_IE = 1;
  IIC_PEIC       =  IIC_PROTOCOL_EVENT_INT_LEVEL;     
  IIC_PEIC_IE = 1;
  iic_status = IIC_READY;
}

unsigned char IIC_Write(unsigned char address, // addres = address from iic component
                        unsigned char sub_address_0,     // sub_addres_0 = sub address from iic component
                        unsigned char sub_address_1,     // sub_addres_1 = sub address from iic component
                        unsigned char two_sub_addresses, // two_sub_address = 0 only 1 sub address, 1 2 sub addresses
                        unsigned char *data,   // data = pointer to array with data to send
                        unsigned char nr)     // nr = number of bytes to send
{
long time_out = 0;

  if (iic_status == IIC_ERROR)
    I2C_Init();
  iic_address = address;
  iic_sub_address_0 = sub_address_0;
  iic_sub_address_1 = sub_address_1;
  iic_two_sub_addresses = two_sub_addresses;
  iic_data = data;
  iic_number_of_bytes = nr;
  iic_status = 1;
  IIC_RTBL = iic_address;
  _nop(); _nop(); _nop(); _nop(); _nop(); 
  IIC_CON |= IIC_CON_BUM_MASK | IIC_CON_RSC_MASK;
  while (1)
  {
    if (time_out < 100000)
    {
      time_out++;
      if (iic_status == IIC_READY)
        return (0);
      else if (iic_status == IIC_ERROR)
        return (1);
    }
    else
    {
      iic_status = IIC_ERROR;    
      return (1);
    }  
  }
}

unsigned char IIC_Read(unsigned char address,           // addres = address from iic component
                       unsigned char sub_address_0,     // sub_addres_0 = sub address from iic component
                       unsigned char sub_address_1,     // sub_addres_1 = sub address from iic component
                       unsigned char two_sub_addresses, // two_sub_address = 0 only 1 sub address, 1 2 sub addresses
                       unsigned char *data,        // data = pointer to array with data to send
                       unsigned char nr)                // nr = number of bytes to send
{
long time_out = 0;

  if (iic_status == IIC_ERROR)
    I2C_Init();
  iic_address = address;
  iic_sub_address_0 = sub_address_0;
  iic_sub_address_1 = sub_address_1;
  iic_two_sub_addresses = two_sub_addresses;
  iic_data = data;
  iic_number_of_bytes = nr;
  iic_status = 10;
  IIC_RTBL = iic_address;
  _nop(); _nop(); _nop(); _nop(); _nop(); 
  IIC_CON |= IIC_CON_BUM_MASK | IIC_CON_RSC_MASK;
  while (1)
  {
    if (time_out < 100000)
    {
      time_out++;
      if (iic_status == IIC_READY)
        return (0);
      else if (iic_status == IIC_ERROR)
        return (1);
    }
    else
    {
      iic_status = IIC_ERROR;    
      return (1);
    }  
  }
}

interrupt IIC_DATA_EVENT_INT_ADR using(IIC_DATA_EVENT_INT_RB) void IIC_Data_Event_Interrupt(void)
{
unsigned char dummy;

//  iic_data_cnt++;
/*
  if ((IIC_ST & IIC_ST_IRQE_MASK) == IIC_ST_IRQE_MASK)
  {
//    iic_irqe_cnt++;
    IIC_ST &= ~IIC_ST_IRQE_MASK;
  }
*/
  if ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK)
  {
//    iic_irqd_cnt++;
    switch (iic_status)
    {
      case 1:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          IIC_RTBL = iic_sub_address_0; // adres
          iic_status++;
          if (iic_two_sub_addresses == 0)
            iic_status++;
        }  
        break;
      case 2:  
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          IIC_RTBL = iic_sub_address_1; // adres
          iic_status++;
        }  
        break;
      case 3:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else if (iic_number_of_bytes > 1)
        {
          IIC_RTBL = *iic_data;
          iic_data++;
          iic_number_of_bytes--;
        }  
        else if (iic_number_of_bytes == 1)
        {
          IIC_CON |= IIC_CON_STP_MASK;
          IIC_RTBL = *iic_data;
          iic_number_of_bytes--;
        }
        else
        {
          IIC_ST &= ~IIC_ST_IRQD_MASK;
          iic_status = 0;
        }
        break;

      case 10:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          IIC_RTBL = iic_sub_address_0; // adres
          iic_status++;
          if (iic_two_sub_addresses == 0)
            iic_status++;
        }  
        break;
      case 11:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          IIC_RTBL = iic_sub_address_1; // adres
          iic_status++;
        }  
        break;
      case 12:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          IIC_CON |= IIC_CON_RSC_MASK;
          IIC_RTBL = iic_address | 0x01; // adres
          iic_status++;
        }  
        break;
      case 13:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          if (iic_number_of_bytes == 1)
          {
            IIC_CON |= IIC_CON_ACKDIS_MASK;
            IIC_CON |= IIC_CON_STP_MASK;
            iic_status++;
          }
          IIC_CON &= ~IIC_CON_TRX_MASK;
          dummy = IIC_RTBL;
          iic_number_of_bytes--;
          iic_status++;
        }
        break;
      case 14:
        if (IIC_ST & IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
// JP          IIC_CON = (IIC_CON & ~IIC_CON_CI_MASK) | IIC_CON_CI_TRANSMIT_BUFFER_LENGTH_1;
          if (iic_number_of_bytes == 1)
          {
            IIC_CON |= IIC_CON_ACKDIS_MASK;
            IIC_CON |= IIC_CON_STP_MASK;
            iic_status++;
          }
          IIC_CON &= ~IIC_CON_TRX_MASK;
          *iic_data = IIC_RTBL;
          iic_data++;
          iic_number_of_bytes--;
        }
        break;
      case 15:
        if ((IIC_ST & IIC_ST_LRB_MASK) != IIC_ST_LRB_MASK)
        {
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          IIC_CON &= ~IIC_CON_BUM_MASK;
          iic_status = IIC_ERROR;
        }
        else
        {
          *iic_data = IIC_RTBL;
          IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
          while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
          iic_status = 0;
        }
        break;
      default: // (error niet mogelijk)
        IIC_ST &= ~(IIC_ST_IRQD_MASK | IIC_ST_IRQE_MASK);
        while ((IIC_ST & IIC_ST_IRQD_MASK) == IIC_ST_IRQD_MASK);
        IIC_CON &= ~IIC_CON_BUM_MASK;
        iic_status = IIC_ERROR;
        break;
    }
  }
}

interrupt IIC_PROTOCOL_EVENT_INT_ADR using(IIC_PROTOCOL_EVENT_INT_RB) void IIC_Protocol_Event_Interrupt(void)
{
  if (IIC_ST & IIC_ST_AL_MASK)               // arbitration lost
  {
    IIC_ST &= ~IIC_ST_AL_MASK;                  // reset IIC_ST_AL_MASK
//    iic_irqp_cnt++;
  }
  IIC_ST &= ~IIC_ST_IRQP_MASK;                  // reset IIC_ST_IRQP_MASK
//  iic_protocol_cnt++;
}
