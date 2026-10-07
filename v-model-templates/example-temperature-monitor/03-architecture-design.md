# Architecture Design: Overtemperature Monitor

**Status:** Reverse-engineered draft
**Derived from:** SR-010 through SR-050

## Component view

```text
100 ms scheduler
      |
      v
temperature sensor adapter ----> temp_monitor ----> alarm callback
 platform-specific                 portable          platform-specific
```

## Decisions

| ID | Decision | Rationale | Requirements |
|---|---|---|---|
| AR-010 | A platform scheduler shall acquire one valid sample and call `temp_monitor_update()` every 100 ms. | Keeps time and hardware access outside decision logic. | SR-010 |
| AR-020 | `temp_monitor` shall contain only integer threshold, counter, and state-transition logic. | Makes behavior deterministic, host-testable, and portable. | SR-020, SR-030, SR-040, SR-050 |
| AR-030 | Sensor acquisition and alarm output shall be supplied through platform-owned adapters; the monitor shall signal output changes through a callback. | Isolates MCU, GPIO, bus, and sensor differences. | SR-050 |

## Interfaces

| Producer | Consumer | Data |
|---|---|---|
| Sensor adapter | Scheduler/application | `int16_t temperature_deci_c` |
| Scheduler/application | Monitor | One sample per invocation |
| Monitor | Alarm adapter | `bool active`, only when output changes |

## Concurrency and timing

- `temp_monitor_update()` executes in one scheduler context and is not
  reentrant.
- No ISR calls the module.
- The scheduler owns the 100 ms timing requirement.
- The module performs constant-time work and allocates no dynamic memory.

## Porting boundary

For a new platform, rewrite the scheduler hook, sensor adapter, and alarm
callback. Reuse the monitor source and unit tests without behavioral changes.
