# Architecture Design (Recovered): Differential Pressure Sensor Acquisition

**Evidence status:** `Implemented`

## Structure

```text
c__main.c superloop
      | calls every pass
      v
  MainSensoren()            <-- portable polling and indexing logic
      |            \
      | mbDevice*   \ alarm registration
      v              v
 Modbus RS485      alarm subsystem
 transport
      |
      v
 val_hr_alg.Sensoren.Drukverschil[]  --> display, application
```

## Architecture decisions

| ID | Decision as implemented | Rationale inferred | Porting |
|---|---|---|---|
| AR-010 | Sensor acquisition runs as a cooperative polled task in the superloop, with no ISR involvement. | Simple, deterministic ordering on a single-core 16-bit MCU. | Reassess |
| AR-020 | Scheduling is intended to be time-gated by a shared 5 ms tick timer abstraction (`TTimer`). | Reuses the common timing idiom of the codebase. | Adapt |
| AR-030 | Modbus transport, device lifecycle, and connection state are owned by the `mbDevice` layer; this module only requests connect and read. | Isolates protocol handling from application polling. | Adapt |
| AR-040 | Sensor values are published through a shared global value structure rather than returned to callers. | Matches the codebase-wide `val_hr_*` publication pattern. | Portable |
| AR-050 | Alarms are registered through the central alarm subsystem keyed by Modbus address. | Centralizes alarm handling across device types. | Adapt |
| AR-060 | Configuration limits are enforced by the UI entry layer rather than by the consuming module. | Historical pattern; weak defensively. | Reassess |

## Interfaces

| Producer | Consumer | Data |
|---|---|---|
| `opt_app.Sensoren` | `MainSensoren` | Count, first address, bus mapping, type |
| `MainSensoren` | `mbDevice` | Connect requests, value reads, alarm registration |
| `MainSensoren` | `val_hr_alg.Sensoren` | `int` differential-pressure values |

## Timing

Intended: one acquisition cycle per 100 ms.
Actual: one cycle per superloop pass after the first 100 ms, because the timer
is never reset (see MD note N-010). Superloop period is unmeasured.

## Platform boundary for migration

Replace: RS485 board IO mapping, Modbus transport, timer tick source, alarm
backend. Reuse: sensor indexing, address validation, per-sensor state
sequencing, value publication contract.
