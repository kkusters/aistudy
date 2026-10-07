// CH_LCD_BITMAP.H

#ifndef _CH_LCD_BITMAP_H
#define _CH_LCD_BITMAP_H

#include "ct_disp.h"

extern s_bitmap const ico_hotraco_groep;
extern s_bitmap const ico_cogaszuid;
extern s_bitmap const priva_ico;
extern s_bitmap const cogas_noord_ico;

extern s_bitmap const ico_empty;

extern s_disp_bitmap const disp_ana_input;
extern s_disp_bitmap const disp_ana_output;
extern s_disp_bitmap const disp_dig_input;
extern s_disp_bitmap const disp_dig_output;
extern s_disp_bitmap const disp_motor_control;
extern s_disp_bitmap const disp_RS485_bus;

extern s_bitmap const ico_RS485_bus;

extern s_bitmap const ico_aanwijzer;
extern s_bitmap const ico_box;
extern s_bitmap const ei_8x10_ico;

extern s_bitmap const ico_arrow_down_0;
extern s_bitmap const ico_arrow_up_0;
extern s_bitmap const ico_arrow_up_1;
extern s_bitmap const ico_check_off;
extern s_bitmap const ico_check_on;
extern s_bitmap const * const bmp_false_true[3];
extern s_bitmap const * const bmp_false_true_klein[3];
extern s_bitmap const ico_vink;
extern s_bitmap const ico_arrow_left_14;
extern s_bitmap const ico_arrow_right_14;
extern s_bitmap const ico_arrow_up_14;
extern s_disp_bitmap const disp_bevestig_arrow_up_bmp;
extern s_bitmap const ico_arrow_down_14;
extern s_bitmap const ico_enter_OK_14;
extern s_bitmap const ico_messages_screen_bot;
extern s_bitmap const ico_messages_screen_right;
extern s_bitmap const ico_alarm;
extern s_bitmap const ico_plus;
extern s_bitmap const ico_min;
extern s_bitmap const ico_abs;
extern s_bitmap const ico_alert;
extern s_bitmap const ico_error;
extern s_bitmap const ico_systeem;
extern s_bitmap const motor_communication_al_ico;
extern s_bitmap const vent_communication_al_ico;
extern s_bitmap const klep_communication_al_ico;
extern s_bitmap const sensor_communication_al_ico;

extern s_bitmap const ico_temp;
extern s_bitmap const ico_streef;
extern s_bitmap const ico_voer;
extern s_bitmap const ico_dierweging;
extern s_bitmap const ico_tijd;

extern s_bitmap const binnenklep_ico;
extern s_bitmap const buitenklep_ico;

extern s_bitmap const ico_fn_1;
extern s_bitmap const ico_fn_2;
extern s_bitmap const ico_fn_3;
extern s_bitmap const ico_fn_4_0;
extern s_bitmap const ico_fn_4_1;
extern s_bitmap const ico_fn_4_2;
extern s_bitmap const ico_fn_4_3;
extern s_bitmap const ico_fn_4_4;
extern s_bitmap const ico_fn_4_5;
extern s_bitmap const ico_fn_4_6;
extern s_bitmap const ico_fn_5;
extern s_bitmap const ico_fn_5_com;
extern s_bitmap const ico_fn_6;

extern s_disp_bitmap const disp_fn_F1;
extern s_disp_bitmap const disp_fn_F2;
extern s_disp_bitmap const disp_fn_F3;
extern s_disp_bitmap_array const disp_fn_alarm;
extern s_disp_bitmap_array const disp_fn_diagnose;
extern s_disp_bitmap const disp_fn_opties;

extern s_bitmap const ico_tab_begin;
extern s_bitmap const ico_tab_act;
extern s_bitmap const ico_tab_norm;
extern s_bitmap const ico_tab_end;

extern s_disp_bitmap const disp_tab_begin;
extern s_disp_bitmap const disp_tab_end;
extern s_disp_bitmap const disp_tab_2_norm;
extern s_disp_bitmap const disp_tab_3_norm;
extern s_disp_bitmap const disp_tab_4_norm;
extern s_disp_bitmap const disp_tab_5_norm;
extern s_disp_bitmap const disp_tab_6_norm;
extern s_disp_bitmap const disp_tab_2_act;
extern s_disp_bitmap const disp_tab_3_act;
extern s_disp_bitmap const disp_tab_4_act;
extern s_disp_bitmap const disp_tab_5_act;
extern s_disp_bitmap const disp_tab_6_act;

