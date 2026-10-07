// SD_LOG.C

#include "ch_define.h"
#include "ch_const.h"
#include "ch_sd.h"
#include "ch_sd_log.h"

unsigned char sd_log_control_flag = 1; // als sd_log_control_flag 1 dan worden log gegevens gecontroleerd
#ifdef SD_CARD

#define SIZE_CHAR 1
#define SIZE_INT  2
#define SIZE_LONG 4

#define SD_LOG_MAX_REGELS ((unsigned int)2000)

#define SD_LOG_EINDE_DAG_MIN_INTERVAL 60
#define SD_LOG_EINDE_DAG_MAX_INTERVAL 3600

typedef struct
{
  unsigned int code;
  void *value_ptr;
  unsigned char value_size;
} s_log_value;

typedef struct
{
  unsigned char length;
  unsigned char aantal_variabelen;
  s_log_value   value[MAX_LOG_VALUES];
} s_log_values;	// wordt telkens na opstarten opgebouwd hierdoor is in value her minder geheugen nodig

typedef struct
{
  unsigned int code;
  void *value_ptr;
  unsigned char value_size;
  void *option_ptr;
  unsigned char option_type;
} s_log_index_value;

s_log_index_value const sd_log_value_table[] =
{
  // Motorgroepen
  { 0x0000, &val_hr_alg.Motorgroup[ 0].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 0].Enabled, CHAR },
  { 0x0100, &val_hr_alg.Motorgroup[ 1].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 1].Enabled, CHAR },
  { 0x0200, &val_hr_alg.Motorgroup[ 2].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 2].Enabled, CHAR },
  { 0x0300, &val_hr_alg.Motorgroup[ 3].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 3].Enabled, CHAR },
  { 0x0400, &val_hr_alg.Motorgroup[ 4].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 4].Enabled, CHAR },
  { 0x0500, &val_hr_alg.Motorgroup[ 5].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 5].Enabled, CHAR },
  { 0x0600, &val_hr_alg.Motorgroup[ 6].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 6].Enabled, CHAR },
  { 0x0700, &val_hr_alg.Motorgroup[ 7].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 7].Enabled, CHAR },
  { 0x0800, &val_hr_alg.Motorgroup[ 8].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 8].Enabled, CHAR },
  { 0x0900, &val_hr_alg.Motorgroup[ 9].PositionPerc, SIZE_INT, &opt_app.Motorgroup[ 9].Enabled, CHAR },
  { 0x0A00, &val_hr_alg.Motorgroup[10].PositionPerc, SIZE_INT, &opt_app.Motorgroup[10].Enabled, CHAR },
  { 0x0B00, &val_hr_alg.Motorgroup[11].PositionPerc, SIZE_INT, &opt_app.Motorgroup[11].Enabled, CHAR },
  { 0x0C00, &val_hr_alg.Motorgroup[12].PositionPerc, SIZE_INT, &opt_app.Motorgroup[12].Enabled, CHAR },
  { 0x0D00, &val_hr_alg.Motorgroup[13].PositionPerc, SIZE_INT, &opt_app.Motorgroup[13].Enabled, CHAR },
  { 0x0E00, &val_hr_alg.Motorgroup[14].PositionPerc, SIZE_INT, &opt_app.Motorgroup[14].Enabled, CHAR },
  { 0x0F00, &val_hr_alg.Motorgroup[15].PositionPerc, SIZE_INT, &opt_app.Motorgroup[15].Enabled, CHAR },
  { 0x1000, &val_hr_alg.Motorgroup[16].PositionPerc, SIZE_INT, &opt_app.Motorgroup[16].Enabled, CHAR },
  { 0x1100, &val_hr_alg.Motorgroup[17].PositionPerc, SIZE_INT, &opt_app.Motorgroup[17].Enabled, CHAR },
  { 0x1200, &val_hr_alg.Motorgroup[18].PositionPerc, SIZE_INT, &opt_app.Motorgroup[18].Enabled, CHAR },
  { 0x1300, &val_hr_alg.Motorgroup[19].PositionPerc, SIZE_INT, &opt_app.Motorgroup[19].Enabled, CHAR },
  { 0x1400, &val_hr_alg.Motorgroup[20].PositionPerc, SIZE_INT, &opt_app.Motorgroup[20].Enabled, CHAR },
  { 0x1500, &val_hr_alg.Motorgroup[21].PositionPerc, SIZE_INT, &opt_app.Motorgroup[21].Enabled, CHAR },
  { 0x1600, &val_hr_alg.Motorgroup[22].PositionPerc, SIZE_INT, &opt_app.Motorgroup[22].Enabled, CHAR },
  { 0x1700, &val_hr_alg.Motorgroup[23].PositionPerc, SIZE_INT, &opt_app.Motorgroup[23].Enabled, CHAR },
  { 0x1800, &val_hr_alg.Motorgroup[24].PositionPerc, SIZE_INT, &opt_app.Motorgroup[24].Enabled, CHAR },
  { 0x1900, &val_hr_alg.Motorgroup[25].PositionPerc, SIZE_INT, &opt_app.Motorgroup[25].Enabled, CHAR },
  { 0x1A00, &val_hr_alg.Motorgroup[26].PositionPerc, SIZE_INT, &opt_app.Motorgroup[26].Enabled, CHAR },
  { 0x1B00, &val_hr_alg.Motorgroup[27].PositionPerc, SIZE_INT, &opt_app.Motorgroup[27].Enabled, CHAR },
  { 0x1C00, &val_hr_alg.Motorgroup[28].PositionPerc, SIZE_INT, &opt_app.Motorgroup[28].Enabled, CHAR },
  { 0x1D00, &val_hr_alg.Motorgroup[29].PositionPerc, SIZE_INT, &opt_app.Motorgroup[29].Enabled, CHAR },
  { 0x1E00, &val_hr_alg.Motorgroup[30].PositionPerc, SIZE_INT, &opt_app.Motorgroup[30].Enabled, CHAR },
  { 0x1F00, &val_hr_alg.Motorgroup[31].PositionPerc, SIZE_INT, &opt_app.Motorgroup[31].Enabled, CHAR },

  // Motoren
  { 0x0008, &val_hr_alg.Motor[ 0].Position, SIZE_INT, &opt_app.Motor[ 0].Enabled, CHAR },
  { 0x0108, &val_hr_alg.Motor[ 1].Position, SIZE_INT, &opt_app.Motor[ 1].Enabled, CHAR },
  { 0x0208, &val_hr_alg.Motor[ 2].Position, SIZE_INT, &opt_app.Motor[ 2].Enabled, CHAR },
  { 0x0308, &val_hr_alg.Motor[ 3].Position, SIZE_INT, &opt_app.Motor[ 3].Enabled, CHAR },
  { 0x0408, &val_hr_alg.Motor[ 4].Position, SIZE_INT, &opt_app.Motor[ 4].Enabled, CHAR },
  { 0x0508, &val_hr_alg.Motor[ 5].Position, SIZE_INT, &opt_app.Motor[ 5].Enabled, CHAR },
  { 0x0608, &val_hr_alg.Motor[ 6].Position, SIZE_INT, &opt_app.Motor[ 6].Enabled, CHAR },
  { 0x0708, &val_hr_alg.Motor[ 7].Position, SIZE_INT, &opt_app.Motor[ 7].Enabled, CHAR },
  { 0x0808, &val_hr_alg.Motor[ 8].Position, SIZE_INT, &opt_app.Motor[ 8].Enabled, CHAR },
  { 0x0908, &val_hr_alg.Motor[ 9].Position, SIZE_INT, &opt_app.Motor[ 9].Enabled, CHAR },
  { 0x0A08, &val_hr_alg.Motor[10].Position, SIZE_INT, &opt_app.Motor[10].Enabled, CHAR },
  { 0x0B08, &val_hr_alg.Motor[11].Position, SIZE_INT, &opt_app.Motor[11].Enabled, CHAR },
  { 0x0C08, &val_hr_alg.Motor[12].Position, SIZE_INT, &opt_app.Motor[12].Enabled, CHAR },
  { 0x0D08, &val_hr_alg.Motor[13].Position, SIZE_INT, &opt_app.Motor[13].Enabled, CHAR },
  { 0x0E08, &val_hr_alg.Motor[14].Position, SIZE_INT, &opt_app.Motor[14].Enabled, CHAR },
  { 0x0F08, &val_hr_alg.Motor[15].Position, SIZE_INT, &opt_app.Motor[15].Enabled, CHAR },
  { 0x1008, &val_hr_alg.Motor[16].Position, SIZE_INT, &opt_app.Motor[16].Enabled, CHAR },
  { 0x1108, &val_hr_alg.Motor[17].Position, SIZE_INT, &opt_app.Motor[17].Enabled, CHAR },
  { 0x1208, &val_hr_alg.Motor[18].Position, SIZE_INT, &opt_app.Motor[18].Enabled, CHAR },
  { 0x1308, &val_hr_alg.Motor[19].Position, SIZE_INT, &opt_app.Motor[19].Enabled, CHAR },
  { 0x1408, &val_hr_alg.Motor[20].Position, SIZE_INT, &opt_app.Motor[20].Enabled, CHAR },
  { 0x1508, &val_hr_alg.Motor[21].Position, SIZE_INT, &opt_app.Motor[21].Enabled, CHAR },
  { 0x1608, &val_hr_alg.Motor[22].Position, SIZE_INT, &opt_app.Motor[22].Enabled, CHAR },
  { 0x1708, &val_hr_alg.Motor[23].Position, SIZE_INT, &opt_app.Motor[23].Enabled, CHAR },
  { 0x1808, &val_hr_alg.Motor[24].Position, SIZE_INT, &opt_app.Motor[24].Enabled, CHAR },
  { 0x1908, &val_hr_alg.Motor[25].Position, SIZE_INT, &opt_app.Motor[25].Enabled, CHAR },
  { 0x1A08, &val_hr_alg.Motor[26].Position, SIZE_INT, &opt_app.Motor[26].Enabled, CHAR },
  { 0x1B08, &val_hr_alg.Motor[27].Position, SIZE_INT, &opt_app.Motor[27].Enabled, CHAR },
  { 0x1C08, &val_hr_alg.Motor[28].Position, SIZE_INT, &opt_app.Motor[28].Enabled, CHAR },
  { 0x1D08, &val_hr_alg.Motor[29].Position, SIZE_INT, &opt_app.Motor[29].Enabled, CHAR },
  { 0x1E08, &val_hr_alg.Motor[30].Position, SIZE_INT, &opt_app.Motor[30].Enabled, CHAR },
  { 0x1F08, &val_hr_alg.Motor[31].Position, SIZE_INT, &opt_app.Motor[31].Enabled, CHAR },
  { 0x2008, &val_hr_alg.Motor[32].Position, SIZE_INT, &opt_app.Motor[32].Enabled, CHAR },
  { 0x2108, &val_hr_alg.Motor[33].Position, SIZE_INT, &opt_app.Motor[33].Enabled, CHAR },
  { 0x2208, &val_hr_alg.Motor[34].Position, SIZE_INT, &opt_app.Motor[34].Enabled, CHAR },
  { 0x2308, &val_hr_alg.Motor[35].Position, SIZE_INT, &opt_app.Motor[35].Enabled, CHAR },
  { 0x2408, &val_hr_alg.Motor[36].Position, SIZE_INT, &opt_app.Motor[36].Enabled, CHAR },
  { 0x2508, &val_hr_alg.Motor[37].Position, SIZE_INT, &opt_app.Motor[37].Enabled, CHAR },
  { 0x2608, &val_hr_alg.Motor[38].Position, SIZE_INT, &opt_app.Motor[38].Enabled, CHAR },
  { 0x2708, &val_hr_alg.Motor[39].Position, SIZE_INT, &opt_app.Motor[39].Enabled, CHAR },
  { 0x2808, &val_hr_alg.Motor[40].Position, SIZE_INT, &opt_app.Motor[40].Enabled, CHAR },
  { 0x2908, &val_hr_alg.Motor[41].Position, SIZE_INT, &opt_app.Motor[41].Enabled, CHAR },
  { 0x2A08, &val_hr_alg.Motor[42].Position, SIZE_INT, &opt_app.Motor[42].Enabled, CHAR },
  { 0x2B08, &val_hr_alg.Motor[43].Position, SIZE_INT, &opt_app.Motor[43].Enabled, CHAR },
  { 0x2C08, &val_hr_alg.Motor[44].Position, SIZE_INT, &opt_app.Motor[44].Enabled, CHAR },
  { 0x2D08, &val_hr_alg.Motor[45].Position, SIZE_INT, &opt_app.Motor[45].Enabled, CHAR },
  { 0x2E08, &val_hr_alg.Motor[46].Position, SIZE_INT, &opt_app.Motor[46].Enabled, CHAR },
  { 0x2F08, &val_hr_alg.Motor[47].Position, SIZE_INT, &opt_app.Motor[47].Enabled, CHAR },
  { 0x3008, &val_hr_alg.Motor[48].Position, SIZE_INT, &opt_app.Motor[48].Enabled, CHAR },
  { 0x3108, &val_hr_alg.Motor[49].Position, SIZE_INT, &opt_app.Motor[49].Enabled, CHAR },
  { 0x3208, &val_hr_alg.Motor[50].Position, SIZE_INT, &opt_app.Motor[50].Enabled, CHAR },
  { 0x3308, &val_hr_alg.Motor[51].Position, SIZE_INT, &opt_app.Motor[51].Enabled, CHAR },
  { 0x3408, &val_hr_alg.Motor[52].Position, SIZE_INT, &opt_app.Motor[52].Enabled, CHAR },
  { 0x3508, &val_hr_alg.Motor[53].Position, SIZE_INT, &opt_app.Motor[53].Enabled, CHAR },
  { 0x3608, &val_hr_alg.Motor[54].Position, SIZE_INT, &opt_app.Motor[54].Enabled, CHAR },
  { 0x3708, &val_hr_alg.Motor[55].Position, SIZE_INT, &opt_app.Motor[55].Enabled, CHAR },
  { 0x3808, &val_hr_alg.Motor[56].Position, SIZE_INT, &opt_app.Motor[56].Enabled, CHAR },
  { 0x3908, &val_hr_alg.Motor[57].Position, SIZE_INT, &opt_app.Motor[57].Enabled, CHAR },
  { 0x3A08, &val_hr_alg.Motor[58].Position, SIZE_INT, &opt_app.Motor[58].Enabled, CHAR },
  { 0x3B08, &val_hr_alg.Motor[59].Position, SIZE_INT, &opt_app.Motor[59].Enabled, CHAR },
  { 0x3C08, &val_hr_alg.Motor[60].Position, SIZE_INT, &opt_app.Motor[60].Enabled, CHAR },
  { 0x3D08, &val_hr_alg.Motor[61].Position, SIZE_INT, &opt_app.Motor[61].Enabled, CHAR },
  { 0x3E08, &val_hr_alg.Motor[62].Position, SIZE_INT, &opt_app.Motor[62].Enabled, CHAR },
  { 0x3F08, &val_hr_alg.Motor[63].Position, SIZE_INT, &opt_app.Motor[63].Enabled, CHAR },

  // Ventilatoren
  { 0x0010, &val_hr_alg.Device[  0].ActualValue, SIZE_CHAR, &opt_app.Device[  0].Enabled, CHAR },
  { 0x0110, &val_hr_alg.Device[  1].ActualValue, SIZE_CHAR, &opt_app.Device[  1].Enabled, CHAR },
  { 0x0210, &val_hr_alg.Device[  2].ActualValue, SIZE_CHAR, &opt_app.Device[  2].Enabled, CHAR },
  { 0x0310, &val_hr_alg.Device[  3].ActualValue, SIZE_CHAR, &opt_app.Device[  3].Enabled, CHAR },
  { 0x0410, &val_hr_alg.Device[  4].ActualValue, SIZE_CHAR, &opt_app.Device[  4].Enabled, CHAR },
  { 0x0510, &val_hr_alg.Device[  5].ActualValue, SIZE_CHAR, &opt_app.Device[  5].Enabled, CHAR },
  { 0x0610, &val_hr_alg.Device[  6].ActualValue, SIZE_CHAR, &opt_app.Device[  6].Enabled, CHAR },
  { 0x0710, &val_hr_alg.Device[  7].ActualValue, SIZE_CHAR, &opt_app.Device[  7].Enabled, CHAR },
  { 0x0810, &val_hr_alg.Device[  8].ActualValue, SIZE_CHAR, &opt_app.Device[  8].Enabled, CHAR },
  { 0x0910, &val_hr_alg.Device[  9].ActualValue, SIZE_CHAR, &opt_app.Device[  9].Enabled, CHAR },
  { 0x0A10, &val_hr_alg.Device[ 10].ActualValue, SIZE_CHAR, &opt_app.Device[ 10].Enabled, CHAR },
  { 0x0B10, &val_hr_alg.Device[ 11].ActualValue, SIZE_CHAR, &opt_app.Device[ 11].Enabled, CHAR },
  { 0x0C10, &val_hr_alg.Device[ 12].ActualValue, SIZE_CHAR, &opt_app.Device[ 12].Enabled, CHAR },
  { 0x0D10, &val_hr_alg.Device[ 13].ActualValue, SIZE_CHAR, &opt_app.Device[ 13].Enabled, CHAR },
  { 0x0E10, &val_hr_alg.Device[ 14].ActualValue, SIZE_CHAR, &opt_app.Device[ 14].Enabled, CHAR },
  { 0x0F10, &val_hr_alg.Device[ 15].ActualValue, SIZE_CHAR, &opt_app.Device[ 15].Enabled, CHAR },
  { 0x1010, &val_hr_alg.Device[ 16].ActualValue, SIZE_CHAR, &opt_app.Device[ 16].Enabled, CHAR },
  { 0x1110, &val_hr_alg.Device[ 17].ActualValue, SIZE_CHAR, &opt_app.Device[ 17].Enabled, CHAR },
  { 0x1210, &val_hr_alg.Device[ 18].ActualValue, SIZE_CHAR, &opt_app.Device[ 18].Enabled, CHAR },
  { 0x1310, &val_hr_alg.Device[ 19].ActualValue, SIZE_CHAR, &opt_app.Device[ 19].Enabled, CHAR },
  { 0x1410, &val_hr_alg.Device[ 20].ActualValue, SIZE_CHAR, &opt_app.Device[ 20].Enabled, CHAR },
  { 0x1510, &val_hr_alg.Device[ 21].ActualValue, SIZE_CHAR, &opt_app.Device[ 21].Enabled, CHAR },
  { 0x1610, &val_hr_alg.Device[ 22].ActualValue, SIZE_CHAR, &opt_app.Device[ 22].Enabled, CHAR },
  { 0x1710, &val_hr_alg.Device[ 23].ActualValue, SIZE_CHAR, &opt_app.Device[ 23].Enabled, CHAR },
  { 0x1810, &val_hr_alg.Device[ 24].ActualValue, SIZE_CHAR, &opt_app.Device[ 24].Enabled, CHAR },
  { 0x1910, &val_hr_alg.Device[ 25].ActualValue, SIZE_CHAR, &opt_app.Device[ 25].Enabled, CHAR },
  { 0x1A10, &val_hr_alg.Device[ 26].ActualValue, SIZE_CHAR, &opt_app.Device[ 26].Enabled, CHAR },
  { 0x1B10, &val_hr_alg.Device[ 27].ActualValue, SIZE_CHAR, &opt_app.Device[ 27].Enabled, CHAR },
  { 0x1C10, &val_hr_alg.Device[ 28].ActualValue, SIZE_CHAR, &opt_app.Device[ 28].Enabled, CHAR },
  { 0x1D10, &val_hr_alg.Device[ 29].ActualValue, SIZE_CHAR, &opt_app.Device[ 29].Enabled, CHAR },
  { 0x1E10, &val_hr_alg.Device[ 30].ActualValue, SIZE_CHAR, &opt_app.Device[ 30].Enabled, CHAR },
  { 0x1F10, &val_hr_alg.Device[ 31].ActualValue, SIZE_CHAR, &opt_app.Device[ 31].Enabled, CHAR },
  { 0x2010, &val_hr_alg.Device[ 32].ActualValue, SIZE_CHAR, &opt_app.Device[ 32].Enabled, CHAR },
  { 0x2110, &val_hr_alg.Device[ 33].ActualValue, SIZE_CHAR, &opt_app.Device[ 33].Enabled, CHAR },
  { 0x2210, &val_hr_alg.Device[ 34].ActualValue, SIZE_CHAR, &opt_app.Device[ 34].Enabled, CHAR },
  { 0x2310, &val_hr_alg.Device[ 35].ActualValue, SIZE_CHAR, &opt_app.Device[ 35].Enabled, CHAR },
  { 0x2410, &val_hr_alg.Device[ 36].ActualValue, SIZE_CHAR, &opt_app.Device[ 36].Enabled, CHAR },
  { 0x2510, &val_hr_alg.Device[ 37].ActualValue, SIZE_CHAR, &opt_app.Device[ 37].Enabled, CHAR },
  { 0x2610, &val_hr_alg.Device[ 38].ActualValue, SIZE_CHAR, &opt_app.Device[ 38].Enabled, CHAR },
  { 0x2710, &val_hr_alg.Device[ 39].ActualValue, SIZE_CHAR, &opt_app.Device[ 39].Enabled, CHAR },
  { 0x2810, &val_hr_alg.Device[ 40].ActualValue, SIZE_CHAR, &opt_app.Device[ 40].Enabled, CHAR },
  { 0x2910, &val_hr_alg.Device[ 41].ActualValue, SIZE_CHAR, &opt_app.Device[ 41].Enabled, CHAR },
  { 0x2A10, &val_hr_alg.Device[ 42].ActualValue, SIZE_CHAR, &opt_app.Device[ 42].Enabled, CHAR },
  { 0x2B10, &val_hr_alg.Device[ 43].ActualValue, SIZE_CHAR, &opt_app.Device[ 43].Enabled, CHAR },
  { 0x2C10, &val_hr_alg.Device[ 44].ActualValue, SIZE_CHAR, &opt_app.Device[ 44].Enabled, CHAR },
  { 0x2D10, &val_hr_alg.Device[ 45].ActualValue, SIZE_CHAR, &opt_app.Device[ 45].Enabled, CHAR },
  { 0x2E10, &val_hr_alg.Device[ 46].ActualValue, SIZE_CHAR, &opt_app.Device[ 46].Enabled, CHAR },
  { 0x2F10, &val_hr_alg.Device[ 47].ActualValue, SIZE_CHAR, &opt_app.Device[ 47].Enabled, CHAR },
  { 0x3010, &val_hr_alg.Device[ 48].ActualValue, SIZE_CHAR, &opt_app.Device[ 48].Enabled, CHAR },
  { 0x3110, &val_hr_alg.Device[ 49].ActualValue, SIZE_CHAR, &opt_app.Device[ 49].Enabled, CHAR },
  { 0x3210, &val_hr_alg.Device[ 50].ActualValue, SIZE_CHAR, &opt_app.Device[ 50].Enabled, CHAR },
  { 0x3310, &val_hr_alg.Device[ 51].ActualValue, SIZE_CHAR, &opt_app.Device[ 51].Enabled, CHAR },
  { 0x3410, &val_hr_alg.Device[ 52].ActualValue, SIZE_CHAR, &opt_app.Device[ 52].Enabled, CHAR },
  { 0x3510, &val_hr_alg.Device[ 53].ActualValue, SIZE_CHAR, &opt_app.Device[ 53].Enabled, CHAR },
  { 0x3610, &val_hr_alg.Device[ 54].ActualValue, SIZE_CHAR, &opt_app.Device[ 54].Enabled, CHAR },
  { 0x3710, &val_hr_alg.Device[ 55].ActualValue, SIZE_CHAR, &opt_app.Device[ 55].Enabled, CHAR },
  { 0x3810, &val_hr_alg.Device[ 56].ActualValue, SIZE_CHAR, &opt_app.Device[ 56].Enabled, CHAR },
  { 0x3910, &val_hr_alg.Device[ 57].ActualValue, SIZE_CHAR, &opt_app.Device[ 57].Enabled, CHAR },
  { 0x3A10, &val_hr_alg.Device[ 58].ActualValue, SIZE_CHAR, &opt_app.Device[ 58].Enabled, CHAR },
  { 0x3B10, &val_hr_alg.Device[ 59].ActualValue, SIZE_CHAR, &opt_app.Device[ 59].Enabled, CHAR },
  { 0x3C10, &val_hr_alg.Device[ 60].ActualValue, SIZE_CHAR, &opt_app.Device[ 60].Enabled, CHAR },
  { 0x3D10, &val_hr_alg.Device[ 61].ActualValue, SIZE_CHAR, &opt_app.Device[ 61].Enabled, CHAR },
  { 0x3E10, &val_hr_alg.Device[ 62].ActualValue, SIZE_CHAR, &opt_app.Device[ 62].Enabled, CHAR },
  { 0x3F10, &val_hr_alg.Device[ 63].ActualValue, SIZE_CHAR, &opt_app.Device[ 63].Enabled, CHAR },
  { 0x4010, &val_hr_alg.Device[ 64].ActualValue, SIZE_CHAR, &opt_app.Device[ 64].Enabled, CHAR },
  { 0x4110, &val_hr_alg.Device[ 65].ActualValue, SIZE_CHAR, &opt_app.Device[ 65].Enabled, CHAR },
  { 0x4210, &val_hr_alg.Device[ 66].ActualValue, SIZE_CHAR, &opt_app.Device[ 66].Enabled, CHAR },
  { 0x4310, &val_hr_alg.Device[ 67].ActualValue, SIZE_CHAR, &opt_app.Device[ 67].Enabled, CHAR },
  { 0x4410, &val_hr_alg.Device[ 68].ActualValue, SIZE_CHAR, &opt_app.Device[ 68].Enabled, CHAR },
  { 0x4510, &val_hr_alg.Device[ 69].ActualValue, SIZE_CHAR, &opt_app.Device[ 69].Enabled, CHAR },
  { 0x4610, &val_hr_alg.Device[ 70].ActualValue, SIZE_CHAR, &opt_app.Device[ 70].Enabled, CHAR },
  { 0x4710, &val_hr_alg.Device[ 71].ActualValue, SIZE_CHAR, &opt_app.Device[ 71].Enabled, CHAR },
  { 0x4810, &val_hr_alg.Device[ 72].ActualValue, SIZE_CHAR, &opt_app.Device[ 72].Enabled, CHAR },
  { 0x4910, &val_hr_alg.Device[ 73].ActualValue, SIZE_CHAR, &opt_app.Device[ 73].Enabled, CHAR },
  { 0x4A10, &val_hr_alg.Device[ 74].ActualValue, SIZE_CHAR, &opt_app.Device[ 74].Enabled, CHAR },
  { 0x4B10, &val_hr_alg.Device[ 75].ActualValue, SIZE_CHAR, &opt_app.Device[ 75].Enabled, CHAR },
  { 0x4C10, &val_hr_alg.Device[ 76].ActualValue, SIZE_CHAR, &opt_app.Device[ 76].Enabled, CHAR },
  { 0x4D10, &val_hr_alg.Device[ 77].ActualValue, SIZE_CHAR, &opt_app.Device[ 77].Enabled, CHAR },
  { 0x4E10, &val_hr_alg.Device[ 78].ActualValue, SIZE_CHAR, &opt_app.Device[ 78].Enabled, CHAR },
  { 0x4F10, &val_hr_alg.Device[ 79].ActualValue, SIZE_CHAR, &opt_app.Device[ 79].Enabled, CHAR },
  { 0x5010, &val_hr_alg.Device[ 80].ActualValue, SIZE_CHAR, &opt_app.Device[ 80].Enabled, CHAR },
  { 0x5110, &val_hr_alg.Device[ 81].ActualValue, SIZE_CHAR, &opt_app.Device[ 81].Enabled, CHAR },
  { 0x5210, &val_hr_alg.Device[ 82].ActualValue, SIZE_CHAR, &opt_app.Device[ 82].Enabled, CHAR },
  { 0x5310, &val_hr_alg.Device[ 83].ActualValue, SIZE_CHAR, &opt_app.Device[ 83].Enabled, CHAR },
  { 0x5410, &val_hr_alg.Device[ 84].ActualValue, SIZE_CHAR, &opt_app.Device[ 84].Enabled, CHAR },
  { 0x5510, &val_hr_alg.Device[ 85].ActualValue, SIZE_CHAR, &opt_app.Device[ 85].Enabled, CHAR },
  { 0x5610, &val_hr_alg.Device[ 86].ActualValue, SIZE_CHAR, &opt_app.Device[ 86].Enabled, CHAR },
  { 0x5710, &val_hr_alg.Device[ 87].ActualValue, SIZE_CHAR, &opt_app.Device[ 87].Enabled, CHAR },
  { 0x5810, &val_hr_alg.Device[ 88].ActualValue, SIZE_CHAR, &opt_app.Device[ 88].Enabled, CHAR },
  { 0x5910, &val_hr_alg.Device[ 89].ActualValue, SIZE_CHAR, &opt_app.Device[ 89].Enabled, CHAR },
  { 0x5A10, &val_hr_alg.Device[ 90].ActualValue, SIZE_CHAR, &opt_app.Device[ 90].Enabled, CHAR },
  { 0x5B10, &val_hr_alg.Device[ 91].ActualValue, SIZE_CHAR, &opt_app.Device[ 91].Enabled, CHAR },
  { 0x5C10, &val_hr_alg.Device[ 92].ActualValue, SIZE_CHAR, &opt_app.Device[ 92].Enabled, CHAR },
  { 0x5D10, &val_hr_alg.Device[ 93].ActualValue, SIZE_CHAR, &opt_app.Device[ 93].Enabled, CHAR },
  { 0x5E10, &val_hr_alg.Device[ 94].ActualValue, SIZE_CHAR, &opt_app.Device[ 94].Enabled, CHAR },
  { 0x5F10, &val_hr_alg.Device[ 95].ActualValue, SIZE_CHAR, &opt_app.Device[ 95].Enabled, CHAR },
  { 0x6010, &val_hr_alg.Device[ 96].ActualValue, SIZE_CHAR, &opt_app.Device[ 96].Enabled, CHAR },
  { 0x6110, &val_hr_alg.Device[ 97].ActualValue, SIZE_CHAR, &opt_app.Device[ 97].Enabled, CHAR },
  { 0x6210, &val_hr_alg.Device[ 98].ActualValue, SIZE_CHAR, &opt_app.Device[ 98].Enabled, CHAR },
  { 0x6310, &val_hr_alg.Device[ 99].ActualValue, SIZE_CHAR, &opt_app.Device[ 99].Enabled, CHAR },
  { 0x6410, &val_hr_alg.Device[100].ActualValue, SIZE_CHAR, &opt_app.Device[100].Enabled, CHAR },
  { 0x6510, &val_hr_alg.Device[101].ActualValue, SIZE_CHAR, &opt_app.Device[101].Enabled, CHAR },
  { 0x6610, &val_hr_alg.Device[102].ActualValue, SIZE_CHAR, &opt_app.Device[102].Enabled, CHAR },
  { 0x6710, &val_hr_alg.Device[103].ActualValue, SIZE_CHAR, &opt_app.Device[103].Enabled, CHAR },
  { 0x6810, &val_hr_alg.Device[104].ActualValue, SIZE_CHAR, &opt_app.Device[104].Enabled, CHAR },
  { 0x6910, &val_hr_alg.Device[105].ActualValue, SIZE_CHAR, &opt_app.Device[105].Enabled, CHAR },
  { 0x6A10, &val_hr_alg.Device[106].ActualValue, SIZE_CHAR, &opt_app.Device[106].Enabled, CHAR },
  { 0x6B10, &val_hr_alg.Device[107].ActualValue, SIZE_CHAR, &opt_app.Device[107].Enabled, CHAR },
  { 0x6C10, &val_hr_alg.Device[108].ActualValue, SIZE_CHAR, &opt_app.Device[108].Enabled, CHAR },
  { 0x6D10, &val_hr_alg.Device[109].ActualValue, SIZE_CHAR, &opt_app.Device[109].Enabled, CHAR },
  { 0x6E10, &val_hr_alg.Device[110].ActualValue, SIZE_CHAR, &opt_app.Device[110].Enabled, CHAR },
  { 0x6F10, &val_hr_alg.Device[111].ActualValue, SIZE_CHAR, &opt_app.Device[111].Enabled, CHAR },
  { 0x7010, &val_hr_alg.Device[112].ActualValue, SIZE_CHAR, &opt_app.Device[112].Enabled, CHAR },
  { 0x7110, &val_hr_alg.Device[113].ActualValue, SIZE_CHAR, &opt_app.Device[113].Enabled, CHAR },
  { 0x7210, &val_hr_alg.Device[114].ActualValue, SIZE_CHAR, &opt_app.Device[114].Enabled, CHAR },
  { 0x7310, &val_hr_alg.Device[115].ActualValue, SIZE_CHAR, &opt_app.Device[115].Enabled, CHAR },
  { 0x7410, &val_hr_alg.Device[116].ActualValue, SIZE_CHAR, &opt_app.Device[116].Enabled, CHAR },
  { 0x7510, &val_hr_alg.Device[117].ActualValue, SIZE_CHAR, &opt_app.Device[117].Enabled, CHAR },
  { 0x7610, &val_hr_alg.Device[118].ActualValue, SIZE_CHAR, &opt_app.Device[118].Enabled, CHAR },
  { 0x7710, &val_hr_alg.Device[119].ActualValue, SIZE_CHAR, &opt_app.Device[119].Enabled, CHAR },
  { 0x7810, &val_hr_alg.Device[120].ActualValue, SIZE_CHAR, &opt_app.Device[120].Enabled, CHAR },
  { 0x7910, &val_hr_alg.Device[121].ActualValue, SIZE_CHAR, &opt_app.Device[121].Enabled, CHAR },
  { 0x7A10, &val_hr_alg.Device[122].ActualValue, SIZE_CHAR, &opt_app.Device[122].Enabled, CHAR },
  { 0x7B10, &val_hr_alg.Device[123].ActualValue, SIZE_CHAR, &opt_app.Device[123].Enabled, CHAR },
  { 0x7C10, &val_hr_alg.Device[124].ActualValue, SIZE_CHAR, &opt_app.Device[124].Enabled, CHAR },
  { 0x7D10, &val_hr_alg.Device[125].ActualValue, SIZE_CHAR, &opt_app.Device[125].Enabled, CHAR },
  { 0x7E10, &val_hr_alg.Device[126].ActualValue, SIZE_CHAR, &opt_app.Device[126].Enabled, CHAR },
  { 0x7F10, &val_hr_alg.Device[127].ActualValue, SIZE_CHAR, &opt_app.Device[127].Enabled, CHAR },
  { 0x8010, &val_hr_alg.Device[128].ActualValue, SIZE_CHAR, &opt_app.Device[128].Enabled, CHAR },
  { 0x8110, &val_hr_alg.Device[129].ActualValue, SIZE_CHAR, &opt_app.Device[129].Enabled, CHAR },
  { 0x8210, &val_hr_alg.Device[130].ActualValue, SIZE_CHAR, &opt_app.Device[130].Enabled, CHAR },
  { 0x8310, &val_hr_alg.Device[131].ActualValue, SIZE_CHAR, &opt_app.Device[131].Enabled, CHAR },
  { 0x8410, &val_hr_alg.Device[132].ActualValue, SIZE_CHAR, &opt_app.Device[132].Enabled, CHAR },
  { 0x8510, &val_hr_alg.Device[133].ActualValue, SIZE_CHAR, &opt_app.Device[133].Enabled, CHAR },
  { 0x8610, &val_hr_alg.Device[134].ActualValue, SIZE_CHAR, &opt_app.Device[134].Enabled, CHAR },
  { 0x8710, &val_hr_alg.Device[135].ActualValue, SIZE_CHAR, &opt_app.Device[135].Enabled, CHAR },
  { 0x8810, &val_hr_alg.Device[136].ActualValue, SIZE_CHAR, &opt_app.Device[136].Enabled, CHAR },
  { 0x8910, &val_hr_alg.Device[137].ActualValue, SIZE_CHAR, &opt_app.Device[137].Enabled, CHAR },
  { 0x8A10, &val_hr_alg.Device[138].ActualValue, SIZE_CHAR, &opt_app.Device[138].Enabled, CHAR },
  { 0x8B10, &val_hr_alg.Device[139].ActualValue, SIZE_CHAR, &opt_app.Device[139].Enabled, CHAR },
  { 0x8C10, &val_hr_alg.Device[140].ActualValue, SIZE_CHAR, &opt_app.Device[140].Enabled, CHAR },
  { 0x8D10, &val_hr_alg.Device[141].ActualValue, SIZE_CHAR, &opt_app.Device[141].Enabled, CHAR },
  { 0x8E10, &val_hr_alg.Device[142].ActualValue, SIZE_CHAR, &opt_app.Device[142].Enabled, CHAR },
  { 0x8F10, &val_hr_alg.Device[143].ActualValue, SIZE_CHAR, &opt_app.Device[143].Enabled, CHAR },
  { 0x9010, &val_hr_alg.Device[144].ActualValue, SIZE_CHAR, &opt_app.Device[144].Enabled, CHAR },
  { 0x9110, &val_hr_alg.Device[145].ActualValue, SIZE_CHAR, &opt_app.Device[145].Enabled, CHAR },
  { 0x9210, &val_hr_alg.Device[146].ActualValue, SIZE_CHAR, &opt_app.Device[146].Enabled, CHAR },
  { 0x9310, &val_hr_alg.Device[147].ActualValue, SIZE_CHAR, &opt_app.Device[147].Enabled, CHAR },
  { 0x9410, &val_hr_alg.Device[148].ActualValue, SIZE_CHAR, &opt_app.Device[148].Enabled, CHAR },
  { 0x9510, &val_hr_alg.Device[149].ActualValue, SIZE_CHAR, &opt_app.Device[149].Enabled, CHAR },
  { 0x9610, &val_hr_alg.Device[150].ActualValue, SIZE_CHAR, &opt_app.Device[150].Enabled, CHAR },
  { 0x9710, &val_hr_alg.Device[151].ActualValue, SIZE_CHAR, &opt_app.Device[151].Enabled, CHAR },
  { 0x9810, &val_hr_alg.Device[152].ActualValue, SIZE_CHAR, &opt_app.Device[152].Enabled, CHAR },
  { 0x9910, &val_hr_alg.Device[153].ActualValue, SIZE_CHAR, &opt_app.Device[153].Enabled, CHAR },
  { 0x9A10, &val_hr_alg.Device[154].ActualValue, SIZE_CHAR, &opt_app.Device[154].Enabled, CHAR },
  { 0x9B10, &val_hr_alg.Device[155].ActualValue, SIZE_CHAR, &opt_app.Device[155].Enabled, CHAR },
  { 0x9C10, &val_hr_alg.Device[156].ActualValue, SIZE_CHAR, &opt_app.Device[156].Enabled, CHAR },
  { 0x9D10, &val_hr_alg.Device[157].ActualValue, SIZE_CHAR, &opt_app.Device[157].Enabled, CHAR },
  { 0x9E10, &val_hr_alg.Device[158].ActualValue, SIZE_CHAR, &opt_app.Device[158].Enabled, CHAR },
  { 0x9F10, &val_hr_alg.Device[159].ActualValue, SIZE_CHAR, &opt_app.Device[159].Enabled, CHAR },
  { 0xA010, &val_hr_alg.Device[160].ActualValue, SIZE_CHAR, &opt_app.Device[160].Enabled, CHAR },
  { 0xA110, &val_hr_alg.Device[161].ActualValue, SIZE_CHAR, &opt_app.Device[161].Enabled, CHAR },
  { 0xA210, &val_hr_alg.Device[162].ActualValue, SIZE_CHAR, &opt_app.Device[162].Enabled, CHAR },
  { 0xA310, &val_hr_alg.Device[163].ActualValue, SIZE_CHAR, &opt_app.Device[163].Enabled, CHAR },
  { 0xA410, &val_hr_alg.Device[164].ActualValue, SIZE_CHAR, &opt_app.Device[164].Enabled, CHAR },
  { 0xA510, &val_hr_alg.Device[165].ActualValue, SIZE_CHAR, &opt_app.Device[165].Enabled, CHAR },
  { 0xA610, &val_hr_alg.Device[166].ActualValue, SIZE_CHAR, &opt_app.Device[166].Enabled, CHAR },
  { 0xA710, &val_hr_alg.Device[167].ActualValue, SIZE_CHAR, &opt_app.Device[167].Enabled, CHAR },
  { 0xA810, &val_hr_alg.Device[168].ActualValue, SIZE_CHAR, &opt_app.Device[168].Enabled, CHAR },
  { 0xA910, &val_hr_alg.Device[169].ActualValue, SIZE_CHAR, &opt_app.Device[169].Enabled, CHAR },
  { 0xAA10, &val_hr_alg.Device[170].ActualValue, SIZE_CHAR, &opt_app.Device[170].Enabled, CHAR },
  { 0xAB10, &val_hr_alg.Device[171].ActualValue, SIZE_CHAR, &opt_app.Device[171].Enabled, CHAR },
  { 0xAC10, &val_hr_alg.Device[172].ActualValue, SIZE_CHAR, &opt_app.Device[172].Enabled, CHAR },
  { 0xAD10, &val_hr_alg.Device[173].ActualValue, SIZE_CHAR, &opt_app.Device[173].Enabled, CHAR },
  { 0xAE10, &val_hr_alg.Device[174].ActualValue, SIZE_CHAR, &opt_app.Device[174].Enabled, CHAR },
  { 0xAF10, &val_hr_alg.Device[175].ActualValue, SIZE_CHAR, &opt_app.Device[175].Enabled, CHAR },
  { 0xB010, &val_hr_alg.Device[176].ActualValue, SIZE_CHAR, &opt_app.Device[176].Enabled, CHAR },
  { 0xB110, &val_hr_alg.Device[177].ActualValue, SIZE_CHAR, &opt_app.Device[177].Enabled, CHAR },
  { 0xB210, &val_hr_alg.Device[178].ActualValue, SIZE_CHAR, &opt_app.Device[178].Enabled, CHAR },
  { 0xB310, &val_hr_alg.Device[179].ActualValue, SIZE_CHAR, &opt_app.Device[179].Enabled, CHAR },
  { 0xB410, &val_hr_alg.Device[180].ActualValue, SIZE_CHAR, &opt_app.Device[180].Enabled, CHAR },
  { 0xB510, &val_hr_alg.Device[181].ActualValue, SIZE_CHAR, &opt_app.Device[181].Enabled, CHAR },
  { 0xB610, &val_hr_alg.Device[182].ActualValue, SIZE_CHAR, &opt_app.Device[182].Enabled, CHAR },
  { 0xB710, &val_hr_alg.Device[183].ActualValue, SIZE_CHAR, &opt_app.Device[183].Enabled, CHAR },
  { 0xB810, &val_hr_alg.Device[184].ActualValue, SIZE_CHAR, &opt_app.Device[184].Enabled, CHAR },
  { 0xB910, &val_hr_alg.Device[185].ActualValue, SIZE_CHAR, &opt_app.Device[185].Enabled, CHAR },
  { 0xBA10, &val_hr_alg.Device[186].ActualValue, SIZE_CHAR, &opt_app.Device[186].Enabled, CHAR },
  { 0xBB10, &val_hr_alg.Device[187].ActualValue, SIZE_CHAR, &opt_app.Device[187].Enabled, CHAR },
  { 0xBC10, &val_hr_alg.Device[188].ActualValue, SIZE_CHAR, &opt_app.Device[188].Enabled, CHAR },
  { 0xBD10, &val_hr_alg.Device[189].ActualValue, SIZE_CHAR, &opt_app.Device[189].Enabled, CHAR },
  { 0xBE10, &val_hr_alg.Device[190].ActualValue, SIZE_CHAR, &opt_app.Device[190].Enabled, CHAR },
  { 0xBF10, &val_hr_alg.Device[191].ActualValue, SIZE_CHAR, &opt_app.Device[191].Enabled, CHAR },
  { 0xC010, &val_hr_alg.Device[192].ActualValue, SIZE_CHAR, &opt_app.Device[192].Enabled, CHAR },
  { 0xC110, &val_hr_alg.Device[193].ActualValue, SIZE_CHAR, &opt_app.Device[193].Enabled, CHAR },
  { 0xC210, &val_hr_alg.Device[194].ActualValue, SIZE_CHAR, &opt_app.Device[194].Enabled, CHAR },
  { 0xC310, &val_hr_alg.Device[195].ActualValue, SIZE_CHAR, &opt_app.Device[195].Enabled, CHAR },
  { 0xC410, &val_hr_alg.Device[196].ActualValue, SIZE_CHAR, &opt_app.Device[196].Enabled, CHAR },
  { 0xC510, &val_hr_alg.Device[197].ActualValue, SIZE_CHAR, &opt_app.Device[197].Enabled, CHAR },
  { 0xC610, &val_hr_alg.Device[198].ActualValue, SIZE_CHAR, &opt_app.Device[198].Enabled, CHAR },
  { 0xC710, &val_hr_alg.Device[199].ActualValue, SIZE_CHAR, &opt_app.Device[199].Enabled, CHAR },
  { 0xC810, &val_hr_alg.Device[200].ActualValue, SIZE_CHAR, &opt_app.Device[200].Enabled, CHAR },
  { 0xC910, &val_hr_alg.Device[201].ActualValue, SIZE_CHAR, &opt_app.Device[201].Enabled, CHAR },
  { 0xCA10, &val_hr_alg.Device[202].ActualValue, SIZE_CHAR, &opt_app.Device[202].Enabled, CHAR },
  { 0xCB10, &val_hr_alg.Device[203].ActualValue, SIZE_CHAR, &opt_app.Device[203].Enabled, CHAR },
  { 0xCC10, &val_hr_alg.Device[204].ActualValue, SIZE_CHAR, &opt_app.Device[204].Enabled, CHAR },
  { 0xCD10, &val_hr_alg.Device[205].ActualValue, SIZE_CHAR, &opt_app.Device[205].Enabled, CHAR },
  { 0xCE10, &val_hr_alg.Device[206].ActualValue, SIZE_CHAR, &opt_app.Device[206].Enabled, CHAR },
  { 0xCF10, &val_hr_alg.Device[207].ActualValue, SIZE_CHAR, &opt_app.Device[207].Enabled, CHAR },
  { 0xD010, &val_hr_alg.Device[208].ActualValue, SIZE_CHAR, &opt_app.Device[208].Enabled, CHAR },
  { 0xD110, &val_hr_alg.Device[209].ActualValue, SIZE_CHAR, &opt_app.Device[209].Enabled, CHAR },
  { 0xD210, &val_hr_alg.Device[210].ActualValue, SIZE_CHAR, &opt_app.Device[210].Enabled, CHAR },
  { 0xD310, &val_hr_alg.Device[211].ActualValue, SIZE_CHAR, &opt_app.Device[211].Enabled, CHAR },
  { 0xD410, &val_hr_alg.Device[212].ActualValue, SIZE_CHAR, &opt_app.Device[212].Enabled, CHAR },
  { 0xD510, &val_hr_alg.Device[213].ActualValue, SIZE_CHAR, &opt_app.Device[213].Enabled, CHAR },
  { 0xD610, &val_hr_alg.Device[214].ActualValue, SIZE_CHAR, &opt_app.Device[214].Enabled, CHAR },
  { 0xD710, &val_hr_alg.Device[215].ActualValue, SIZE_CHAR, &opt_app.Device[215].Enabled, CHAR },
  { 0xD810, &val_hr_alg.Device[216].ActualValue, SIZE_CHAR, &opt_app.Device[216].Enabled, CHAR },
  { 0xD910, &val_hr_alg.Device[217].ActualValue, SIZE_CHAR, &opt_app.Device[217].Enabled, CHAR },
  { 0xDA10, &val_hr_alg.Device[218].ActualValue, SIZE_CHAR, &opt_app.Device[218].Enabled, CHAR },
  { 0xDB10, &val_hr_alg.Device[219].ActualValue, SIZE_CHAR, &opt_app.Device[219].Enabled, CHAR },
  { 0xDC10, &val_hr_alg.Device[220].ActualValue, SIZE_CHAR, &opt_app.Device[220].Enabled, CHAR },
  { 0xDD10, &val_hr_alg.Device[221].ActualValue, SIZE_CHAR, &opt_app.Device[221].Enabled, CHAR },
  { 0xDE10, &val_hr_alg.Device[222].ActualValue, SIZE_CHAR, &opt_app.Device[222].Enabled, CHAR },
  { 0xDF10, &val_hr_alg.Device[223].ActualValue, SIZE_CHAR, &opt_app.Device[223].Enabled, CHAR },
  { 0xE010, &val_hr_alg.Device[224].ActualValue, SIZE_CHAR, &opt_app.Device[224].Enabled, CHAR },
  { 0xE110, &val_hr_alg.Device[225].ActualValue, SIZE_CHAR, &opt_app.Device[225].Enabled, CHAR },
  { 0xE210, &val_hr_alg.Device[226].ActualValue, SIZE_CHAR, &opt_app.Device[226].Enabled, CHAR },
  { 0xE310, &val_hr_alg.Device[227].ActualValue, SIZE_CHAR, &opt_app.Device[227].Enabled, CHAR },
  { 0xE410, &val_hr_alg.Device[228].ActualValue, SIZE_CHAR, &opt_app.Device[228].Enabled, CHAR },
  { 0xE510, &val_hr_alg.Device[229].ActualValue, SIZE_CHAR, &opt_app.Device[229].Enabled, CHAR },
  { 0xE610, &val_hr_alg.Device[230].ActualValue, SIZE_CHAR, &opt_app.Device[230].Enabled, CHAR },
  { 0xE710, &val_hr_alg.Device[231].ActualValue, SIZE_CHAR, &opt_app.Device[231].Enabled, CHAR },
  { 0xE810, &val_hr_alg.Device[232].ActualValue, SIZE_CHAR, &opt_app.Device[232].Enabled, CHAR },
  { 0xE910, &val_hr_alg.Device[233].ActualValue, SIZE_CHAR, &opt_app.Device[233].Enabled, CHAR },
  { 0xEA10, &val_hr_alg.Device[234].ActualValue, SIZE_CHAR, &opt_app.Device[234].Enabled, CHAR },
  { 0xEB10, &val_hr_alg.Device[235].ActualValue, SIZE_CHAR, &opt_app.Device[235].Enabled, CHAR },
  { 0xEC10, &val_hr_alg.Device[236].ActualValue, SIZE_CHAR, &opt_app.Device[236].Enabled, CHAR },
  { 0xED10, &val_hr_alg.Device[237].ActualValue, SIZE_CHAR, &opt_app.Device[237].Enabled, CHAR },
  { 0xEE10, &val_hr_alg.Device[238].ActualValue, SIZE_CHAR, &opt_app.Device[238].Enabled, CHAR },
  { 0xEF10, &val_hr_alg.Device[239].ActualValue, SIZE_CHAR, &opt_app.Device[239].Enabled, CHAR },
  { 0xF010, &val_hr_alg.Device[240].ActualValue, SIZE_CHAR, &opt_app.Device[240].Enabled, CHAR },
  { 0xF110, &val_hr_alg.Device[241].ActualValue, SIZE_CHAR, &opt_app.Device[241].Enabled, CHAR },
  { 0xF210, &val_hr_alg.Device[242].ActualValue, SIZE_CHAR, &opt_app.Device[242].Enabled, CHAR },
  { 0xF310, &val_hr_alg.Device[243].ActualValue, SIZE_CHAR, &opt_app.Device[243].Enabled, CHAR },
  { 0xF410, &val_hr_alg.Device[244].ActualValue, SIZE_CHAR, &opt_app.Device[244].Enabled, CHAR },
  { 0xF510, &val_hr_alg.Device[245].ActualValue, SIZE_CHAR, &opt_app.Device[245].Enabled, CHAR },
  { 0xF610, &val_hr_alg.Device[246].ActualValue, SIZE_CHAR, &opt_app.Device[246].Enabled, CHAR },
  { 0xF710, &val_hr_alg.Device[247].ActualValue, SIZE_CHAR, &opt_app.Device[247].Enabled, CHAR },
  { 0xF810, &val_hr_alg.Device[248].ActualValue, SIZE_CHAR, &opt_app.Device[248].Enabled, CHAR },
  { 0xF910, &val_hr_alg.Device[249].ActualValue, SIZE_CHAR, &opt_app.Device[249].Enabled, CHAR },
  { 0xFA10, &val_hr_alg.Device[250].ActualValue, SIZE_CHAR, &opt_app.Device[250].Enabled, CHAR },
  { 0xFB10, &val_hr_alg.Device[251].ActualValue, SIZE_CHAR, &opt_app.Device[251].Enabled, CHAR },
  { 0xFC10, &val_hr_alg.Device[252].ActualValue, SIZE_CHAR, &opt_app.Device[252].Enabled, CHAR },
  { 0xFD10, &val_hr_alg.Device[253].ActualValue, SIZE_CHAR, &opt_app.Device[253].Enabled, CHAR },
  { 0xFE10, &val_hr_alg.Device[254].ActualValue, SIZE_CHAR, &opt_app.Device[254].Enabled, CHAR },
  { 0xFF10, &val_hr_alg.Device[255].ActualValue, SIZE_CHAR, &opt_app.Device[255].Enabled, CHAR },
    
  // Luchtmengkast groepen
  { 0x0020, &val_hr_alg.LuchtmengkastGroep[0].Buitenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[0].Enabled,                CHAR },
  { 0x0021, &val_hr_alg.LuchtmengkastGroep[0].Binnenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[0].Enabled,                CHAR },
  { 0x0022, &val_hr_alg.LuchtmengkastGroep[0].Inblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[0].Enabled,                CHAR },
  { 0x0023, &val_hr_alg.LuchtmengkastGroep[0].Afblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[0].AfblaasventEnabled,     CHAR },
  { 0x0024, &val_hr_alg.LuchtmengkastGroep[0].Verwarming.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[0].VerwarmingEnabled,      CHAR },
