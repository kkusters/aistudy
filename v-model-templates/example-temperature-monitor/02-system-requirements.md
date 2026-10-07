# System Requirements: Overtemperature Monitor

**Status:** Reverse-engineered draft
**Derived from:** BR-010 through BR-030

## Functional requirements

| ID | Requirement | Source | Verification |
|---|---|---|---|
| SR-010 | While operational, the system shall provide one temperature sample to the monitor every 100 ms, with a tolerance of +/-5 ms. | BR-010 | System test |
| SR-020 | The system shall assert the overtemperature alarm no later than the completion of the third consecutive sample at or above 80.0 C. | BR-010, BR-020 | System test |
| SR-030 | After asserting the alarm, the system shall clear it no later than the completion of the fifth consecutive sample at or below 75.0 C. | BR-020 | System test |
| SR-040 | Samples between 75.0 C and 80.0 C shall neither assert a cleared alarm nor clear an asserted alarm. | BR-020 | System test |
| SR-050 | The alarm decision logic shall not depend directly on MCU registers, an RTOS, or a specific temperature-sensor driver. | BR-030 | Inspection and unit test |

## External interfaces

| Interface | Description |
|---|---|
| Temperature input | Signed temperature in tenths of a degree Celsius |
| Alarm output | Boolean request passed to a platform-owned output function |
| Scheduler | Platform invokes the monitor once per valid 100 ms sample |

## System modes

| Mode | Monitor behavior |
|---|---|
| Initialization | Alarm clear; consecutive-sample counters zero |
| Normal | Count samples at or above 80.0 C |
| Alarm | Count samples at or below 75.0 C |

## Reverse-engineering evidence

- The 800 and 750 constants in `temp_monitor.c` represent tenths of a degree.
- The trip and clear counts are passed in configuration by the application.
- Timing depends on the caller; it is not enforced by the portable module.
