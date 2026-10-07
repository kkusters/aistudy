# Reverse-Engineering Baseline: Orion MC — Differential Pressure Sensors

## Scope

| Item | Value |
|---|---|
| Feature slice | Differential pressure sensor acquisition (`Drukverschil`) |
| Primary source | `inputs/orion-mc/sources/app/c__sensoren.c` (69 lines) |
| Interface | `inputs/orion-mc/sources/app/ch_sensoren.h` |
| In scope | `InitSensoren()`, `MainSensoren()`, its configuration and outputs |
| Out of scope | Modbus transport internals, alarm subsystem internals, display, other sensor types |

## Baseline record

| Item | Value |
|---|---|
| Repository commit | `d454c03` |
| MCU | Infineon XC161CJ, 16-bit |
| Toolchain | Tasking C166 (`Orion_MC_V4.pjt`) |
| Scheduler | Non-preemptive superloop in `c__main.c` |
| System tick | 5 ms (`SYSTEM_TIMER_BASE`) |
| Third-party stacks | BACnet, CANopen DS301/DS401, uIP 1.0 |
| Hardware unit | TBD — no physical unit measured |
| Firmware binary | TBD — not rebuilt |

## Evidence status of this analysis

All findings below are classified `Implemented`: derived by static source
analysis only. Nothing is `Observed`, because no hardware was executed and no
measurement was taken. No item may be promoted to `Approved` without
stakeholder review.

## Source evidence used

| File | Purpose |
|---|---|
| `c__sensoren.c` | Feature implementation |
| `ch_timer.h`, `c__timer.c` | Timer semantics and tick base |
| `ct_data.h` | `TOptSensoren`, `TValHrSensoren`, `MAX_SENSOREN` |
| `ch_mb_device.h` | Modbus device API contracts |
| `c__main.c` | Initialization and call frequency |
| `c__disp_opt_sensoren_2.c` | Configuration entry limits |
| `c__motor.c`, `c__device.c` | Established timer usage idiom |

## Key call-site facts

- `InitSensoren()` is called once during startup (`c__main.c:608`).
- `MainSensoren()` is called unconditionally in every superloop pass
  (`c__main.c:671`).
- The superloop has no fixed period; its duration varies with display,
  communication, and motor-control work.

## Open evidence gaps

| Gap | Impact | Owner |
|---|---|---|
| No hardware measurement of actual poll rate | Cannot confirm real timing behavior | TBD |
| Sensor unit and scaling of the returned value unknown | Cannot specify accuracy or range | TBD |
| Behavior on sensor loss not traced into `mbDevice` | Stale-value behavior unconfirmed | TBD |
| Intent of the unused `Type` field unknown | Cannot classify as defect or future feature | TBD |
