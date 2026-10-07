# System Test Plan: Overtemperature Monitor

**Validation counterpart:** System requirements

Use a production-representative assembled product in a controlled thermal
environment. Measure temperature at the software sensor location and record
alarm output with synchronized timestamps.

| ID | Requirements | Procedure | Acceptance criterion |
|---|---|---|---|
| ST-010 | SR-010 | Hold temperature at 70.0 C and record 1,000 sample events. | Every sample interval is 100 ms +/-5 ms. |
| ST-020 | SR-020 | Stabilize below 75.0 C, then step to at least 80.0 C. | Alarm is asserted on the third consecutive qualifying sample, no later than 305 ms after the first. |
| ST-030 | SR-030 | With alarm active, step to at most 75.0 C. | Alarm clears on the fifth consecutive qualifying sample, no later than 505 ms after the first. |
| ST-040 | SR-040 | Hold at 77.5 C for 10 seconds first with alarm clear and then with alarm active. | Alarm state does not change in either run. |
| ST-050 | SR-050 | Build and run the same module unit tests on the reference and new-platform host toolchains. | Both executions pass without source changes to the module or tests. |

## Report

| Test | Product serial | HW/FW version | Result | Evidence |
|---|---|---|---|---|
| ST-010 | | | Not run | |
| ST-020 | | | Not run | |
| ST-030 | | | Not run | |
| ST-040 | | | Not run | |
| ST-050 | | | Not run | |