extern s_bitmap const zandloper_ico;

extern s_disp_bitmap const disp_rel_cursor;

extern s_disp_bitmap const disp_temp;
extern s_disp_bitmap const disp_tijd;
extern s_disp_bitmap const disp_dag;
extern s_disp_bitmap const disp_kalender;
extern s_disp_bitmap const disp_io;
extern s_disp_bitmap const disp_globel;

extern s_bitmap const ico_can_local;
extern s_disp_bitmap const disp_can_local;

extern s_disp_bitmap const disp_pen;
extern s_disp_bitmap const disp_bril;

extern s_disp_bitmap const disp_alarm_0;
extern s_disp_bitmap const disp_alarm_on;
extern s_disp_bitmap const disp_plus_0;
extern s_disp_bitmap const disp_min_0;
extern s_disp_bitmap const disp_abs_0;
extern s_disp_bitmap const disp_streef_0;
extern s_disp_bitmap const disp_streef_1;
extern s_disp_bitmap const disp_hysterese;
extern s_disp_bitmap const disp_bandbreedte_ico;

extern s_bitmap const ico_ingang;
extern s_disp_bitmap const disp_ingang_inst;
extern s_disp_bitmap const disp_ingang;
extern s_bitmap const ico_uitgang;
extern s_disp_bitmap const disp_copy_inst;
extern s_disp_bitmap const disp_uitgang_inst;
extern s_disp_bitmap const disp_uitgang;
extern s_disp_bitmap const disp_analoog;
extern s_disp_bitmap const disp_digitaal;
extern s_bitmap const ico_box_1;
extern s_bitmap const ico_box_2;
extern s_bitmap const ico_box_3;
extern s_bitmap const ico_box_4;
extern s_bitmap const ico_box_5;
extern s_bitmap const ico_box_6;
extern s_bitmap const ico_box_7;
extern s_bitmap const ico_box_8;
extern s_bitmap const ico_box_9;
extern s_bitmap const ico_box_10;
extern s_bitmap const ico_box_11;
extern s_bitmap const ico_box_12;
extern s_bitmap const ico_box_13;
extern s_bitmap const ico_box_14;
extern s_bitmap const ico_box_15;
extern s_bitmap const ico_box_16;

extern s_bitmap const ico_IO_CO2;
extern s_bitmap const ico_IO_Pa;
extern s_bitmap const ico_IO_RV;
extern s_bitmap const ico_IO_temperatuur;
extern s_bitmap const ico_IO_windrichting;
extern s_bitmap const ico_IO_windsnelheid;
extern s_bitmap const ico_IO_vent;
extern s_bitmap const ico_IO_klep;
extern s_bitmap const ico_IO_motor;
extern s_bitmap const ico_IO_raam;
extern s_bitmap const ico_IO_doek;
extern s_bitmap const ico_IO_lamel;
extern s_bitmap const ico_IO_verwarm;
extern s_bitmap const ico_IO_koel;
extern s_bitmap const ico_IO_licht;
extern s_bitmap const ico_IO_uni_reg;
extern s_bitmap const ico_IO_klok;
extern s_bitmap const ico_IO_alarm;
extern s_bitmap const ico_IO_tunnel;
extern s_bitmap const ico_IO_voer;
extern s_bitmap const ico_IO_voer_puls;
extern s_bitmap const ico_IO_voerweger;
extern s_bitmap const ico_IO_water;
extern s_bitmap const ico_IO_ei_puls;
extern s_bitmap const ico_IO_ei;
extern s_bitmap const ico_IO_hopper;
extern s_bitmap const ico_IO_silo;
extern s_bitmap const ico_IO_kWh_puls;
extern s_bitmap const ico_IO_dierweegschaal;
extern s_bitmap const ico_IO_mestdrg;

extern s_bitmap const verwarm_9x10_ico;
extern s_bitmap const ico_onderdruk;
extern s_bitmap const temp_5x10_ico;
extern s_bitmap const ico_kalender13x12;

