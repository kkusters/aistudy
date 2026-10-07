# 07 — Constraints and Safety

## 7.1 The safety position, stated by the vendor

> *"The Orion-MC is under no circumstances a safety component."*

`Documented` — repeated across the manual variants.

This is an unusually direct disclaimer, and it has concrete consequences for the migration:

- There is **no SIL rating** (IEC 61508) and **no Performance Level** (ISO 13849).
- There is no evidence of a safety case, hazard analysis, or safety-related software
  development process behind the existing firmware.
- The new platform **inherits this position by default**. If the intention is to change it —
  for example because a new market requires it — that is a major scope change that affects
  toolchain qualification, development process, architecture and documentation, and it must
  be decided explicitly and early.

The honest reading: this product is a *functional* controller whose failure modes are
mitigated by **external** means, and the manuals say so.

## 7.2 The external mitigations the vendor relies on

The manuals are specific about what the installation must provide around the Orion-MC:

| Mitigation | Purpose |
|---|---|
| Oxygen content must be guaranteed | Crop and personnel survival if ventilation fails |
| Emergency generator recommended | Ventilation must survive mains loss |
| Emergency opening facility | Windows must open without the controller |
| Manual operation facility | Operators must be able to drive motors directly |
| Installation inspected **at least daily** | Human detection of undetected faults |

`Documented`

These are **requirements on the system of which the Orion-MC is a part**, not on the
Orion-MC itself. They belong in the business-requirements layer of the V-Model
(`01-business-requirements`) as context and assumptions, because they are the reason the
product is allowed to not be a safety component.

> If a future platform removes the need for any of these — for example by adding redundant
> position monitoring — that is a *product strategy* change, not a software improvement.

## 7.3 Fail-safe design features present in the product

Despite not being a safety component, the design does contain deliberate fail-safe choices:

| Feature | Behaviour | Evidence |
|---|---|---|
| Alarm contact | **De-energised** on hard alarm | `Documented` |
| Invalid feedback | Analog output forced to **50 %** rather than held or zeroed | `Implemented`, `c__motorgroep.c:396` |
| OFF mode | Actively drives the group to 0 % rather than freeing it | `Confirmed` |
| Position clamping | `PositionTime` clamped to `[0, Runtime]` every tick | `Implemented` |
| Group disable | Alarms reset exactly once on the enable→disable edge | `Implemented` |

The de-energised alarm contact is the important one: it means a power failure, a blown fuse
or a cut wire all present as "alarm" to the horticulture computer. **This property must be
preserved bit-for-bit in the new platform.** It is the kind of thing a well-intentioned
rewrite inverts by accident, because an energised-on-alarm relay feels more natural to code.

## 7.4 Environmental and electrical constraints

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

### Communication

| Link | Max length | Rate | In scope for new platform? |
|---|---|---|---|
| CAN Local | 500 m | 100 kbps | **Yes — retained** (H1MC/H2MC motor modules) |
| CAN Backbone | 500 m | 125 kbps | **No — removed 2026-10-01** (was the upstream CANopen link to the horticulture computer) |
| RS232 | 15 m | 38 400 Bd | TBD |
| USB | 5 m | 115 kbps | TBD |
| Ethernet | 100 m | 10 Mbps | **No — removed 2026-10-01** (was required for BACnet/IP, now out of scope) |
| RS485 (CAN IO-7-7) | — | max 31 addresses | Out of scope — belongs to the Modbus sensor subsystem (F-10), not phase 1 |

### Analog

| Signal | Characteristic | In scope for new platform? |
|---|---|---|
| 0–20 mA | Ri = 250 Ω | **No — removed 2026-10-01** |
| 0–10 V | Ri = 50 kΩ | **Yes — the only analog signal required** |
| Potentiometer | 5 kΩ | **No — removed 2026-10-01** |

All `Documented`. Scope column reflects the product-owner decision of 2026-10-01 — see
`06-migration-scope.md` §6.1a.

## 7.5 Constraints that bind the new platform

| # | Constraint | Type | Negotiable? |
|---|---|---|---|
| 1 | 0–40 °C ambient, IP54 | Environmental | No — same greenhouses |
| 2 | CAN Local 100 kbps, 500 m (H1MC/H2MC) | Interface | No — existing field modules |
| 3 | ~~CAN Backbone 125 kbps, 500 m~~ | Interface | **Removed 2026-10-01** — no longer a constraint; CANopen-backbone and BACnet/Ethernet upstream paths are out of scope |
| 4 | Analog signal level: **0–10 V only** | Interface | No — existing field wiring, reduced to the confirmed range |
| 5 | Alarm contact de-energised on alarm | Behavioural | No — fail-safe property |
| 6 | Not a safety component | Process | Only by explicit decision |
| 7 | 100 ms control period | Timing | Effectively no — position unit depends on it |
| 8 | Panel dimensions 220 × 165 × 105 mm | Mechanical | Possibly — depends on retrofit strategy |
| 9 | RS232 / USB / Ethernet service ports | Interface | Partially |
| 10 | LCD + keypad operator interface | HMI | Yes — candidate for redesign |

Rows 1–7 should be treated as **frozen constraints** and recorded in the architecture layer
of the V-Model. Rows 8–10 are genuine design freedom.

## 7.6 A note on the 100 ms period

It is tempting to treat the control period as an implementation detail. It is not. In this
design the simulated position unit is *defined* as one 100 ms tick (`Runtime` is documented
as `[x100ms]`), so the period is part of the data model, not just the schedule. Every stored
calibration value in every installed system is expressed in those units.

Changing the period on the new platform therefore requires either converting all stored
configuration, or decoupling the position integration from the task period by integrating
against measured elapsed time instead of tick count. The second is cleaner and is the
recommended approach — but it changes rounding behaviour at the edges, so it needs a test
against the legacy implementation as oracle.
