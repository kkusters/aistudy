# Module Design (LLD): `temp_monitor`

**Files:** `05-coding/temp_monitor.c`, `05-coding/temp_monitor.h`
**Derived from:** AR-020 and AR-030

## Public API

| API | Contract |
|---|---|
| `temp_monitor_init()` | Validate pointers, thresholds, and nonzero counts; initialize the monitor in NORMAL with zero counters. |
| `temp_monitor_update()` | Consume one valid temperature sample and update counters/state. |
| `temp_monitor_is_alarm_active()` | Return whether the module is in ALARM. |

## Detailed requirements

| ID | Requirement | Architecture source |
|---|---|---|
| MD-010 | In NORMAL, each sample at or above the trip threshold shall increment the trip counter; any lower sample shall reset it. The module shall enter ALARM when the configured trip count is reached. | AR-020 |
| MD-020 | In ALARM, each sample at or below the clear threshold shall increment the clear counter; any higher sample shall reset it. The module shall enter NORMAL when the configured clear count is reached. | AR-020 |
| MD-030 | The module shall invoke the alarm callback exactly once for each NORMAL-to-ALARM or ALARM-to-NORMAL transition and never for samples that do not change state. | AR-030 |
| MD-040 | Initialization shall reject null pointers, a clear threshold not below the trip threshold, and zero trip or clear counts. | AR-020 |

## State

| Field | Meaning |
|---|---|
| `config` | Thresholds, required consecutive counts, callback, callback context |
| `state` | NORMAL or ALARM |
| `consecutive_count` | Qualifying samples for the transition currently being evaluated |

## State transitions

```text
NORMAL -- trip_count samples >= trip_threshold --> ALARM
ALARM  -- clear_count samples <= clear_threshold --> NORMAL
```

Samples in the hysteresis band reset the counter relevant to the current state.
The counter saturates at its configured limit and cannot overflow.

## Porting notes

This module uses only fixed-width integers, `stdbool.h`, and a callback. It has
no platform-specific code and should be reused unchanged.
