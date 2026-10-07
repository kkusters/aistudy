# System Requirements (Derived): Differential Pressure Sensor Acquisition

**Evidence status:** `Inferred` unless stated otherwise. Requires review.

These statements describe externally observable behavior intended for the new
platform. Where current behavior is a suspected defect, the requirement states
the intended behavior and the deviation is tracked in `open-issues.md`.

| ID | Requirement | Derived from | Verification | Status |
|---|---|---|---|---|
| SR-010 | The system shall support acquisition of up to 16 differential pressure sensors on the RS485 Modbus interface. | MD-020, `MAX_SENSOREN` | System test | Implemented |
| SR-020 | The system shall acquire each configured sensor value at a configured nominal period of 100 ms. | MD-010, AR-020 | System test | **Inferred — conflicts with current behavior, see OI-010** |
| SR-030 | The system shall address configured sensors consecutively starting at a configurable first Modbus address. | MD-020 | System test | Implemented |
| SR-040 | The system shall only communicate with Modbus addresses in the range 1 to 247 inclusive. | MD-030 | System test | Implemented |
| SR-050 | The system shall automatically establish a Modbus connection to a configured sensor that is not connected, and shall retry until connected. | MD-050 | Integration test | Implemented |
| SR-060 | The system shall make the most recent value of each configured sensor available to the user interface and application logic. | MD-060, AR-040 | System test | Implemented |
| SR-070 | The system shall raise a per-sensor alarm when a configured sensor is not communicating. | MD-040 | Integration test | **Inferred — alarm trigger condition not traced** |
| SR-080 | The system shall not write sensor data outside the storage allocated for the supported number of sensors, for any configuration value. | N-030 | Unit test and inspection | **Inferred — hardening requirement** |
| SR-090 | Sensor polling, indexing, and address validation shall be independent of MCU, RS485 hardware, and Modbus stack implementation. | AR-030, porting goal | Inspection and unit test | Inferred |

## Requirements that cannot yet be written

| Missing requirement | Blocking information |
|---|---|
| Value unit, scale, range, and accuracy | Sensor datasheet and `mbDeviceGetActualValue` scaling |
| Behavior when a sensor stops responding | `mbDevice` timeout and stale-value handling |
| Alarm set and clear conditions and delays | Alarm subsystem analysis |
| Analog sensor support | Purpose of the unused `Type` field |