//  { 0x0025, &val_hr_alg.LuchtmengkastGroep[0].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[0].AfblaasventOpOnderdruk, CHAR },
//  { 0x0026, &val_hr_alg.LuchtmengkastGroep[0].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[0].AfblaasventOpOnderdruk, CHAR },
//  { 0x0027, &val_hr_alg.LuchtmengkastGroep[0].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[0].BovenklepOpOnderdruk,   CHAR },
//  { 0x0028, &val_hr_alg.LuchtmengkastGroep[0].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[0].BovenklepOpOnderdruk,   CHAR },
  { 0x0120, &val_hr_alg.LuchtmengkastGroep[1].Buitenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[1].Enabled,                CHAR },
  { 0x0121, &val_hr_alg.LuchtmengkastGroep[1].Binnenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[1].Enabled,                CHAR },
  { 0x0122, &val_hr_alg.LuchtmengkastGroep[1].Inblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[1].Enabled,                CHAR },
  { 0x0123, &val_hr_alg.LuchtmengkastGroep[1].Afblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[1].AfblaasventEnabled,     CHAR },
  { 0x0124, &val_hr_alg.LuchtmengkastGroep[1].Verwarming.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[1].VerwarmingEnabled,      CHAR },
//  { 0x0125, &val_hr_alg.LuchtmengkastGroep[1].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[1].AfblaasventOpOnderdruk, CHAR },
//  { 0x0126, &val_hr_alg.LuchtmengkastGroep[1].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[1].AfblaasventOpOnderdruk, CHAR },
//  { 0x0127, &val_hr_alg.LuchtmengkastGroep[1].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[1].BovenklepOpOnderdruk,   CHAR },
//  { 0x0128, &val_hr_alg.LuchtmengkastGroep[1].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[1].BovenklepOpOnderdruk,   CHAR },
  { 0x0220, &val_hr_alg.LuchtmengkastGroep[2].Buitenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[2].Enabled,                CHAR },
  { 0x0221, &val_hr_alg.LuchtmengkastGroep[2].Binnenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[2].Enabled,                CHAR },
  { 0x0222, &val_hr_alg.LuchtmengkastGroep[2].Inblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[2].Enabled,                CHAR },
  { 0x0223, &val_hr_alg.LuchtmengkastGroep[2].Afblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[2].AfblaasventEnabled,     CHAR },
  { 0x0224, &val_hr_alg.LuchtmengkastGroep[2].Verwarming.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[2].VerwarmingEnabled,      CHAR },
