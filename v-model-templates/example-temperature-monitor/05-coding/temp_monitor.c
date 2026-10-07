#include "temp_monitor.h"

bool temp_monitor_init(
    temp_monitor_t *monitor,
    const temp_monitor_config_t *config)
{
    if ((monitor == 0) || (config == 0) ||
        (config->alarm_callback == 0) ||
        (config->clear_threshold_deci_c >= config->trip_threshold_deci_c) ||
        (config->trip_sample_count == 0U) ||
        (config->clear_sample_count == 0U)) {
        return false;
    }

    monitor->config = *config;
    monitor->state = TEMP_MONITOR_NORMAL;
    monitor->consecutive_count = 0U;
    return true;
}

void temp_monitor_update(temp_monitor_t *monitor, int16_t temperature_deci_c)
{
    if (monitor == 0) {
        return;
    }

    if (monitor->state == TEMP_MONITOR_NORMAL) {
        /* @implements MD-010, SR-020 */
        if (temperature_deci_c >= monitor->config.trip_threshold_deci_c) {
            monitor->consecutive_count++;
            if (monitor->consecutive_count >=
                monitor->config.trip_sample_count) {
                monitor->state = TEMP_MONITOR_ALARM;
                monitor->consecutive_count = 0U;
                monitor->config.alarm_callback(
                    true, monitor->config.callback_context);
            }
        } else {
            monitor->consecutive_count = 0U;
        }
    } else {
        /* @implements MD-020, SR-030 */
        if (temperature_deci_c <= monitor->config.clear_threshold_deci_c) {
            monitor->consecutive_count++;
            if (monitor->consecutive_count >=
                monitor->config.clear_sample_count) {
                monitor->state = TEMP_MONITOR_NORMAL;
                monitor->consecutive_count = 0U;
                monitor->config.alarm_callback(
                    false, monitor->config.callback_context);
            }
        } else {
            monitor->consecutive_count = 0U;
        }
    }
}

bool temp_monitor_is_alarm_active(const temp_monitor_t *monitor)
{
    return (monitor != 0) && (monitor->state == TEMP_MONITOR_ALARM);
}
