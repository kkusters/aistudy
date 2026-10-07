// CH_DISP_PASSWORD.H

#ifndef _CH_DISP_PASSWORD_H
#define _CH_DISP_PASSWORD_H

#include "ct_disp.h"

#define PASSWORD_LEVEL_GEEN          0
#define PASSWORD_LEVEL_SETP          1
#define PASSWORD_LEVEL_SETP_SYST     2
#define PASSWORD_LEVEL_SETP_SYST_OPT 3

#define PASSWORD_ENABLED_GEEN      0x00
#ifdef PASSWORD
#define PASSWORD_ENABLED_SETP_MASK 0x01
#define PASSWORD_ENABLED_SYST_MASK 0x02
#define PASSWORD_ENABLED_OPT_MASK  0x04
#else // PASSWORD
#define PASSWORD_ENABLED_SETP_MASK 0x0F
#define PASSWORD_ENABLED_SYST_MASK 0x0F
#define PASSWORD_ENABLED_OPT_MASK  0x0F
#endif // PASSWORD

#define PASSWORD_GEBRUIKER_MASK 0xF0
#define PASSWORD_BEHEERDER   0x10
#define PASSWORD_GEBRUIKER_1 0x20
#define PASSWORD_GEBRUIKER_2 0x30
#define PASSWORD_GEBRUIKER_3 0x40
#define PASSWORD_GEBRUIKER_4 0x50
#define PASSWORD_HOTRACO     0x60
#define PASSWORD_HOTRACO_ADM 0x70

#define PASSWORD_DELAY 10

#ifdef PASSWORD
#else // PASSWORD
extern unsigned char password_installateur_enabled;
extern unsigned char password_gebruiker_enabled;
#endif // PASSWORD
extern unsigned char password_enabled;
extern unsigned char password_enabled_delay;

extern s_screen screen_password;

void Control_Screen_Password(void);

void Right_Password(void);
void Right_Option_0_Password(void);
void Right_Setpoint_Syst_Password(void);
void Password_Control(void);
void Password_Init(void);

#endif // _CH_DISP_PASSWORD_H