//  { 0x0225, &val_hr_alg.LuchtmengkastGroep[2].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[2].AfblaasventOpOnderdruk, CHAR },
//  { 0x0226, &val_hr_alg.LuchtmengkastGroep[2].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[2].AfblaasventOpOnderdruk, CHAR },
//  { 0x0227, &val_hr_alg.LuchtmengkastGroep[2].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[2].BovenklepOpOnderdruk,   CHAR },
//  { 0x0228, &val_hr_alg.LuchtmengkastGroep[2].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[2].BovenklepOpOnderdruk,   CHAR },
  { 0x0320, &val_hr_alg.LuchtmengkastGroep[3].Buitenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[3].Enabled,                CHAR },
  { 0x0321, &val_hr_alg.LuchtmengkastGroep[3].Binnenklep.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[3].Enabled,                CHAR },
  { 0x0322, &val_hr_alg.LuchtmengkastGroep[3].Inblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[3].Enabled,                CHAR },
  { 0x0323, &val_hr_alg.LuchtmengkastGroep[3].Afblaasvent.PositionPerc, SIZE_CHAR, &opt_app.LuchtmengkastGroep[3].AfblaasventEnabled,     CHAR },
  { 0x0324, &val_hr_alg.LuchtmengkastGroep[3].Verwarming.PositionPerc,  SIZE_CHAR, &opt_app.LuchtmengkastGroep[3].VerwarmingEnabled,      CHAR },
