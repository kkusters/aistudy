# 03 — Product Functions (revised)

Supersedes `../orion-mc-re-001/product-functions.md` F-01…F-09. The earlier list was derived from
code alone and framed the product as a climate controller. This list is derived from code
**and** official documentation.

| ID | Function | Status |
|---|---|---|
| F-01 | Accept a position demand per motor group from a horticulture computer | `Confirmed` |
| F-02 | Present each motor group upstream as a single virtual motor | `Confirmed` |
| F-03 | Translate group demand into individual motor commands | `Confirmed` |
| F-04 | Simulate position where no encoder feedback exists | `Confirmed` |
| F-05 | Aggregate and report group position feedback upstream | `Conflict` — see F-05 note |
| F-06 | Detect, classify and report motor and system alarms | `Confirmed` |
| F-07 | Provide local operator control and override (AUTO/HAND/OFF) | `Confirmed` |
| F-08 | Protect coupled screens from collision (gap control) | `Implemented` |
| F-09 | Configure, calibrate and commission the installation | `Documented` |
| F-10 | Provide generic greenhouse IO beyond motor control | `Implemented` |

---

## F-01 — Accept a position demand per motor group

The Orion-MC receives a desired position, in tenths of a percent (0…1000), for each motor
group.

> **⚠️ Scope decision (2026-10-01):** the legacy product supports five upstream
> interfaces — CANopen DS-401, BACnet, Hoogendoorn, analog, and digital open/close. **The
> product owner has reduced the new platform's upstream interface to just two: digital
> open/close contacts and 0–10 V analog.** CANopen backbone, BACnet and the Hoogendoorn
> protocol are no longer required. This does **not** affect CAN Local to H1MC/H2MC
> (F-03/IF-08), which is a separate, downstream link and stays in scope.

Position convention: **windows 0 % = closed, 100 % = open**. Screens are frequently the
inverse, and that inversion is a *group configuration* item rather than a separate code path.
`Documented`

Implementation: `MotorgroupGetTargetPosition()`, `c__motorgroep.c:301`. See
[`02-system-context.md`](02-system-context.md) §2.2 for the full legacy priority chain and
§2.5 for the reduced-scope interface table, and `06-migration-scope.md` §6.1a for the
rationale and impact.

## F-02 — Present each group as a single virtual motor

> *"The Orion-MC presents itself to the climate computer as one virtual motor per group."*

The upstream system sees one demand input and one feedback output per group, regardless of
how many physical motors are behind it. `Confirmed` — named function `ControlVirtualMotor()`.

For **open/close (digital) control**, the documentation states the simulation uses the
**longest calibrated running time in the group** — the group is only fully open when its
slowest motor is fully open. `Documented`

## F-03 — Translate group demand into individual motor commands

Each group holds a linked list of motors (`pGroup->FirstMotor` → `pMotor->Next`).
`SetPositionAllMotors()` (`c__motorgroep.c:403`) distributes the group demand:

```c
val_hr_alg.Motor[pMotor->Number].PositionAuto = val_hr_alg.Motorgroup[pGroup->Number].PositionPerc;
```

Gated by `if (pGroup->Delay == 0)` — a start-stagger mechanism that prevents every motor in a
group starting simultaneously. `Implemented`

Per-motor refinements include:

- **High/low speed switchover** at a configurable `PositionLowSpeed` threshold, with the
  direction of the comparison depending on group `Type` — windows and screens are handled as
  mirror images. `Implemented`
- **Hold open / hold close** flags (`SetMotorHold()` / `SetMotorRelease()`, ~line 709).
- **Per-motor manual override** (`Overruled`), which the feedback logic is aware of.

Capacity: `MAX_GROUP 32`, `MAX_MOTOR 64` (`common/ct_data.h:222-223`). `Implemented`
*(This conflicts with the migration target — see [`05-doc-vs-code-findings.md`](05-doc-vs-code-findings.md).)*

## F-04 — Simulate position where no encoder feedback exists

`ControlVirtualMotor()` (`c__motorgroep.c:641`) integrates elapsed running time into
`PositionTime` whenever the group is driven by digital inputs, then clamps it:

```c
if (PositionTime < 0)                     PositionTime = 0;
if (PositionTime > opt_app...Runtime)     PositionTime = opt_app...Runtime;
```

Position percentage is then `Calc_Perc_Abs(PositionTime, Runtime, 1000)`.

The increment is **direction- and type-dependent**, and this is where the window/screen
inversion physically lives:

| Group type | On OPEN | On CLOSE |
|---|---|---|
| `TYPE_RAAM` (window) | `PositionTime++` | `PositionTime--` (or `-= 4` at high speed) |
| `TYPE_DOEK` (screen) | `PositionTime--` (or `-= 4`) | `PositionTime++` (or `+= 4`) |
| `TYPE_VENT`, `TYPE_KLEP` | `PositionTime++` | `PositionTime--` |

`Implemented`

> The `4` is a hard-coded high-speed ratio: high speed is assumed to be exactly **four times**
> low speed. It is not configurable. On a new platform this should be an explicit, named,
> configurable parameter rather than a magic number — but note that changing it changes
> behaviour, so it must be a deliberate decision, not a refactoring side-effect.
> Recorded as a question in `08-open-questions.md`.

## F-05 — Aggregate and report group position feedback

`MotorgroupSetFeedback()` (`c__motorgroep.c:350`) mirrors the demand interface: CANopen
analog input, or BACnet `CURRENT_POSITION`, or an analog output.

Three behaviours deserve attention:

1. **Feedback can be suppressed.** If `AlarmAnaloog` is configured, feedback is only enabled
   when at least one motor in the group is in AUTO, not overruled, and the group has no hard
   alarm.
2. **The suppressed value is 500, i.e. 50 %.** Not 0, not hold-last-value. This is an
   out-of-band signal to the horticulture computer that the feedback is invalid.
   `Implemented` — and almost certainly a deliberate convention worth confirming with the
   vendor, because a new platform that "sensibly" substitutes 0 would silently break it.
3. **In digital+AUTO mode the reported value is the simulated one** (`PositionTime/Runtime`),
   whereas otherwise the passed-in measured `Position` is used.

**Conflict:** every call site passes `pGroup->PositionAvg` — the *average* of motor
positions. The meeting note states that for 0–10 V control the feedback should use the motor
with the **greatest deviation**, deliberately, so that a lagging motor is not masked by the
average. The code does not do this. See `05-doc-vs-code-findings.md` D-02.

## F-06 — Detect, classify and report alarms

Four documented severities, confirmed in code: **soft**, **several soft**, **hard**,
**several including hard**. Soft alarms can be suppressed; most cancel automatically when the
underlying condition resolves. `Confirmed`

The alarm relay is **de-energised on a hard alarm** — a fail-safe design, so that loss of
power or a broken wire also raises the alarm upstream. `Documented`

Hard alarms participate directly in control: `AlarmHardGroup(pGroup->Number + 1)` gates
feedback enablement. Alarm state is held per group and per motor
(`sAlarmHrMotorgroup Motorgroup[MAX_GROUP]`, `sAlarmHrMotor Motor[MAX_MOTOR]`).
`Implemented`

A separate position-deviation alarm exists for the case where a motor fails to track its
group.

## F-07 — Local operator control and override

Three operating modes per group and per motor — AUTO, HAND (manual), OFF — plus three running
modes — OPEN, CLOSE, STOP. In HAND the operator drives directly; in OFF the group is driven
to 0 %. `Confirmed`

Digital inputs allow hard-wired open/close/high-speed control
(`DigInOpen`, `DigInClose`, `DigInHiSpeed`), read via `IO_Get_Dig_In_Status()`. These are
typically local control-cabinet switches. `Implemented`

## F-08 — Coupled-screen gap control

`inputs/orion-mc/sources/app/c__kiersturing.c` ("kiersturing" = gap control) handles **two screens on one
section** that must not collide. It tracks `VirtualPositionA` / `VirtualPositionB`, computes

```c
MinimumGap = CurrentPositionA - VirtualPositionB;
```

and constrains each screen's desired position so the required gap is maintained — with
special cases for standby, fully-closed B (`VirtualPositionB == 0`) and fully-open A
(`VirtualPositionA == 1000`). `Implemented`

Documented by the `Multi Connect coupled screens` diagram. Out of phase-1 scope, but it is
the most intricate algorithm in the application layer and will need its own analysis slice
when screens are migrated.

## F-09 — Configure, calibrate and commission

Menu-driven installation procedure covering General, Motor groups, Group settings and Alarms
option groups, protected by an access code. The manual provides fill-in charts for recording
each installation's configuration. `Documented`

Calibration of per-motor running time is central: the virtual-motor simulation is only as
accurate as the calibrated `Runtime`.

## F-10 — Generic greenhouse IO

Beyond motor control the firmware supports differential-pressure and other sensors over
Modbus (`c__sensoren.c`), EC fans, air-handling units, heating control, and generic digital
and analog IO through CAN-IO-07-07 modules. `Implemented`

Entirely **out of phase-1 scope** — including the `c__sensoren.c` slice analysed in
`../orion-mc-re-001/04-module-design.md`.
