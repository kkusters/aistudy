# Architecture Design Document (SAD)

**Product:** <name/model number>
**Traces to:** System Requirements (SR-xxx)

## 1. Reverse-Engineering Notes
- Static analysis method used (call graph tool, map file, linker script, IDE cross-reference):
- MCU/SoC part number, revision, package:
- Clock tree source (datasheet/CubeMX/reg dump):

## 2. Hardware/Software Boundary
- MCU/SoC:
- Key peripherals used (timers, ADC, DMA, comms):
- External components software must interact with (sensors, power management ICs, radios):

## 3. Software Architecture Overview
- Architectural style (bare-metal superloop / RTOS tasks / layered / event-driven):
- Block diagram: layers (HAL, drivers, middleware, application, BSP).
- Third-party stacks used (RTOS, TCP/IP, USB, BLE stack, filesystem) + versions/licenses.

## 4. Module/Component Map
| Component | Responsibility | Depends on | Requirement ID (SR) | Design ID (AR) |
|-----------|-----------------|------------|-----------------------|-----------------|
| e.g. sensor_driver | Read temp sensor over I2C | i2c_hal | SR-010 | AR-010 |

## 5. Concurrency / Task Design
| Task/ISR | Priority | Period/Trigger | Stack size | Shared resources | Notes |
|----------|----------|------------------|------------|--------------------|-------|

## 6. Interrupt Map
| IRQ | Source | Priority | Handler | Latency requirement |
|-----|--------|----------|---------|-----------------------|

## 7. Memory Map
| Region | Address range | Size | Used for | Notes |
|--------|----------------|------|----------|-------|
| Flash - bootloader | | | | |
| Flash - application | | | | |
| Flash - config/NVM | | | | |
| RAM | | | | |

## 8. Communication & Data Flow
- Internal (inter-task queues/mailboxes/events):
- External (bus protocols, message formats, byte order):
- Data flow diagram (link).

## 9. Error Handling & Fault Strategy
- Watchdog strategy:
- Fault/exception handler behavior:
- Logging/diagnostics:

## 10. Safety/Security Architecture (if applicable)
- Isolation strategy (MPU regions, dual-bank, secure boot):
- Threat model reference:

## 11. Platform Abstraction (critical for future porting)
- Which components are portable (platform-independent business logic)?
- Which components are platform-specific (HAL/BSP/drivers) and must be
  rewritten per target?
- Recommended interface boundary for the port (e.g. `hal_*` function table).

| Component | Portable? | Porting notes for new platform |
|-----------|-----------|----------------------------------|

## 12. Architecture Decisions (ADR-style)
| AR ID | Decision | Alternatives considered | Rationale | Requirement(s) satisfied |
|-------|----------|---------------------------|-----------|-----------------------------|
| AR-010 | | | | SR-010 |

## 13. Integration Test Plan Summary (detail in layer 07 Integration Test)
| AR ID | Linked Integration Test ID(s) |
|-------|----------------------------------|

## 14. Traceability
- Upstream: SR-xxx
- Downstream: MD-xxx (module design), IT-xxx (integration test)

## 15. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