//  { 0x0325, &val_hr_alg.LuchtmengkastGroep[3].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[3].AfblaasventOpOnderdruk, CHAR },
//  { 0x0326, &val_hr_alg.LuchtmengkastGroep[3].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[3].AfblaasventOpOnderdruk, CHAR },
//  { 0x0327, &val_hr_alg.LuchtmengkastGroep[3].DrukAct,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[3].BovenklepOpOnderdruk,   CHAR },
//  { 0x0328, &val_hr_alg.LuchtmengkastGroep[3].DrukAvg,                  SIZE_INT,  &opt_app.LuchtmengkastGroep[3].BovenklepOpOnderdruk,   CHAR },

  // Luchtmengkast units
  { 0x0030, &val_hr_alg.Luchtmengkast[ 0].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 0].AnaInInblaastemp.board_type, CHAR },
  { 0x0031, &val_hr_alg.Luchtmengkast[ 0].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 0].AnaInMengtemp.board_type,    CHAR },
  { 0x0032, &val_hr_alg.Luchtmengkast[ 0].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 0].Enabled,                     CHAR },
  { 0x0033, &val_hr_alg.Luchtmengkast[ 0].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 0].Enabled,                     CHAR },
  { 0x0034, &val_hr_alg.Luchtmengkast[ 0].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 0].Enabled,                     CHAR },
  { 0x0035, &val_hr_alg.Luchtmengkast[ 0].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 0].AfblaasventEnabled,          CHAR },
  { 0x0036, &val_hr_alg.Luchtmengkast[ 0].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 0].VerwarmingEnabled,           CHAR },
  { 0x0130, &val_hr_alg.Luchtmengkast[ 1].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 1].AnaInInblaastemp.board_type, CHAR },
  { 0x0131, &val_hr_alg.Luchtmengkast[ 1].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 1].AnaInMengtemp.board_type,    CHAR },
  { 0x0132, &val_hr_alg.Luchtmengkast[ 1].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 1].Enabled,                     CHAR },
  { 0x0133, &val_hr_alg.Luchtmengkast[ 1].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 1].Enabled,                     CHAR },
  { 0x0134, &val_hr_alg.Luchtmengkast[ 1].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 1].Enabled,                     CHAR },
  { 0x0135, &val_hr_alg.Luchtmengkast[ 1].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 1].AfblaasventEnabled,          CHAR },
  { 0x0136, &val_hr_alg.Luchtmengkast[ 1].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 1].VerwarmingEnabled,           CHAR },
  { 0x0230, &val_hr_alg.Luchtmengkast[ 2].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 2].AnaInInblaastemp.board_type, CHAR },
  { 0x0231, &val_hr_alg.Luchtmengkast[ 2].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 2].AnaInMengtemp.board_type,    CHAR },
  { 0x0232, &val_hr_alg.Luchtmengkast[ 2].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 2].Enabled,                     CHAR },
  { 0x0233, &val_hr_alg.Luchtmengkast[ 2].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 2].Enabled,                     CHAR },
  { 0x0234, &val_hr_alg.Luchtmengkast[ 2].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 2].Enabled,                     CHAR },
  { 0x0235, &val_hr_alg.Luchtmengkast[ 2].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 2].AfblaasventEnabled,          CHAR },
  { 0x0236, &val_hr_alg.Luchtmengkast[ 2].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 2].VerwarmingEnabled,           CHAR },
  { 0x0330, &val_hr_alg.Luchtmengkast[ 3].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 3].AnaInInblaastemp.board_type, CHAR },
  { 0x0331, &val_hr_alg.Luchtmengkast[ 3].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 3].AnaInMengtemp.board_type,    CHAR },
  { 0x0332, &val_hr_alg.Luchtmengkast[ 3].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 3].Enabled,                     CHAR },
  { 0x0333, &val_hr_alg.Luchtmengkast[ 3].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 3].Enabled,                     CHAR },
  { 0x0334, &val_hr_alg.Luchtmengkast[ 3].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 3].Enabled,                     CHAR },
  { 0x0335, &val_hr_alg.Luchtmengkast[ 3].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 3].AfblaasventEnabled,          CHAR },
  { 0x0336, &val_hr_alg.Luchtmengkast[ 3].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 3].VerwarmingEnabled,           CHAR },
  { 0x0430, &val_hr_alg.Luchtmengkast[ 4].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 4].AnaInInblaastemp.board_type, CHAR },
  { 0x0431, &val_hr_alg.Luchtmengkast[ 4].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 4].AnaInMengtemp.board_type,    CHAR },
  { 0x0432, &val_hr_alg.Luchtmengkast[ 4].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 4].Enabled,                     CHAR },
  { 0x0433, &val_hr_alg.Luchtmengkast[ 4].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 4].Enabled,                     CHAR },
  { 0x0434, &val_hr_alg.Luchtmengkast[ 4].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 4].Enabled,                     CHAR },
  { 0x0435, &val_hr_alg.Luchtmengkast[ 4].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 4].AfblaasventEnabled,          CHAR },
  { 0x0436, &val_hr_alg.Luchtmengkast[ 4].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 4].VerwarmingEnabled,           CHAR },
  { 0x0530, &val_hr_alg.Luchtmengkast[ 5].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 5].AnaInInblaastemp.board_type, CHAR },
  { 0x0531, &val_hr_alg.Luchtmengkast[ 5].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 5].AnaInMengtemp.board_type,    CHAR },
  { 0x0532, &val_hr_alg.Luchtmengkast[ 5].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 5].Enabled,                     CHAR },
  { 0x0533, &val_hr_alg.Luchtmengkast[ 5].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 5].Enabled,                     CHAR },
  { 0x0534, &val_hr_alg.Luchtmengkast[ 5].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 5].Enabled,                     CHAR },
  { 0x0535, &val_hr_alg.Luchtmengkast[ 5].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 5].AfblaasventEnabled,          CHAR },
  { 0x0536, &val_hr_alg.Luchtmengkast[ 5].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 5].VerwarmingEnabled,           CHAR },
  { 0x0630, &val_hr_alg.Luchtmengkast[ 6].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 6].AnaInInblaastemp.board_type, CHAR },
  { 0x0631, &val_hr_alg.Luchtmengkast[ 6].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 6].AnaInMengtemp.board_type,    CHAR },
  { 0x0632, &val_hr_alg.Luchtmengkast[ 6].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 6].Enabled,                     CHAR },
  { 0x0633, &val_hr_alg.Luchtmengkast[ 6].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 6].Enabled,                     CHAR },
  { 0x0634, &val_hr_alg.Luchtmengkast[ 6].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 6].Enabled,                     CHAR },
  { 0x0635, &val_hr_alg.Luchtmengkast[ 6].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 6].AfblaasventEnabled,          CHAR },
  { 0x0636, &val_hr_alg.Luchtmengkast[ 6].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 6].VerwarmingEnabled,           CHAR },
  { 0x0730, &val_hr_alg.Luchtmengkast[ 7].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 7].AnaInInblaastemp.board_type, CHAR },
  { 0x0731, &val_hr_alg.Luchtmengkast[ 7].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 7].AnaInMengtemp.board_type,    CHAR },
  { 0x0732, &val_hr_alg.Luchtmengkast[ 7].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 7].Enabled,                     CHAR },
  { 0x0733, &val_hr_alg.Luchtmengkast[ 7].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 7].Enabled,                     CHAR },
  { 0x0734, &val_hr_alg.Luchtmengkast[ 7].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 7].Enabled,                     CHAR },
  { 0x0735, &val_hr_alg.Luchtmengkast[ 7].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 7].AfblaasventEnabled,          CHAR },
  { 0x0736, &val_hr_alg.Luchtmengkast[ 7].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 7].VerwarmingEnabled,           CHAR },
  { 0x0830, &val_hr_alg.Luchtmengkast[ 8].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 8].AnaInInblaastemp.board_type, CHAR },
  { 0x0831, &val_hr_alg.Luchtmengkast[ 8].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 8].AnaInMengtemp.board_type,    CHAR },
  { 0x0832, &val_hr_alg.Luchtmengkast[ 8].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 8].Enabled,                     CHAR },
  { 0x0833, &val_hr_alg.Luchtmengkast[ 8].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 8].Enabled,                     CHAR },
  { 0x0834, &val_hr_alg.Luchtmengkast[ 8].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 8].Enabled,                     CHAR },
  { 0x0835, &val_hr_alg.Luchtmengkast[ 8].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 8].AfblaasventEnabled,          CHAR },
  { 0x0836, &val_hr_alg.Luchtmengkast[ 8].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 8].VerwarmingEnabled,           CHAR },
  { 0x0930, &val_hr_alg.Luchtmengkast[ 9].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[ 9].AnaInInblaastemp.board_type, CHAR },
  { 0x0931, &val_hr_alg.Luchtmengkast[ 9].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[ 9].AnaInMengtemp.board_type,    CHAR },
  { 0x0932, &val_hr_alg.Luchtmengkast[ 9].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 9].Enabled,                     CHAR },
  { 0x0933, &val_hr_alg.Luchtmengkast[ 9].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 9].Enabled,                     CHAR },
  { 0x0934, &val_hr_alg.Luchtmengkast[ 9].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 9].Enabled,                     CHAR },
  { 0x0935, &val_hr_alg.Luchtmengkast[ 9].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[ 9].AfblaasventEnabled,          CHAR },
  { 0x0936, &val_hr_alg.Luchtmengkast[ 9].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[ 9].VerwarmingEnabled,           CHAR },
  { 0x0A30, &val_hr_alg.Luchtmengkast[10].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[10].AnaInInblaastemp.board_type, CHAR },
  { 0x0A31, &val_hr_alg.Luchtmengkast[10].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[10].AnaInMengtemp.board_type,    CHAR },
  { 0x0A32, &val_hr_alg.Luchtmengkast[10].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[10].Enabled,                     CHAR },
  { 0x0A33, &val_hr_alg.Luchtmengkast[10].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[10].Enabled,                     CHAR },
  { 0x0A34, &val_hr_alg.Luchtmengkast[10].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[10].Enabled,                     CHAR },
  { 0x0A35, &val_hr_alg.Luchtmengkast[10].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[10].AfblaasventEnabled,          CHAR },
  { 0x0A36, &val_hr_alg.Luchtmengkast[10].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[10].VerwarmingEnabled,           CHAR },
  { 0x0B30, &val_hr_alg.Luchtmengkast[11].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[11].AnaInInblaastemp.board_type, CHAR },
  { 0x0B31, &val_hr_alg.Luchtmengkast[11].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[11].AnaInMengtemp.board_type,    CHAR },
  { 0x0B32, &val_hr_alg.Luchtmengkast[11].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[11].Enabled,                     CHAR },
  { 0x0B33, &val_hr_alg.Luchtmengkast[11].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[11].Enabled,                     CHAR },
  { 0x0B34, &val_hr_alg.Luchtmengkast[11].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[11].Enabled,                     CHAR },
  { 0x0B35, &val_hr_alg.Luchtmengkast[11].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[11].AfblaasventEnabled,          CHAR },
  { 0x0B36, &val_hr_alg.Luchtmengkast[11].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[11].VerwarmingEnabled,           CHAR },
  { 0x0C30, &val_hr_alg.Luchtmengkast[12].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[12].AnaInInblaastemp.board_type, CHAR },
  { 0x0C31, &val_hr_alg.Luchtmengkast[12].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[12].AnaInMengtemp.board_type,    CHAR },
  { 0x0C32, &val_hr_alg.Luchtmengkast[12].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[12].Enabled,                     CHAR },
  { 0x0C33, &val_hr_alg.Luchtmengkast[12].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[12].Enabled,                     CHAR },
  { 0x0C34, &val_hr_alg.Luchtmengkast[12].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[12].Enabled,                     CHAR },
  { 0x0C35, &val_hr_alg.Luchtmengkast[12].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[12].AfblaasventEnabled,          CHAR },
  { 0x0C36, &val_hr_alg.Luchtmengkast[12].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[12].VerwarmingEnabled,           CHAR },
  { 0x0D30, &val_hr_alg.Luchtmengkast[13].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[13].AnaInInblaastemp.board_type, CHAR },
  { 0x0D31, &val_hr_alg.Luchtmengkast[13].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[13].AnaInMengtemp.board_type,    CHAR },
  { 0x0D32, &val_hr_alg.Luchtmengkast[13].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[13].Enabled,                     CHAR },
  { 0x0D33, &val_hr_alg.Luchtmengkast[13].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[13].Enabled,                     CHAR },
  { 0x0D34, &val_hr_alg.Luchtmengkast[13].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[13].Enabled,                     CHAR },
  { 0x0D35, &val_hr_alg.Luchtmengkast[13].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[13].AfblaasventEnabled,          CHAR },
  { 0x0D36, &val_hr_alg.Luchtmengkast[13].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[13].VerwarmingEnabled,           CHAR },
  { 0x0E30, &val_hr_alg.Luchtmengkast[14].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[14].AnaInInblaastemp.board_type, CHAR },
  { 0x0E31, &val_hr_alg.Luchtmengkast[14].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[14].AnaInMengtemp.board_type,    CHAR },
  { 0x0E32, &val_hr_alg.Luchtmengkast[14].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[14].Enabled,                     CHAR },
  { 0x0E33, &val_hr_alg.Luchtmengkast[14].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[14].Enabled,                     CHAR },
  { 0x0E34, &val_hr_alg.Luchtmengkast[14].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[14].Enabled,                     CHAR },
  { 0x0E35, &val_hr_alg.Luchtmengkast[14].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[14].AfblaasventEnabled,          CHAR },
  { 0x0E36, &val_hr_alg.Luchtmengkast[14].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[14].VerwarmingEnabled,           CHAR },
  { 0x0F30, &val_hr_alg.Luchtmengkast[15].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[15].AnaInInblaastemp.board_type, CHAR },
  { 0x0F31, &val_hr_alg.Luchtmengkast[15].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[15].AnaInMengtemp.board_type,    CHAR },
  { 0x0F32, &val_hr_alg.Luchtmengkast[15].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[15].Enabled,                     CHAR },
  { 0x0F33, &val_hr_alg.Luchtmengkast[15].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[15].Enabled,                     CHAR },
  { 0x0F34, &val_hr_alg.Luchtmengkast[15].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[15].Enabled,                     CHAR },
  { 0x0F35, &val_hr_alg.Luchtmengkast[15].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[15].AfblaasventEnabled,          CHAR },
  { 0x0F36, &val_hr_alg.Luchtmengkast[15].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[15].VerwarmingEnabled,           CHAR },
  { 0x1030, &val_hr_alg.Luchtmengkast[16].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[16].AnaInInblaastemp.board_type, CHAR },
  { 0x1031, &val_hr_alg.Luchtmengkast[16].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[16].AnaInMengtemp.board_type,    CHAR },
  { 0x1032, &val_hr_alg.Luchtmengkast[16].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[16].Enabled,                     CHAR },
  { 0x1033, &val_hr_alg.Luchtmengkast[16].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[16].Enabled,                     CHAR },
  { 0x1034, &val_hr_alg.Luchtmengkast[16].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[16].Enabled,                     CHAR },
  { 0x1035, &val_hr_alg.Luchtmengkast[16].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[16].AfblaasventEnabled,          CHAR },
  { 0x1036, &val_hr_alg.Luchtmengkast[16].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[16].VerwarmingEnabled,           CHAR },
  { 0x1130, &val_hr_alg.Luchtmengkast[17].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[17].AnaInInblaastemp.board_type, CHAR },
  { 0x1131, &val_hr_alg.Luchtmengkast[17].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[17].AnaInMengtemp.board_type,    CHAR },
  { 0x1132, &val_hr_alg.Luchtmengkast[17].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[17].Enabled,                     CHAR },
  { 0x1133, &val_hr_alg.Luchtmengkast[17].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[17].Enabled,                     CHAR },
  { 0x1134, &val_hr_alg.Luchtmengkast[17].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[17].Enabled,                     CHAR },
  { 0x1135, &val_hr_alg.Luchtmengkast[17].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[17].AfblaasventEnabled,          CHAR },
  { 0x1136, &val_hr_alg.Luchtmengkast[17].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[17].VerwarmingEnabled,           CHAR },
  { 0x1230, &val_hr_alg.Luchtmengkast[18].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[18].AnaInInblaastemp.board_type, CHAR },
  { 0x1231, &val_hr_alg.Luchtmengkast[18].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[18].AnaInMengtemp.board_type,    CHAR },
  { 0x1232, &val_hr_alg.Luchtmengkast[18].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[18].Enabled,                     CHAR },
  { 0x1233, &val_hr_alg.Luchtmengkast[18].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[18].Enabled,                     CHAR },
  { 0x1234, &val_hr_alg.Luchtmengkast[18].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[18].Enabled,                     CHAR },
  { 0x1235, &val_hr_alg.Luchtmengkast[18].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[18].AfblaasventEnabled,          CHAR },
  { 0x1236, &val_hr_alg.Luchtmengkast[18].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[18].VerwarmingEnabled,           CHAR },
  { 0x1330, &val_hr_alg.Luchtmengkast[19].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[19].AnaInInblaastemp.board_type, CHAR },
  { 0x1331, &val_hr_alg.Luchtmengkast[19].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[19].AnaInMengtemp.board_type,    CHAR },
  { 0x1332, &val_hr_alg.Luchtmengkast[19].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[19].Enabled,                     CHAR },
  { 0x1333, &val_hr_alg.Luchtmengkast[19].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[19].Enabled,                     CHAR },
  { 0x1334, &val_hr_alg.Luchtmengkast[19].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[19].Enabled,                     CHAR },
  { 0x1335, &val_hr_alg.Luchtmengkast[19].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[19].AfblaasventEnabled,          CHAR },
  { 0x1336, &val_hr_alg.Luchtmengkast[19].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[19].VerwarmingEnabled,           CHAR },
  { 0x1430, &val_hr_alg.Luchtmengkast[20].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[20].AnaInInblaastemp.board_type, CHAR },
  { 0x1431, &val_hr_alg.Luchtmengkast[20].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[20].AnaInMengtemp.board_type,    CHAR },
  { 0x1432, &val_hr_alg.Luchtmengkast[20].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[20].Enabled,                     CHAR },
  { 0x1433, &val_hr_alg.Luchtmengkast[20].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[20].Enabled,                     CHAR },
  { 0x1434, &val_hr_alg.Luchtmengkast[20].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[20].Enabled,                     CHAR },
  { 0x1435, &val_hr_alg.Luchtmengkast[20].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[20].AfblaasventEnabled,          CHAR },
  { 0x1436, &val_hr_alg.Luchtmengkast[20].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[20].VerwarmingEnabled,           CHAR },
  { 0x1530, &val_hr_alg.Luchtmengkast[21].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[21].AnaInInblaastemp.board_type, CHAR },
  { 0x1531, &val_hr_alg.Luchtmengkast[21].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[21].AnaInMengtemp.board_type,    CHAR },
  { 0x1532, &val_hr_alg.Luchtmengkast[21].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[21].Enabled,                     CHAR },
  { 0x1533, &val_hr_alg.Luchtmengkast[21].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[21].Enabled,                     CHAR },
  { 0x1534, &val_hr_alg.Luchtmengkast[21].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[21].Enabled,                     CHAR },
  { 0x1535, &val_hr_alg.Luchtmengkast[21].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[21].AfblaasventEnabled,          CHAR },
  { 0x1536, &val_hr_alg.Luchtmengkast[21].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[21].VerwarmingEnabled,           CHAR },
  { 0x1630, &val_hr_alg.Luchtmengkast[22].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[22].AnaInInblaastemp.board_type, CHAR },
  { 0x1631, &val_hr_alg.Luchtmengkast[22].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[22].AnaInMengtemp.board_type,    CHAR },
  { 0x1632, &val_hr_alg.Luchtmengkast[22].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[22].Enabled,                     CHAR },
  { 0x1633, &val_hr_alg.Luchtmengkast[22].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[22].Enabled,                     CHAR },
  { 0x1634, &val_hr_alg.Luchtmengkast[22].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[22].Enabled,                     CHAR },
  { 0x1635, &val_hr_alg.Luchtmengkast[22].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[22].AfblaasventEnabled,          CHAR },
  { 0x1636, &val_hr_alg.Luchtmengkast[22].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[22].VerwarmingEnabled,           CHAR },
  { 0x1730, &val_hr_alg.Luchtmengkast[23].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[23].AnaInInblaastemp.board_type, CHAR },
  { 0x1731, &val_hr_alg.Luchtmengkast[23].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[23].AnaInMengtemp.board_type,    CHAR },
  { 0x1732, &val_hr_alg.Luchtmengkast[23].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[23].Enabled,                     CHAR },
  { 0x1733, &val_hr_alg.Luchtmengkast[23].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[23].Enabled,                     CHAR },
  { 0x1734, &val_hr_alg.Luchtmengkast[23].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[23].Enabled,                     CHAR },
  { 0x1735, &val_hr_alg.Luchtmengkast[23].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[23].AfblaasventEnabled,          CHAR },
  { 0x1736, &val_hr_alg.Luchtmengkast[23].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[23].VerwarmingEnabled,           CHAR },
  { 0x1830, &val_hr_alg.Luchtmengkast[24].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[24].AnaInInblaastemp.board_type, CHAR },
  { 0x1831, &val_hr_alg.Luchtmengkast[24].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[24].AnaInMengtemp.board_type,    CHAR },
  { 0x1832, &val_hr_alg.Luchtmengkast[24].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[24].Enabled,                     CHAR },
  { 0x1833, &val_hr_alg.Luchtmengkast[24].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[24].Enabled,                     CHAR },
  { 0x1834, &val_hr_alg.Luchtmengkast[24].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[24].Enabled,                     CHAR },
  { 0x1835, &val_hr_alg.Luchtmengkast[24].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[24].AfblaasventEnabled,          CHAR },
  { 0x1836, &val_hr_alg.Luchtmengkast[24].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[24].VerwarmingEnabled,           CHAR },
  { 0x1930, &val_hr_alg.Luchtmengkast[25].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[25].AnaInInblaastemp.board_type, CHAR },
  { 0x1931, &val_hr_alg.Luchtmengkast[25].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[25].AnaInMengtemp.board_type,    CHAR },
  { 0x1932, &val_hr_alg.Luchtmengkast[25].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[25].Enabled,                     CHAR },
  { 0x1933, &val_hr_alg.Luchtmengkast[25].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[25].Enabled,                     CHAR },
  { 0x1934, &val_hr_alg.Luchtmengkast[25].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[25].Enabled,                     CHAR },
  { 0x1935, &val_hr_alg.Luchtmengkast[25].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[25].AfblaasventEnabled,          CHAR },
  { 0x1936, &val_hr_alg.Luchtmengkast[25].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[25].VerwarmingEnabled,           CHAR },
  { 0x1A30, &val_hr_alg.Luchtmengkast[26].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[26].AnaInInblaastemp.board_type, CHAR },
  { 0x1A31, &val_hr_alg.Luchtmengkast[26].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[26].AnaInMengtemp.board_type,    CHAR },
  { 0x1A32, &val_hr_alg.Luchtmengkast[26].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[26].Enabled,                     CHAR },
  { 0x1A33, &val_hr_alg.Luchtmengkast[26].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[26].Enabled,                     CHAR },
  { 0x1A34, &val_hr_alg.Luchtmengkast[26].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[26].Enabled,                     CHAR },
  { 0x1A35, &val_hr_alg.Luchtmengkast[26].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[26].AfblaasventEnabled,          CHAR },
  { 0x1A36, &val_hr_alg.Luchtmengkast[26].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[26].VerwarmingEnabled,           CHAR },
  { 0x1B30, &val_hr_alg.Luchtmengkast[27].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[27].AnaInInblaastemp.board_type, CHAR },
  { 0x1B31, &val_hr_alg.Luchtmengkast[27].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[27].AnaInMengtemp.board_type,    CHAR },
  { 0x1B32, &val_hr_alg.Luchtmengkast[27].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[27].Enabled,                     CHAR },
  { 0x1B33, &val_hr_alg.Luchtmengkast[27].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[27].Enabled,                     CHAR },
  { 0x1B34, &val_hr_alg.Luchtmengkast[27].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[27].Enabled,                     CHAR },
  { 0x1B35, &val_hr_alg.Luchtmengkast[27].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[27].AfblaasventEnabled,          CHAR },
  { 0x1B36, &val_hr_alg.Luchtmengkast[27].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[27].VerwarmingEnabled,           CHAR },
  { 0x1C30, &val_hr_alg.Luchtmengkast[28].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[28].AnaInInblaastemp.board_type, CHAR },
  { 0x1C31, &val_hr_alg.Luchtmengkast[28].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[28].AnaInMengtemp.board_type,    CHAR },
  { 0x1C32, &val_hr_alg.Luchtmengkast[28].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[28].Enabled,                     CHAR },
  { 0x1C33, &val_hr_alg.Luchtmengkast[28].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[28].Enabled,                     CHAR },
  { 0x1C34, &val_hr_alg.Luchtmengkast[28].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[28].Enabled,                     CHAR },
  { 0x1C35, &val_hr_alg.Luchtmengkast[28].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[28].AfblaasventEnabled,          CHAR },
  { 0x1C36, &val_hr_alg.Luchtmengkast[28].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[28].VerwarmingEnabled,           CHAR },
  { 0x1D30, &val_hr_alg.Luchtmengkast[29].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[29].AnaInInblaastemp.board_type, CHAR },
  { 0x1D31, &val_hr_alg.Luchtmengkast[29].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[29].AnaInMengtemp.board_type,    CHAR },
  { 0x1D32, &val_hr_alg.Luchtmengkast[29].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[29].Enabled,                     CHAR },
  { 0x1D33, &val_hr_alg.Luchtmengkast[29].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[29].Enabled,                     CHAR },
  { 0x1D34, &val_hr_alg.Luchtmengkast[29].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[29].Enabled,                     CHAR },
  { 0x1D35, &val_hr_alg.Luchtmengkast[29].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[29].AfblaasventEnabled,          CHAR },
  { 0x1D36, &val_hr_alg.Luchtmengkast[29].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[29].VerwarmingEnabled,           CHAR },
  { 0x1E30, &val_hr_alg.Luchtmengkast[30].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[30].AnaInInblaastemp.board_type, CHAR },
  { 0x1E31, &val_hr_alg.Luchtmengkast[30].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[30].AnaInMengtemp.board_type,    CHAR },
  { 0x1E32, &val_hr_alg.Luchtmengkast[30].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[30].Enabled,                     CHAR },
  { 0x1E33, &val_hr_alg.Luchtmengkast[30].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[30].Enabled,                     CHAR },
  { 0x1E34, &val_hr_alg.Luchtmengkast[30].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[30].Enabled,                     CHAR },
  { 0x1E35, &val_hr_alg.Luchtmengkast[30].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[30].AfblaasventEnabled,          CHAR },
  { 0x1E36, &val_hr_alg.Luchtmengkast[30].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[30].VerwarmingEnabled,           CHAR },
  { 0x1F30, &val_hr_alg.Luchtmengkast[31].Inblaastemp,                  SIZE_INT,  &opt_app.Luchtmengkast[31].AnaInInblaastemp.board_type, CHAR },
  { 0x1F31, &val_hr_alg.Luchtmengkast[31].Mengtemp,                     SIZE_INT,  &opt_app.Luchtmengkast[31].AnaInMengtemp.board_type,    CHAR },
  { 0x1F32, &val_hr_alg.Luchtmengkast[31].Buitenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[31].Enabled,                     CHAR },
  { 0x1F33, &val_hr_alg.Luchtmengkast[31].Binnenklep.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[31].Enabled,                     CHAR },
  { 0x1F34, &val_hr_alg.Luchtmengkast[31].Inblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[31].Enabled,                     CHAR },
  { 0x1F35, &val_hr_alg.Luchtmengkast[31].Afblaasvent.Actual,           SIZE_CHAR, &opt_app.Luchtmengkast[31].AfblaasventEnabled,          CHAR },
  { 0x1F36, &val_hr_alg.Luchtmengkast[31].Verwarming.Actual,            SIZE_CHAR, &opt_app.Luchtmengkast[31].VerwarmingEnabled,           CHAR },
};