extern s_bitmap const ico_orion;
extern s_disp_bitmap const disp_orion;

extern s_bitmap const ico_switch_on;
extern s_bitmap const ico_switch_off;
extern s_bitmap const ico_klok_switch_on;
extern s_bitmap const ico_klok_switch_off;
extern s_bitmap const ico_tijd_15x15;
extern s_bitmap const max_rpm_ico;
extern s_bitmap const fabrieksinst_ico;
extern s_bitmap const vermogen_ico;
extern s_bitmap const motor_temp_ico;

extern s_disp_data_component const disp_messagebox_bevestig;
extern s_disp_data_component const disp_messagebox_error;

extern s_bitmap const ico_buitentemp_0;
extern s_bitmap const ico_buitentemp_0_10;
extern s_bitmap const ico_buitentemp_10_25;
extern s_bitmap const ico_buitentemp_25;
extern s_bitmap const ico_buiten_wind;
extern s_disp_bitmap const disp_flag;
extern s_disp_bitmap const disp_comp_nr;
extern s_bitmap const ico_afvalemmer;
extern s_disp_bitmap const disp_afvalemmer;
extern s_disp_bitmap const disp_helderheid;
extern s_disp_bitmap const disp_lcd_dimmen;
extern s_disp_bitmap const disp_sleutel;
extern s_bitmap const huis_xl_ico;
extern s_disp_bitmap const disp_huis_xl_ico;

extern s_bitmap const output_ico;
extern s_bitmap const input_ico;

extern s_bitmap const raamstand_small_ico;
extern s_bitmap const motor_ico;

extern s_bitmap const raamstand_ico;
extern s_bitmap const doekstand_ico;
extern s_disp_bitmap const disp_raamstand_ico;
extern s_disp_bitmap const disp_doekstand_ico;

extern s_bitmap const raamstand_00_ico;
extern s_bitmap const raamstand_01_ico;
extern s_bitmap const raamstand_02_ico;
extern s_bitmap const raamstand_03_ico;
extern s_bitmap const raamstand_04_ico;
extern s_bitmap const raamstand_05_ico;
extern s_bitmap const doekstand_00_ico;
extern s_bitmap const doekstand_01_ico;
extern s_bitmap const doekstand_02_ico;
extern s_bitmap const doekstand_03_ico;
extern s_bitmap const doekstand_04_ico;
extern s_bitmap const doekstand_05_ico;
extern s_bitmap const cabriostand_00_ico;
extern s_bitmap const cabriostand_01_ico;
extern s_bitmap const cabriostand_02_ico;
extern s_bitmap const cabriostand_03_ico;
extern s_bitmap const cabriostand_04_ico;
extern s_bitmap const cabriostand_05_ico;

extern s_bitmap const raamstand_small_ico;
extern s_disp_bitmap const disp_raamstand_small_ico;
extern s_bitmap const doekstand_small_ico;
extern s_disp_bitmap const disp_doekstand_small_ico;

extern s_bitmap const * const ico_type_array[];

extern s_bitmap const vent_ico_17x17;
extern s_disp_bitmap const disp_vent_ico_17x17;

extern s_bitmap const ico_pijl_rechts;
extern s_bitmap const ico_vent;
extern s_bitmap const ico_klep_links;
extern s_disp_bitmap const disp_vent;
extern s_disp_bitmap const disp_vent_inst;
extern s_disp_bitmap const disp_verwarm;
extern s_disp_bitmap const disp_verwarm_inst;
extern s_disp_bitmap const disp_klep_links;
extern s_disp_bitmap const disp_klep_links_inst;
extern s_disp_bitmap const disp_cyclustijd;

extern s_disp_bitmap const disp_motorgroup_ico;
extern s_disp_bitmap const disp_motorgroup_small_ico;
extern s_disp_bitmap const disp_motor_ico;
extern s_disp_bitmap const disp_motor_inst;
extern s_disp_bitmap const disp_vent_min_val;
extern s_disp_bitmap const disp_vent_max_val;
extern s_disp_bitmap const disp_offset_ico;
extern s_disp_bitmap const disp_klok_15x15_ico;
extern s_disp_bitmap const disp_alarm_delay_ico;
extern s_disp_bitmap const disp_alarm_position_ico;
extern s_disp_bitmap const disp_alarm_urgent_ico;
extern s_disp_bitmap const disp_pulse_system_ico;
extern s_disp_bitmap const disp_kierregeling_ico;
extern s_disp_bitmap const disp_pulsetime_ico;
extern s_disp_bitmap const disp_cycletime_ico;
extern s_disp_bitmap const disp_schakelingen_ico;
extern s_disp_bitmap const disp_looptijd_ico;

