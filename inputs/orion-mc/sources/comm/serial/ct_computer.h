// CT_COMPUTER.H

#ifndef _CT_COMPUTER_H
#define _CT_COMPUTER_H

// computer
#define SIRIUS_OLD 1
#define ORION_OLD 2
#define BLACKBOX 10 
#define SIRIUS 11
#define ORION 12

// soort
#define BASIS 0
#define PLUIMVEE 1
#define VARKENS 2
#define OPSLAG 3
#define MESTDROGING 4
#define CO2 5
#define DATALOGGER 6
#define VOER 7
#define OPTIMALISATIE 12
#define MULTI_CONNECT 14
#define BASIS_AFD 100
#define VARKENS_AFD 102
#define OPSLAG_AFD 103
#define VOER_AFD 107

// type = onderverdeling soort 
#define BASIS_BASIS 0
#define PLUIMVEE_CL 0
#define PLUIMVEE_PB 1
#define PLUIMVEE_PS 2
#define PLUIMVEE_PL 3
#define PLUIMVEE_HH 4
#define VARKENS_BASIS 0
#define OPSLAG_BASIS 0
#define MESTDROGING_BASIS 0
#define CO2_BASIS 0
#define DATALOGGER_BASIS 0
#define DATALOGGER_ACU 1
#define VOER_BASIS 0
#define VOER_STALLEN 1
#define OPTIMALISATIE_BASIS 0

#define MODULE_BASIS          0x0000
#define MODULE_CANOPEN        0x0001
#define MODULE_VENTILATIE     0x0002
#define MODULE_LUCHTMENGKAST  0x0004
#define MODULE_BACNET         0x0008
#define MODULE_SCHAKELGROEPEN 0x0010
#define MODULE_KLEP           0x0020
#define MODULE_DRUKVERSCHIL   0x0040
#define MODULE_WATCHDOGMODE   0x0080
#define MODULE_HOOGENDOORN    0x0100

// firma
#define HOTRACO     0
#define MOOIJ       1
#define RR          2
#define FIENHAGE    3
#define ALKE        4
#define LASYSYSTEMS 5
#define STARTAGRI   6
#define ZUCAMI      7
#define PLETTENBURG 8
#define BRINK       9
#define ERMAF       10
#define BIGDUTCHMAN 11
#define PRULLAGE    12
#define VDL         13
#define ASC         14
#define DANVAN      15
#define BERG        16
#define AVG         17
#define FAROMOR     18
#define WILDEBOER   19
#define COGASZUID   20

#define PC_NO 0
#define PC_ENGELS 1
#define PC_NEDERLANDS 2
#define PC_DUITS 3
#define PC_SPAANS 4
#define PC_USER 255

#endif