s_log_values log_values; // wordt telkens bij opstarten opnieuw gevuld

static unsigned int SD_Get_Log_Table_Index(unsigned int code)
// geeft index terug van sd_log_value_table als index gevonden
// anders return 0xFFFF
{
unsigned int loop;
unsigned char option;

  for (loop = 0; loop < sizeof(sd_log_value_table)/sizeof(s_log_index_value); loop++)
  {
    if (code == sd_log_value_table[loop].code)
    {
      switch (sd_log_value_table[loop].option_type)
      {
        default:        option = 0; break;
        case CHAR:
        case UCHAR:
        case TIME_CHAR: option = (*(unsigned char *)sd_log_value_table[loop].option_ptr != 0); break;
        case INT:
        case UINT:
        case TIME_INT:  option = (*(unsigned int *)sd_log_value_table[loop].option_ptr != 0); break;
        case LONG:      option = (*(unsigned long *)sd_log_value_table[loop].option_ptr != 0); break;
      }
      if (option)
        return (loop);
      break;
    }  
  }
  return (0xFFFF);
}

void SD_Log_Value_Toevoegen(unsigned int code)
{
unsigned int index;
s_log_value *ptr_value = &log_values.value[log_values.aantal_variabelen];

  if (log_values.length < 100)
  {
    index = SD_Get_Log_Table_Index(code);
    if (index != 0xFFFF)
    {
      ptr_value->code = sd_log_value_table[index].code;
      ptr_value->value_ptr = sd_log_value_table[index].value_ptr;
      ptr_value->value_size = sd_log_value_table[index].value_size;
      log_values.length += ptr_value->value_size;
      log_values.aantal_variabelen++;
    }
  }
}

