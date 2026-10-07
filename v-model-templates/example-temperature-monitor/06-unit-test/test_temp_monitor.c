#include "temp_monitor.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    unsigned int calls;
    bool active;
} alarm_spy_t;

static void alarm_spy(bool active, void *context)
{
    alarm_spy_t *spy = context;
    spy->calls++;
    spy->active = active;
}

static temp_monitor_t new_monitor(alarm_spy_t *spy)
{
    temp_monitor_t monitor;
    const temp_monitor_config_t config = {
        .trip_threshold_deci_c = 800,
        .clear_threshold_deci_c = 750,
        .trip_sample_count = 3,
        .clear_sample_count = 5,
        .alarm_callback = alarm_spy,
        .callback_context = spy,
    };

    assert(temp_monitor_init(&monitor, &config));
    return monitor;
}

static void test_ut_010_alarm_after_three_high_samples(void)
{
    alarm_spy_t spy = {0};
    temp_monitor_t monitor = new_monitor(&spy);

    temp_monitor_update(&monitor, 800);
    temp_monitor_update(&monitor, 810);
    assert(!temp_monitor_is_alarm_active(&monitor));

    temp_monitor_update(&monitor, 805);
    assert(temp_monitor_is_alarm_active(&monitor));
    assert(spy.calls == 1U);
    assert(spy.active);
}

static void test_ut_020_nonqualifying_sample_resets_trip_count(void)
{
    alarm_spy_t spy = {0};
    temp_monitor_t monitor = new_monitor(&spy);

    temp_monitor_update(&monitor, 800);
    temp_monitor_update(&monitor, 799);
    temp_monitor_update(&monitor, 810);
    temp_monitor_update(&monitor, 810);
    assert(!temp_monitor_is_alarm_active(&monitor));
    assert(spy.calls == 0U);
}

static void test_ut_030_alarm_clears_after_five_low_samples(void)
{
    alarm_spy_t spy = {0};
    temp_monitor_t monitor = new_monitor(&spy);
    unsigned int sample;

    for (sample = 0U; sample < 3U; sample++) {
        temp_monitor_update(&monitor, 800);
    }
    for (sample = 0U; sample < 4U; sample++) {
        temp_monitor_update(&monitor, 750);
    }
    assert(temp_monitor_is_alarm_active(&monitor));

    temp_monitor_update(&monitor, 750);
    assert(!temp_monitor_is_alarm_active(&monitor));
    assert(spy.calls == 2U);
    assert(!spy.active);
}

static void test_ut_040_hysteresis_does_not_change_state(void)
{
    alarm_spy_t spy = {0};
    temp_monitor_t monitor = new_monitor(&spy);
    unsigned int sample;

    for (sample = 0U; sample < 10U; sample++) {
        temp_monitor_update(&monitor, 775);
    }
    assert(!temp_monitor_is_alarm_active(&monitor));
    assert(spy.calls == 0U);
}

static void test_ut_050_invalid_configuration_is_rejected(void)
{
    alarm_spy_t spy = {0};
    temp_monitor_t monitor;
    temp_monitor_config_t config = {
        .trip_threshold_deci_c = 800,
        .clear_threshold_deci_c = 800,
        .trip_sample_count = 3,
        .clear_sample_count = 5,
        .alarm_callback = alarm_spy,
        .callback_context = &spy,
    };

    assert(!temp_monitor_init(&monitor, &config));
}

int main(void)
{
    test_ut_010_alarm_after_three_high_samples();
    test_ut_020_nonqualifying_sample_resets_trip_count();
    test_ut_030_alarm_clears_after_five_low_samples();
    test_ut_040_hysteresis_does_not_change_state();
    test_ut_050_invalid_configuration_is_rejected();
    puts("All temp_monitor unit tests passed.");
    return 0;
}
