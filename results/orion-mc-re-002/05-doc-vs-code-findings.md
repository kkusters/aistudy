# 05 — Documentation vs Code: Confirmations and Discrepancies

The purpose of this file is to record where the official documentation and the source code
**agree**, where they **disagree**, and where the documentation **answered a question the
code could not**. Discrepancies are not resolved here — they are escalated.

---

## 5.1 Confirmations

Documentation claim verified independently in code.

| ID | Claim | Code evidence | Status |
|---|---|---|---|
| C-01 | Orion-MC does not compute setpoints; it receives them | `MotorgroupGetTargetPosition()` has no control law | `Confirmed` |
| C-02 | AUTO / HAND / OFF operating modes | `omAuto` / `omManual` / `omOff` | `Confirmed` |
| C-03 | OPEN / CLOSE / STOP running modes | `rmOpen` / `rmClose` / `rmStop` | `Confirmed` |
| C-04 | OFF drives the group to 0 % | `case omOff: PositionPerc = 0;` | `Confirmed` |
| C-05 | A group is presented upstream as one virtual motor | `ControlVirtualMotor()` | `Confirmed` |
| C-06 | Screens are the positional inverse of windows | `TYPE_DOEK` increments mirror `TYPE_RAAM` | `Confirmed` |
| C-07 | Four alarm severities: soft / several soft / hard / several incl. hard | alarm enums + `AlarmHardGroup()` | `Confirmed` |
| C-08 | Hoogendoorn is a supported horticulture computer | `opt_alg.hoogendoorn_enabled`, priority 3 | `Confirmed` |
| C-09 | H1MC = 1 motor, H2MC = 2 motors, over CAN Local | `component_nr` parameter, CANopen PDO/SDO | `Confirmed` |
| C-10 | Motor controllers read encoders and return position/status | `IO_H2MC_Motor_Control_PDO_Receive()` | `Confirmed` |
| C-11 | ΔP is the heater pressure difference reported by the ventilation unit | `c__sensoren.c` Modbus read | `Confirmed` |

C-01 is the most consequential: it retires the single largest open question from the first
analysis pass.

---

## 5.2 Discrepancies — must be resolved before porting

### D-01 — Motor capacity: 64 in code vs ~128 in the migration target

| Source | Value |
|---|---|
| `inputs/orion-mc/sources/common/ct_data.h:223` | `#define MAX_MOTOR 64` |
| Meeting note, 29 Sep 2026 | "~32 groups, ~128 motors" |

Groups agree (32 both ways). Motors do not — the target is **double** the current limit.

The meeting note hedges: *"exact limits are yet to be confirmed"*, and names CAN-Local
capacity as the practical constraint rather than firmware arrays. So three readings are
possible:

1. 128 is a new requirement for the new platform (capacity increase).
2. 128 is a misremembering of the current product.
3. 128 is achievable today across multiple CAN segments, and 64 is the per-something limit.

This is **not** a detail. Doubling `MAX_MOTOR` doubles `sOptMotor`, `sValHrMotor` and
`sAlarmHrMotor` arrays and changes the CAN-Local loading per segment. On the legacy XC161CJ
it would likely not fit; on a new platform it may be free. Either way it must be a *decided
requirement*, not an accident of a `#define`.

**Action:** ask the vendor directly whether 128 motors is a target capability or a
restatement of today's behaviour. Record the answer as a system requirement with a rationale.

**Severity: high.** Architecture-determining.

### D-02 — Position feedback rule: average vs greatest deviation

| Source | Rule |
|---|---|
| Meeting note | For 0–10 V control, feedback uses the motor with the **greatest deviation**, deliberately, so a lagging motor is not masked |
| User manual (screens) | *"the average of the measured positions of all screen motors in the group"* |
| Code — all call sites | `MotorgroupSetFeedback(pGroup, pGroup->PositionAvg)` — **average** |

The code and the manual agree on *average*. The meeting note describes *greatest deviation*.

Note the meeting note also says the rationale "has yet to be verified". So the most likely
explanation is that greatest-deviation is **desired behaviour for the new platform**, or a
behaviour that exists in a different product, and not what the Orion-MC does today.