static void SD_Log_Regel_Aanmaken(unsigned char aantal_variabelen, unsigned int *code_array)
{
unsigned char loop = 0;

  log_values.length = 4;
  log_values.aantal_variabelen = 0;
  for (loop = 0; loop < aantal_variabelen; loop++)
  {
    SD_Log_Value_Toevoegen(code_array[loop]);
  }
}

void SD_Log_Data_Aanmaken(unsigned char aantal_variabelen, unsigned int *code_array, unsigned int interval, time_t start_tijd, time_t stop_tijd)
{
unsigned char loop = 0;

  SD_Log_Regel_Aanmaken(aantal_variabelen, code_array);
  val_hr_alg.sd_log.length = log_values.length;
  val_hr_alg.sd_log.aantal_variabelen = log_values.aantal_variabelen;
  for (loop = 0; loop < log_values.aantal_variabelen; loop++)
    val_hr_alg.sd_log.code[loop] = log_values.value[loop].code;
  val_hr_alg.sd_log.start_tijd = start_tijd;
  val_hr_alg.sd_log.stop_tijd = stop_tijd;
  if (interval == 0)
    interval = 1;
  else if (interval > (unsigned int)60 * 60)
    interval = 60 * 60;
  val_hr_alg.sd_log.interval_in_seconden = interval;
  if ((start_tijd != 0) || (stop_tijd != 0))
    val_hr_alg.sd_log.changed = 1;
}

