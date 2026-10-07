// CH_XML.H

#ifndef _CH_XML_H
#define _CH_XML_H

#define XML_VENT_STATUS_NOT_AVAILABLE 0
#define XML_VENT_STATUS_ACTIVE        1
#define XML_VENT_STATUS_OFF           2
#define XML_VENT_STATUS_ON            3

#define XML_KLEP_STATUS_NOT_AVAILABLE 0
#define XML_KLEP_STATUS_ACTIVE        1
#define XML_KLEP_STATUS_OFF           2
#define XML_KLEP_STATUS_ON            3

typedef struct
{
  unsigned char Status;
  unsigned char Position;
  unsigned int  Rpm;
  unsigned int  EnergyConsumption;
  unsigned char ErrorUrgent;
  unsigned char ErrorNotUrgent;
} TXMLVent;

typedef struct
{
  unsigned char Status;
  unsigned char Position;
  unsigned char Limitswitch;
  unsigned char Motorstatus;
  unsigned char ErrorLimitswitch;
  unsigned char Error;
} TXMLKlep;

typedef struct
{
  TXMLVent Vent;
  TXMLKlep Klep;
} TXML;

void XML_GetData_Vent_Group(unsigned char i, TXML *XML);
int  XML_GetData_Vent(unsigned char i, TXML *XML);
void XML_GetData_Klep_Group(unsigned char i, TXML *XML);
int  XML_GetData_Klep(unsigned char i, TXML *XML);

#endif
