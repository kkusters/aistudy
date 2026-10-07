# Integration Test Plan: Overtemperature Monitor

**Validation counterpart:** Architecture design

| ID | Architecture | Setup and steps | Expected result |
|---|---|---|---|
| IT-010 | AR-010 | On target, timestamp 1,000 consecutive scheduler callbacks with a logic-analyzer pin toggled at entry. | Each period is 100 ms +/-5 ms. |
| IT-020 | AR-020, AR-030 | Use the real sensor adapter; inject 79.0 C twice, then 80.0 C three times. Observe the alarm-adapter test point. | Alarm changes only after the third qualifying sample. |
| IT-030 | AR-030 | Replace the sensor adapter with the new-platform implementation and replay the recorded temperature sequence. | Callback sequence and transition sample numbers match the reference platform. |

## Fault injection

Sensor communication failure handling belongs to the sensor adapter and must
be specified separately before this example is used in a real product.

## Report fields

| Test | HW/FW version | Date | Result | Evidence |
|---|---|---|---|---|
| IT-010 | | | Not run | |
| IT-020 | | | Not run | |
| IT-030 | | | Not run | |
