# 04 — Window Control Slice (phase-1 migration target)

This is the vertical slice that actually has to be ported first. It replaces
`c__sensoren.c` as the active reverse-engineering slice.

Primary file: `inputs/orion-mc/sources/app/c__motorgroep.c` (≈1180 lines)
Supporting: `inputs/orion-mc/sources/app/c__motor.c`, `inputs/orion-mc/sources/io/c__IO_H1MC*.c`, `inputs/orion-mc/sources/io/c__IO_H2MC*.c`,
`inputs/orion-mc/sources/common/ct_data.h`

---

## 4.1 Execution model

`ControlMotorgroup(TMotorgroup *pGroup)` (`c__motorgroep.c:1067`) is called once per group
from the superloop. Everything is driven by two software timers:

```c
if (opt_app.Motorgroup[n].Enabled)
{
  if (pGroup->InitFlag) InitMotorgroup(pGroup);

  if (TimerExpired(&pGroup->Timer_100ms))
  {
      ControlHiSpeed(pGroup);            // 100 ms
      GetRunningModeMotorgroup(pGroup);  // 100 ms
      ControlVirtualMotor(pGroup);       // 100 ms  <-- integrates position

      if (TimerExpired(&pGroup->Timer_1s))
      {
        TimerReset(&pGroup->Timer_1s);   // 1 s
        ControlAlarmMotorgroup(pGroup);
        GetAveragePosition(pGroup);
        GetRuntimeMotorgroup(pGroup);
      }

      switch (opt_app.Motorgroup[n].Type) { ... }   // type dispatch

      TimerReset(&pGroup->Timer_100ms);   //  <-- correctly paired
      pGroup->Disabled = 0;
      ...
  }
}
else if (pGroup->Disabled == 0) { ResetAlarmMotorgroup(pGroup); pGroup->Disabled = 1; }
```

`Implemented`

### Two observations that matter

**1. The 100 ms tick is load-bearing, and here it is correct.**
`ControlVirtualMotor()` increments `PositionTime` by exactly 1 per invocation, and `Runtime`
is documented in `ct_data.h` as `// Time from 0-100% [x100ms]`. The unit of simulated
position *is* the timer period. If the tick rate drifts, every simulated position drifts
with it.

This module pairs `TimerSet` (line 88) / `TimerExpired` (1075) / `TimerReset` (1129)
correctly. **That is strong corroboration for defect candidate OI-010**: the same idiom in
`c__sensoren.c` omits the `TimerReset`, and here we can see what the author's intended
pattern was. OI-010 should now be treated as a confirmed coding defect rather than an
ambiguity.

**2. Disable is edge-triggered, not level-triggered.**
`pGroup->Disabled` ensures `ResetAlarmMotorgroup()` runs exactly once on the
enabled→disabled transition. A naive port that calls it every pass would continuously clear
alarms. Small detail, easy to lose, visible only in the field.

## 4.2 Type dispatch

| Group `Type` | `ControlType` | Handler | Phase 1? |
|---|---|---|---|
| `TYPE_RAAM` (window) | `CONTROL_NORMAL` | `ControlWindow()` | **Yes** |
| `TYPE_RAAM` | `CONTROL_CABRIOKAS` | `ControlCabriokas()` | No — explicitly excluded |
| `TYPE_DOEK` (screen) | `CONTROL_NORMAL` | `ControlScreen()` | No |
| `TYPE_DOEK` | `CONTROL_DUALSCREEN` | `ControlDualScreen()` | No |
| `TYPE_VENT` | — | `ControlVent()` | No |
| `TYPE_KLEP` (flap) | — | `ControlKlep()` | No |

Only the first row is in phase-1 scope. `Documented` + `Implemented`

## 4.3 `ControlWindow()` — the function to port

```c
static void ControlWindow(TMotorgroup *pGroup)      // line 695
{
  MotorgroupGetTargetPosition(pGroup);              // 1. acquire demand
  MotorgroupSetFeedback(pGroup, pGroup->PositionAvg); // 2. report feedback
  SetPositionAllMotors(pGroup);                     // 3. distribute to motors

  for each motor:                                   // 4. apply hold flags
    pMotor->Flags.HoldOpen  ? SetMotorHold(pMotor, rmOpen)  : SetMotorRelease(pMotor, rmOpen);
    pMotor->Flags.HoldClose ? SetMotorHold(pMotor, rmClose) : SetMotorRelease(pMotor, rmClose);
}
```

Four steps, in order, every 100 ms. The whole window-control behaviour is this plus the
functions it calls. `Implemented`

Note that **feedback is sent before the new demand is distributed**, and uses
`PositionAvg`, which was last refreshed in the 1 s block. So feedback is up to one second
stale relative to the measured positions. Whether that is intentional or incidental is a
question for the vendor — but a new platform that "fixes" it by recomputing fresh changes
the timing seen by the climate computer.

## 4.4 Data model

```c
#define MAX_GROUP  32   // ct_data.h:222
#define MAX_MOTOR  64   // ct_data.h:223
```