static void SD_Log_Regel_Controleren(void)
{
int loop;
unsigned char error = 0;

  SD_Log_Regel_Aanmaken(val_hr_alg.sd_log.aantal_variabelen, val_hr_alg.sd_log.code);
  if ((val_hr_alg.sd_log.aantal_variabelen != log_values.aantal_variabelen) ||
      (val_hr_alg.sd_log.length != log_values.length))
    error = 1;
  else
  {
    for (loop = 0; loop < log_values.aantal_variabelen; loop++)
    {
	  if (val_hr_alg.sd_log.code[loop] != log_values.value[loop].code)
      {
	    error = 1;
        break;
      }
	}
  }
  if (error)
  {
    val_hr_alg.sd_log.length = log_values.length;
    val_hr_alg.sd_log.aantal_variabelen = log_values.aantal_variabelen;
    for (loop = 0; loop < log_values.aantal_variabelen; loop++)
      val_hr_alg.sd_log.code[loop] = log_values.value[loop].code;
    if ((val_hr_alg.sd_log.start_tijd != 0) || (val_hr_alg.sd_log.stop_tijd != 0))
      val_hr_alg.sd_log.changed = 1;
  }
}

static void SD_Log_Header(void)
{
unsigned char loop;

  log_file[LOG].write.checksum = 0;
  File_Send_Char_Checksum(LOG_FILE, 2 + val_hr_alg.sd_log.aantal_variabelen * 2);
  File_Send_Int_Checksum(LOG_FILE, val_hr_alg.sd_log.aantal_variabelen);
  for (loop = 0; loop < val_hr_alg.sd_log.aantal_variabelen; loop++)
    File_Send_Int_Checksum(LOG_FILE, val_hr_alg.sd_log.code[loop]);
  File_Send_Char(LOG_FILE, log_file[LOG].write.checksum);
  File_Put_Char(LOG_FILE, '\r');
  File_Put_Char(LOG_FILE, '\n');
}

