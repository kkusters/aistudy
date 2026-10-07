# System Requirements Specification (SRS)

**Product:** <name/model number>
**Version:** <doc version>
**Traces to:** Business Requirements (BR-xxx)

## 1. Reverse-Engineering Notes
> Fill only when deriving from code/hardware.
- How system-level behavior was observed (bench test, code reading, logic analyzer, existing test reports):
- Hardware revision / BOM used:
- Open questions / unverifiable behavior (mark TBD):

## 2. System Overview
- Block diagram (link image/drawio) of system in its environment (sensors, actuators, host/cloud, HMI).
- Operating environment (temp, humidity, EMC, power).

## 3. Functional System Requirements
| ID | Requirement | Derived from (BR) | Verification method (Test/Analysis/Inspection/Demo) |
|----|-------------|--------------------|--------------------------------------------------------|
| SR-010 | e.g. "System shall sample temperature every 100 ms ±5 ms" | BR-010 | Test |
| SR-020 | | | |

## 4. Non-Functional System Requirements
| ID | Category | Requirement | Derived from (BR) |
|----|----------|-------------|--------------------|
| SR-100 | Performance | | |
| SR-110 | Reliability/MTBF | | |
| SR-120 | Safety | | |
| SR-130 | Security | | |
| SR-140 | Power consumption | | |
| SR-150 | EMC/EMI | | |
| SR-160 | Environmental | | |
| SR-170 | Maintainability/Field update (OTA/bootloader) | | |

## 5. External Interfaces
| Interface | Type (UART/I2C/SPI/CAN/BLE/Wi-Fi/Analog/GPIO) | Protocol/Standard | Requirement ID |
|-----------|--------------------------------------------------|--------------------|-----------------|

## 6. System States / Modes
| State | Entry condition | Exit condition | Power mode | Notes |
|-------|------------------|-----------------|------------|-------|
| e.g. Off | | | | |
| Init | | | | |
| Run | | | | |
| Fault | | | | |
| Low-power/Sleep | | | | |

## 7. Timing Budget
| Event/loop | Deadline | Measured (if reverse-engineered) | Requirement ID |
|------------|----------|------------------------------------|-----------------|

## 8. System Test Plan Summary (detail in layer 08 System Test)
| SR ID | Linked System Test ID(s) |
|-------|----------------------------|

## 9. Traceability
- Upstream: BR-xxx
- Downstream: AR-xxx (architecture), ST-xxx (system test)

## 10. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