Groups own motors through an intrusive linked list:

```
TMotorgroup.FirstMotor ──► TMotor.Next ──► TMotor.Next ──► NULL
TMotorgroup.LastMotor  ──────────────────────┘
TMotorgroup.Link       ──► another TMotorgroup   (coupled screens)
```

Built in `c__main.c` (~line 401) by walking the configured motors and appending each to its
group. `Implemented`

Parallel flat arrays, all indexed by group or motor number, separate configuration from
runtime state:

| Array | Purpose |
|---|---|
| `opt_app.Motorgroup[]`, `opt_app.Motor[]` | Configuration — runtime, type, IO channel assignment |
| `val_hr_alg.Motorgroup[]`, `val_hr_alg.Motor[]` | Runtime values — `PositionPerc`, `PositionTime`, `OperationMode` |
| `alarm_hr_alg.Motorgroup[]`, `alarm_hr_alg.Motor[]` | Alarm state |
| `setp_alg.Motorgroup[]` | Setpoints |

> **Porting note.** This separation is good and should survive the port. The
> *global-singleton* access pattern (`opt_app`, `val_hr_alg` referenced directly from deep
> inside control functions) should not: it makes the control logic untestable in isolation.
> Passing a context struct would let the virtual-motor integration and the gap control be
> unit-tested on a host, exactly as the worked example in
> `v-model-templates/example-temperature-monitor/` demonstrates. This is the single highest-value
> structural change available in the migration.

## 4.5 Derived requirement candidates

Draft only. Not approved. Each needs vendor confirmation before entering the V-Model
requirement baseline.

| ID | Candidate requirement | Source | Status |
|---|---|---|---|
| SR-100 | The system shall support at least 32 motor groups. | `ct_data.h:222` | `Implemented`, but see D-01 |
| SR-101 | The system shall support at least 64 motors. | `ct_data.h:223` | `Conflict` — target is ~128 |
| SR-102 | The system shall represent position in tenths of a percent, 0…1000. | `c__motorgroep.c` | `Implemented` |
| SR-103 | The system shall update motor-group control at a 100 ms period. | `c__motorgroep.c:1075` | `Implemented` |
| SR-104 | The system shall update alarms, average position and runtime at a 1 s period. | `c__motorgroep.c:1081` | `Implemented` |
| SR-105 | For windows, 0 % shall mean fully closed and 100 % fully open. | manual | `Documented` |
| SR-106 | When a group is set to OFF, the system shall drive it to 0 %. | `c__motorgroep.c:340` + manual | `Confirmed` |
| SR-107 | The system shall simulate group position by integrating running time against a calibrated runtime, clamped to [0, Runtime]. | `c__motorgroep.c:641` | `Implemented` |
| SR-108 | The system shall stagger motor starts within a group using a configurable delay. | `SetPositionAllMotors()` | `Implemented` |
| SR-109 | The system shall de-energise the alarm contact on a hard alarm. | manual | `Documented` |
| SR-110 | The system shall output 50 % feedback when feedback is invalid. | `c__motorgroep.c:396` | `Implemented` — confirm intent |
| SR-111 | The system shall accept a position demand per motor group over a digital open/close interface or a 0–10 V analog interface. No other upstream protocol (CANopen backbone, BACnet, Hoogendoorn) is required. | `c__motorgroep.c:301`, reduced per product-owner scope decision 2026-10-01 | `Approved` — see `06-migration-scope.md` §6.1a |
| SR-112 | The system shall retain the CAN Local interface to H1MC/H2MC motor-controller modules, unaffected by SR-111. | `io/c__IO_H1MC*.c`, `io/c__IO_H2MC*.c` | `Approved` — explicitly confirmed with product owner 2026-10-01 |

## 4.6 Suggested next actions on this slice

1. Read `c__motor.c` in full and document the per-motor state machine (the group layer is now
   understood; the motor layer is not).
2. Specify the H1MC/H2MC CANopen interface as a frozen ICD — object dictionary, PDO map,
   timing. Ideally verify against a live bus capture. **Unaffected by the 2026-10-01 scope
   decision** — this is the retained CAN Local link (SR-112), not the removed CAN Backbone.
3. Resolve D-01 and D-02 from `05-doc-vs-code-findings.md` with the vendor.
4. Write host-runnable unit tests for `ControlVirtualMotor()` against SR-107 *before* porting,
   using the legacy code as the oracle.
5. ~~Design/port the CANopen-backbone and BACnet upstream paths.~~ **Dropped** — removed
   from scope 2026-10-01 (SR-111). Do not port `opt_alg.can_backbone` / `BACnet_enabled` /
   `hoogendoorn_enabled` branches of `MotorgroupGetTargetPosition()` or `MotorgroupSetFeedback()`.
6. Confirm whether the digital open/close interface needs its own feedback channel, or
   whether feedback is analog-only (see `06-migration-scope.md` §6.1a "Effect on feedback").