This matters because the two rules differ exactly in the failure case. With eight motors, if
one jams at 0 % while seven reach 100 %, the average reports 87.5 % — plausible, and the
climate computer sees nothing wrong. Greatest-deviation would report 0 % and provoke a
reaction. That is a meaningful behavioural difference with horticultural consequences.

**Action:** confirm with the vendor whether this is a deliberate improvement for the new
platform. If yes, it becomes a *new requirement*, clearly flagged as a behaviour change —
not something to be slipped in as "what the old one did".

**Severity: high.** Changes observable system behaviour under fault.

### D-03 — Address range: 31 documented vs 247 in code

| Source | Limit |
|---|---|
| Manual | *"Maximum 31 addresses on one CAN IO-7-7 (RS485 Bus)"* |
| Code (SR-040, `c__sensoren.c`) | Modbus address range 1–247 |

Not a contradiction so much as two different limits: 247 is the Modbus protocol maximum, 31
is the electrically supported RS485 bus limit for this hardware.

**Action:** the *requirement* is 31. The code's 1–247 is a protocol-level validation range,
which is harmless but should be documented as such so a future reader does not infer a
247-device capability.

**Severity: low.** Documentation clarity.

### D-04 — Analog interface levels differ between documents — **RESOLVED 2026-10-01**

| Source | Levels |
|---|---|
| Multi Connect connection diagram | 0–5 V / 0–20 mA |
| Technical specification | 0–20 mA (Ri 250 Ω), 0–10 V (Ri 50 kΩ), potentiometer 5 kΩ |
| Meeting note | 0–10 V control |

Most likely all are true for different IO modules and installation variants. But a new
platform's analog front-end must support the full set, so the set must be known exactly.

**Resolution:** the product owner confirmed only **0–10 V** analog control is required for
the new platform, alongside digital open/close. 0–5 V, 0–20 mA and the 5 kΩ potentiometer
input are **not required** unless separately re-confirmed for a specific installation.
See `06-migration-scope.md` §6.1a and `08-open-questions.md` Q-07.

**Severity: medium → resolved.** Hardware-determining; now settled.

---

## 5.3 Defect candidates reconfirmed

### OI-010 — upgraded from "candidate" to "defect"

`MainSensoren()` in `c__sensoren.c` tests `TimerExpired(&Timer_100ms)` but never calls
`TimerReset()`. `TimerExpired()` (`c__timer.c:352`) is a pure query with no side effects, so
once 100 ms has elapsed the condition is permanently true and the body executes on **every**
superloop pass.

The first analysis flagged this as a candidate because the author's intent was unclear.
`c__motorgroep.c` now settles it: the same module author uses `TimerSet` (line 88) →
`TimerExpired` (1075) → `TimerReset` (1129) as a correctly closed triple, and additionally
nests a correctly-reset 1 s timer inside it. The paired idiom is clearly the intended pattern.

**OI-010 is a coding defect, not a design choice.** `Defect candidate` → **confirmed defect**.

It still needs an explicit **disposition** before porting — *Correct* or *Preserve* — because
sensor polling at superloop rate may have become load-bearing for downstream timing in ways
nobody intended. Do not silently fix it during the port.

> Note: this does not affect phase 1, since `c__sensoren.c` is out of scope. It is recorded
> so the decision is not lost.

---

## 5.4 Questions the documentation answered

| Former open question | Answer |
|---|---|
| Does the Orion-MC compute climate setpoints? | **No.** It is a subordinate position controller. |
| What is the `Hoogendoorn` flag in the code? | Integration with the Hoogendoorn horticulture computer, one of two named target systems alongside Priva. |
| Is this product safety-certified? | **No** — see `07-constraints-and-safety.md`. |
| What are the real deployment limits? | CAN Local 500 m / 100 kbps; CAN Backbone 500 m / 125 kbps; 31 addresses per RS485 bus. |
| Why are there six manual variants? | One codebase, configuration-selected option groups per licensed module. |
