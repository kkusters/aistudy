# 08 — Open Questions

> ## Scope decision log
>
> **2026-10-01** — product owner decided the upstream interface (to the horticulture
> computer) only needs **digital open/close** and **0–10 V analog** control. **CAN
> Backbone/CANopen, BACnet and the Hoogendoorn protocol are removed from scope.**
> **CAN Local to H1MC/H2MC is explicitly retained** (confirmed separately — it is the
> downstream link to motor field modules, a different CAN segment from the removed
> backbone). This resolves Q-07 and narrows Q-02/Q-10/Q-11 as noted inline below. See
> `06-migration-scope.md` §6.1a for the full reduced-scope interface list.

Questions that must be answered before the corresponding requirements can be `Approved`.
Grouped by who can answer them.

Priority: **P1** blocks phase-1 architecture · **P2** blocks phase-1 implementation ·
**P3** needed before phase 2.

---

## 8.1 For the vendor / product owner

| ID | Pri | Question | Why it matters |
|---|---|---|---|
| Q-01 | **P1** | Is ~128 motors a new capacity *requirement*, or a restatement of today's product? Code says `MAX_MOTOR 64`. | Determines memory budget, CAN segment count, possibly whether multi-segment support is architectural. See D-01. |
| Q-02 | **P1** | Should position feedback use the **average** (as code and manual say) or the **motor with greatest deviation** (as the meeting note says)? | Changes observable behaviour under motor fault. See D-02. |
| Q-03 | **P1** | Does the new platform remain "under no circumstances a safety component"? | Changing this changes the entire development process, toolchain qualification and architecture. |
| Q-04 | **P2** | Is the 50 % analog output on invalid feedback a deliberate out-of-band signal that horticulture computers detect? | If yes it is a frozen interface contract; if no it is free to change. |
| Q-05 | **P2** | Is the hard-coded high-speed ratio of 4× correct for all supported motors, or should it be configurable? | `ControlVirtualMotor()` assumes exactly 4×. |
| Q-06 | **P2** | Is feedback intentionally computed from a position that is up to 1 s stale? | Affects whether the port may recompute fresh. |
| Q-07 | **RESOLVED (2026-10-01)** | ~~Which analog ranges must the new hardware support: 0–5 V, 0–10 V, 0–20 mA, 5 kΩ pot?~~ Product owner confirmed **only 0–10 V** is required. | Hardware-determining. See D-04 — now resolved. |
| Q-08 | **P3** | Is position deviation deliberately fed back so the horticulture computer can raise its own alarm? Meeting note says this "has yet to be verified". | Determines whether deviation is an interface feature or an internal matter. |
| Q-09 | **P3** | Will existing installations be migrated in place, or is this new-build only? | Determines whether stored configuration must be converted. |

## 8.2 Requiring hardware or a bus capture

| ID | Pri | Question | How to answer |
|---|---|---|---|
| Q-10 | **P1** | What exactly is on the H1MC/H2MC CANopen interface — object dictionary, PDO mapping, SDO usage, timing? **Still applies** — this is CAN Local (downstream to motor modules), explicitly retained on 2026-10-01, and is unaffected by the CANopen-backbone/BACnet removal. | Capture a live CAN-Local bus; cross-read `c__IO_H1MC_mc.c` / `c__IO_H2MC_mc.c`. **This is the highest-value missing artifact.** |
| Q-11 | **P1** | What is the actual CAN-Local bus load at 100 kbps with N motors at a 100 ms period? | Compute from the ICD once Q-10 is answered; then measure. |
| Q-12 | **P2** | Does the regrouped source tree still build under Tasking C166? | Requires the Windows IDE. Not verifiable in this environment. |
| Q-13 | **P2** | What is the real superloop period, and how much does it vary? | Instrument the legacy system. Relevant to OI-010's actual impact. |
| Q-14 | **P3** | What is the observed behaviour when a motor jams mid-travel? | Field test; needed to validate alarm requirements. |

## 8.3 Requiring further code analysis

| ID | Pri | Question | Where to look |
|---|---|---|---|
| Q-15 | **P1** | What is the per-motor state machine? | `app/c__motor.c` — not yet analysed |
| Q-16 | **P2** | How is the start-stagger `pGroup->Delay` computed and what are its limits? | `c__motorgroep.c`, `SetPositionAllMotors()` |
| Q-17 | **P2** | How are runtime calibration values acquired, stored and validated? | `GetRuntimeMotorgroup()`, option menus, EEPROM layer |
| Q-18 | **P2** | Full enumeration of alarm codes, their severities and their triggers | `ControlAlarmMotorgroup()` + alarm tables |
| Q-19 | **P3** | How does gap control constrain the motor-group abstraction? | `app/c__kiersturing.c` |
| Q-20 | **P3** | What does `ControlHoldPosition()` do and why is it BACnet-only? | `c__motorgroep.c:1040` |

## 8.4 Carried forward from the first analysis pass

| ID | Status now | Note |
|---|---|---|
| OI-010 | **Confirmed defect** — needs disposition | `c__motorgroep.c` proves the correct paired-timer idiom was the author's intent. Still requires an explicit *Correct* or *Preserve* decision. Out of phase-1 scope. |
| OI-020 | Open | Unused `Type` field in the sensor configuration. Out of phase-1 scope. |
| OI-030 | Open | Stale sensor value retained on device disconnect. Out of phase-1 scope. |
| OI-050 | Open | Alarm re-registration idempotency. **Still relevant** — the alarm subsystem is in phase-1 scope. |
| OI-070 | Open | See `../orion-mc-re-001/open-issues.md`. |

## 8.5 Environment and housekeeping

| ID | Note |
|---|---|
| E-01 | The regrouped `inputs/orion-mc/sources/` tree and the updated `Orion_MC_V4.pjt` are **not committed**. Revert with `git checkout -- . && git clean -fd inputs/orion-mc/sources`. |
| E-02 | 573 pre-existing modified files are pure line-ending churn — not caused by the regrouping. |
| E-03 | `start.asm` is referenced by the `.pjt` but absent from disk and from HEAD. Pre-existing. |
| E-04 | 32 unresolved includes exist both before and after regrouping — no regression, but they should be explained. |
| E-05 | `/tmp/ormc_txt/` is ephemeral. Regenerate with `pdftotext -layout` over `inputs/orion-mc-docs/` if needed. |

---

## 8.6 The three that matter most

If only three questions get answered, make them these:

1. **Q-10** — the H1MC/H2MC interface. It is completely frozen and completely undocumented.
   Everything downstream depends on it, and it cannot be recovered from the new platform
   later.
2. **Q-01** — 64 vs 128 motors. It is architecture-determining and cheap to answer.
3. **Q-02** — average vs greatest-deviation feedback. It is the difference between
   reproducing the product and silently changing how it behaves when a motor fails.