static void SD_Log_Data(void)
{
unsigned char loop = 0;

  log_file[LOG].write.checksum = 0;
  File_Send_Char_Checksum(LOG_FILE, log_values.length);
  File_Send_Long_Checksum(LOG_FILE, time(0));
  for (loop = 0; loop < log_values.aantal_variabelen; loop++)
  {
    switch (log_values.value[loop].value_size)
    {
      case SIZE_CHAR: File_Send_Char_Checksum(LOG_FILE, *(unsigned char *)log_values.value[loop].value_ptr); break;
      case SIZE_INT:  File_Send_Int_Checksum(LOG_FILE, *(unsigned int *)log_values.value[loop].value_ptr);  break;
      case SIZE_LONG: File_Send_Long_Checksum(LOG_FILE, *(unsigned long *)log_values.value[loop].value_ptr); break;  
    }
  }
  File_Send_Char(LOG_FILE, log_file[LOG].write.checksum);
  File_Put_Char(LOG_FILE, '\r');
  File_Put_Char(LOG_FILE, '\n');
  val_hr_alg.sd_log.aantal_regels++;
}

static void SD_Log_Regel(void)
{
  if (log_file[LOG].file_info_is_read)
  {
    if ((log_file[LOG].file_info.fsize == 0) &&
        (log_file[LOG].write.cnt == 0))
    {
      SD_Log_Header();
      log_file[LOG].write_time_out = WRITE_TIME_OUT_MAX + 1;
    }
    else
    {
      SD_Log_Data();
    }
  }  
}

void SD_Log_Control(void)
{
  if (sd_log_control_flag == 1)
  {
    sd_log_control_flag = 0;
    SD_Log_Regel_Controleren();
  }
  if ((setp_alg.sd_card_status == 0) &&
      ((sd_status & STA_PROTECT) == 0))
  {    
    switch (val_hr_alg.sd_log.changed)
    {
      default:
        setp_alg.sd_card_log_on[LOG] = 0;  
        val_hr_alg.sd_log.timer.interval = 0;
        break;
      case 0:
        if (setp_alg.sd_card_log_on[LOG])
        {
          if ((val_hr_alg.sd_log.aantal_variabelen == 0) ||
              ((val_hr_alg.sd_log.start_tijd == 0) && (val_hr_alg.sd_log.stop_tijd == 0)))
          {
            setp_alg.sd_card_log_on[LOG] = 0;
            val_hr_alg.sd_log.start_tijd = 0;
            val_hr_alg.sd_log.stop_tijd = 0;
            val_hr_alg.sd_log.timer.interval = 0;
          }
          else
          {
            if (time(0) >= val_hr_alg.sd_log.start_tijd)
            {
              if ((val_hr_alg.sd_log.stop_tijd == 0) || (time(0) <= val_hr_alg.sd_log.stop_tijd))
              {
                if (val_hr_alg.sd_log.timer.interval != val_hr_alg.sd_log.interval_in_seconden)
				{
                  Timer_Set(&val_hr_alg.sd_log.timer, val_hr_alg.sd_log.interval_in_seconden, TIME_BASE_1_SEC);
				  SD_Log_Regel();
				}
                if (Timer_Expired(&val_hr_alg.sd_log.timer))
                {
                  Timer_Reset(&val_hr_alg.sd_log.timer);
                  if (Timer_Expired(&val_hr_alg.sd_log.timer)) // zorgt ervoor dat als timer te groot is deze weer in de pas komt
                    Timer_Restart(&val_hr_alg.sd_log.timer);
                  SD_Log_Regel();
                  if (val_hr_alg.sd_log.aantal_regels >= SD_LOG_MAX_REGELS)
                    val_hr_alg.sd_log.changed = 1;
                }
              }  
              else
              {
                setp_alg.sd_card_log_on[LOG] = 0;
                val_hr_alg.sd_log.start_tijd = 0;
                val_hr_alg.sd_log.stop_tijd = 0;
                val_hr_alg.sd_log.timer.interval = 0;
              }  
            }    
            else
              val_hr_alg.sd_log.timer.interval = 0;
          }    
        }
        else
          val_hr_alg.sd_log.timer.interval = 0;
        break;
      case 1:
        log_file[LOG].rename_flag = 1;
        val_hr_alg.sd_log.aantal_regels = 0;
        val_hr_alg.sd_log.changed = 2;
        val_hr_alg.sd_log.timer.interval = 0;
        break;
      case 2:
        val_hr_alg.sd_log.timer.interval = 0;
        if ((log_file[LOG].rename_flag == 0) &&
            (log_file[LOG].file_info_is_read))
        {
          val_hr_alg.sd_log.changed = 0;
          val_hr_alg.sd_log.timer.interval = 0;
          if ((val_hr_alg.sd_log.start_tijd != 0) || (val_hr_alg.sd_log.stop_tijd != 0)) // als start en stoptijd gelijk aan 0 dan niet starten met loggen
          {
            setp_alg.sd_card_log_on[LOG] = 1;
            SD_Log_Regel();
          }  
        }
        break;
    }
  }
  else
    val_hr_alg.sd_log.timer.interval = 0;
}

void SD_Log_Einde_Dag(void)
{
  if ((val_hr_alg.sd_log.interval_in_seconden >= SD_LOG_EINDE_DAG_MIN_INTERVAL) && (val_hr_alg.sd_log.interval_in_seconden <= SD_LOG_EINDE_DAG_MAX_INTERVAL))
    val_hr_alg.sd_log.changed = 1;
}

#endif // SD_CARD
