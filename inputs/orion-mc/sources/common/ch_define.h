// CH_DEFINE.H

#ifndef _CH_DEFINE_H
#define _CH_DEFINE_H

// if EMULATOR defined then there is no watchdog triggering
//#define EMULATOR

#define CANopen

// voor testen verzenden fast sdo
// pas na SDO_MAX blokken 1 antwoord
// voor SDO_FAST aanpassen FIFO_BUFFER_SIZE naa 100 zodat screen data van 1 regel minimaal 3 x op stack past
#define SDO_1_BLOCK 0
#define SDO_8_BLOCKS 1
#define SDO_MAX_BLOCKS 2
//#define SDO_MAX 8

//#define TEST_1MS 1
#define ALARM_TEKST_NAAR_SMARTLINK 1
#define CAN_BACKBONE_PC_WARNING 1

// #define PASSWORD 1

#define CLKFREQ 40000000L

#define PULSES_PER_SECOND 8 //number of beats/second

#define MAX_FILES 10

#define CAN_IO_BAUDRATE 100000L
#define CAN_BACKBONE_BAUDRATE 125000L

#define ETHERNET 1
#define SD_CARD 1

#define INPUT 0
#define OUTPUT 1

#define DIGITAL 0
#define ANALOG 1

#define PUSH_PULL 0
#define OPEN_DRAIN 1

#define FALSE 0
#define TRUE 1

#define STX '@'
#define ETX '\r'
#define ETX_CH '*'
#define ETB_CH '+'

#define RECEIVE_WAIT  0
#define RECEIVE_BUSY  1
#define RECEIVE_READY 2

#define TRANSMIT_READY 0
#define TRANSMIT_BUSY  1

#include "ch_data.h"
#include "ch_int.h"
#include "cal_conf.h"

#endif // _CH_DEFINE_H
