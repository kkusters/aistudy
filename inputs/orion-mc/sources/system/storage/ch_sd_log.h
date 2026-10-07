// CH_SD_LOG.H

#ifndef _CH_SD_LOG_H
#define _CH_SD_LOG_H

#include "ch_timer.h"

extern unsigned char sd_log_control_flag; // als sd_log_control_flag 1 dan worden log gegevens gecontroleerd
#ifdef SD_CARD

void SD_Log_Data_Aanmaken(unsigned char aantal_variabelen, unsigned int *index_array, unsigned int interval, time_t start_tijd, time_t stop_tijd);
void SD_Log_Control(void);
void SD_Log_Einde_Dag(void);

#endif // SD_CARD

#endif // _CH_SD_LOG_H