#ifndef TEMP_MONITOR_H
#define TEMP_MONITOR_H

#include <stdbool.h>
#include <stdint.h>

typedef void (*temp_monitor_alarm_callback_t)(bool active, void *context);

typedef struct {
    int16_t trip_threshold_deci_c;
    int16_t clear_threshold_deci_c;
    uint8_t trip_sample_count;
    uint8_t clear_sample_count;
    temp_monitor_alarm_callback_t alarm_callback;
    void *callback_context;
} temp_monitor_config_t;

typedef enum {
    TEMP_MONITOR_NORMAL = 0,
    TEMP_MONITOR_ALARM
} temp_monitor_state_t;

typedef struct {
    temp_monitor_config_t config;
    temp_monitor_state_t state;
    uint8_t consecutive_count;
} temp_monitor_t;

bool temp_monitor_init(
    temp_monitor_t *monitor,
    const temp_monitor_config_t *config);

void temp_monitor_update(temp_monitor_t *monitor, int16_t temperature_deci_c);

bool temp_monitor_is_alarm_active(const temp_monitor_t *monitor);

#endif