extern s_bitmap const looptijd_raam_ico;
extern s_bitmap const looptijd_doek_ico;
extern s_disp_bitmap const disp_looptijd_raam_ico;
extern s_disp_bitmap const disp_looptijd_doek_ico;

extern s_bitmap const luchtmengkast_ico;

extern s_disp_bitmap const disp_motorgroepen_ico;
extern s_disp_bitmap const disp_groep_ico;
extern s_disp_bitmap const disp_cabriokas_ico;
extern s_disp_bitmap const disp_luchtmengkast_ico;
extern s_disp_bitmap const disp_sensoren_ico;
extern s_disp_bitmap const disp_sensoren_inst;
extern s_disp_bitmap const disp_drukverschil_ico;
extern s_disp_bitmap const disp_twee_doek_een_bed_inst;
extern s_disp_bitmap_array const disp_twee_doek_een_bed_ico;
extern s_disp_bitmap_option_on const disp_combimatic_a_b_ico;

extern s_disp_bitmap const disp_dualscreen_frame_ico;

extern s_bitmap const doek_00_ico;
extern s_bitmap const doek_01_ico;
extern s_bitmap const doek_02_ico;
extern s_bitmap const doek_03_ico;
extern s_bitmap const doek_04_ico;
extern s_bitmap const doek_05_ico;
extern s_bitmap const doek_06_ico;
extern s_bitmap const doek_07_ico;
extern s_bitmap const doek_08_ico;
extern s_bitmap const doek_09_ico;
extern s_bitmap const doek_10_ico;
extern s_bitmap const doek_11_ico;
extern s_bitmap const doek_12_ico;
extern s_bitmap const doek_13_ico;
extern s_bitmap const doek_14_ico;
extern s_bitmap const doek_15_ico;
extern s_bitmap const doek_16_ico;
extern s_bitmap const doek_17_ico;
extern s_bitmap const doek_18_ico;
extern s_bitmap const doek_19_ico;
extern s_bitmap const doek_20_ico;
extern s_bitmap const doek_21_ico;
extern s_bitmap const doek_22_ico;
extern s_bitmap const doek_23_ico;
extern s_bitmap const doek_24_ico;

extern s_bitmap const ico_bus_ok;
extern s_disp_bitmap_option_on const disp_bus_ok_ico; // JP nog verwijderen

extern s_bitmap const * const ico_klok_niet_ontvangen_zenden[3];

extern s_bitmap const ico_silo_FWS;

extern s_disp_bitmap const disp_password_help;

extern s_bitmap const ico_sd_card;
extern s_bitmap const ico_no_sd_card;
extern s_bitmap const ico_arrow_right;
extern s_bitmap const ico_cross_arrow_right;
extern s_bitmap const ico_arrow_left;
extern s_disp_bitmap const disp_sd_card;

extern s_bitmap const * const ico_pc_modem_big_array[];
extern s_disp_bitmap const disp_globe;

//-----------------------------------------------------------------------------

extern s_bitmap const luchtmengkast_bovenkant_ico;
extern s_bitmap const luchtmengkast_onderkant_ico;
extern s_bitmap const luchtmengkast_kanaal_ico;

extern s_bitmap const * const bmp_buitenklep_pos[8];
extern s_bitmap const * const bmp_binnenklep_pos[8];

//-----------------------------------------------------------------------------

extern s_bitmap const bovenklep_ico;
extern s_bitmap const vent_13x13_ico;

//-----------------------------------------------------------------------------
extern unsigned char screen_catcher_time_out;

//-----------------------------------------------------------------------------
extern s_bitmap const * const input_nr_ico[];
extern s_bitmap const input_small_ico;
extern s_bitmap const vent_small_ico;
extern s_bitmap const bolletje_ico;

#endif
