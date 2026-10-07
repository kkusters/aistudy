# 01 — Product Definition

## 1.1 Identity

| Field | Value | Evidence |
|---|---|---|
| Manufacturer | Hotraco Horti BV, Stationsstraat 142, 5963 AC Hegelsom, Netherlands | `Documented` |
| Product | Orion-MC ("Multi Connect") | `Documented` |
| Panel hardware | Orion-UPM-30 | `Documented` |
| IO modules | CAN-IO-07-07, H1MC, H2MC, H2MC-F | `Confirmed` |
| Manual version | 05/ENG, February 2017 | `Documented` |
| Processor | Infineon XC161CJ, Tasking C166 toolchain | `Implemented` |

## 1.2 What the product *is*

The decisive sentence, repeated across all six manual variants:

> *"The Orion-MC is the communication interface between a horticulture computer
> (the main controller) and a variety of systems."*

And:

> *"The horticulture computer determines the required action; the Orion-MC translates it
> into commands for the IO modules and returns the position feedback."*

`Documented`

**This corrects the earlier analysis.** `../orion-mc-re-001/product-functions.md` framed the product
as performing "climate and motor control" and left open the question of whether it computes
its own setpoints. **It does not.** The Orion-MC is a *subordinate* controller. It owns
*how* a position is reached — never *what* position is wanted.

The code confirms this unambiguously. `MotorgroupGetTargetPosition()`
(`inputs/orion-mc/sources/app/c__motorgroep.c:301`) has no setpoint computation anywhere in it: every branch
either reads a demand from an external interface or, in digital mode, integrates the
operator's own open/close commands. There is no PID, no climate model, no sensor-driven
control law in the motor path. `Confirmed`

### The boundary, stated precisely

| Owned by the horticulture computer | Owned by the Orion-MC |
|---|---|
| Desired position per group | Which motors are in the group |
| When to open/close, and why | Motor runtimes and calibration |
| Climate strategy, weather reaction | High/low-speed switchover points |
| Supervisory alarming | Start delays, interlocks, gap control |
| | Per-motor fault detection |
| | Aggregating group position feedback |
| | Local AUTO / HAND / OFF override |

## 1.3 The core abstraction: a group is one virtual motor

This is the product's reason to exist, and the thing a new platform must reproduce exactly.

> *"The Orion-MC presents itself to the climate computer as one virtual motor per group."*
> — meeting note, `Documented`

A greenhouse section may have eight window motors on one roof side. The climate computer
does not want to know that. It wants to command *"this section to 40 % open"* and read back
*"this section is at 37 %"*. The Orion-MC provides exactly that illusion, and absorbs all
the resulting complexity: motors that run at different speeds, motors that must be staggered
to limit inrush current, motors that have failed, screens that must not collide.

The implementing function is literally named for it:

```c
static void ControlVirtualMotor(TMotorgroup *pGroup)   // c__motorgroep.c:641
```

`Confirmed`

## 1.4 Application variants

Six documented variants share one codebase, differentiated by configuration and licensed
option groups:

| Variant | Controls | Upstream interface |
|---|---|---|
| Ventilation | Window motors | analog / digital / CANopen |
| Ventilation Groups | Window motors, grouped | analog / digital / CANopen |
| Ventilation Flaps | Flaps (`TYPE_KLEP`) | analog / digital |
| Aeration & Screens — BACnet | Windows + screens (`TYPE_DOEK`) | BACnet/IP |
| Aeration & Screens — CANopen | Windows + screens | CANopen backbone |
| Air handling | Air-handling units, EC fans | analog / Modbus |

In code the variants map onto the group `Type` enum — `TYPE_RAAM` (window), `TYPE_DOEK`
(screen/cloth), `TYPE_VENT` (ventilation unit), `TYPE_KLEP` (flap) — plus the
`opt_alg` feature flags `can_backbone`, `BACnet_enabled`, `hoogendoorn_enabled`.
`Confirmed`

> The manual is explicit that the menu adapts: *"solely the option groups of the available
> modules are displayed."* This is a licensing/configuration mechanism, not separate firmware.

## 1.5 Physical and electrical specification

### Orion-UPM-30 panel

| Parameter | Value |
|---|---|
| Supply | 100–240 VAC |
| Ambient temperature | 0–40 °C |
| Protection | IP54 |
| Dimensions | 220 × 165 × 105 mm |
| Mass | ≈ 1 kg |

### CAN-IO-07-07 module

| Parameter | Value |
|---|---|
| Supply | 230 VAC ± 10 % |
| Power | max 15 VA |

### Communication limits

| Link | Max length | Rate |
|---|---|---|
| CAN Local | 500 m | 100 kbps |
| CAN Backbone | 500 m | 125 kbps |
| RS232 | 15 m | 38 400 Bd |
| USB | 5 m | 115 kbps |
| Ethernet | 100 m | 10 Mbps |

### Analog interface ratings

| Signal | Characteristic |
|---|---|
| 0–20 mA | Ri = 250 Ω |
| 0–10 V | Ri = 50 kΩ |
| Potentiometer | 5 kΩ |

All `Documented`.

> **Porting note.** These are *product* constraints, not platform constraints. A new platform
> must still meet the 0–40 °C range and IP54 enclosure, and must still drive and read the
> same field signal levels, because the installed base of field wiring and motors will not
> change. The CAN bit rates, by contrast, are an *interface compatibility* constraint: they
> must be preserved exactly as long as existing H1MC/H2MC modules remain in the field.
