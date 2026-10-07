# Orion-MC — Combined Analysis (Code + Official Documentation)

Second-pass reverse-engineering analysis of the **Hotraco Horti Orion-MC**, produced after
official product documentation became available in `inputs/orion-mc-docs/`.

This folder **supersedes the product-level conclusions** in `../orion-mc-re-001/`.
The earlier folder remains valid for its module-level work (`c__sensoren.c` LLD, baseline,
open issues), but its framing of the product as a *climate controller* is **refuted** here.

---

## Documents in this folder

| # | File | Contents |
|---|------|----------|
| 00 | [`00-evidence-sources.md`](00-evidence-sources.md) | What was analysed, how, and the evidence-status legend |
| 01 | [`01-product-definition.md`](01-product-definition.md) | What the Orion-MC actually is; identity, hardware, variants |
| 02 | [`02-system-context.md`](02-system-context.md) | External interfaces, topology, setpoint-source chain |
| 03 | [`03-product-functions.md`](03-product-functions.md) | Revised function list F-01…F-10 with doc + code evidence |
| 04 | [`04-window-control-slice.md`](04-window-control-slice.md) | Deep-dive on the phase-1 migration target |
| 05 | [`05-doc-vs-code-findings.md`](05-doc-vs-code-findings.md) | Confirmations and discrepancies |
| 06 | [`06-migration-scope.md`](06-migration-scope.md) | Phase-1 scope, out-of-scope, porting implications |
| 07 | [`07-constraints-and-safety.md`](07-constraints-and-safety.md) | Safety position, environmental and electrical limits |
| 08 | [`08-open-questions.md`](08-open-questions.md) | Carried-forward and newly raised questions |

---

## The one-paragraph summary

The Orion-MC is **not** a climate controller. It is a **communication interface and
subordinate position controller**. A horticulture computer (Priva, Hoogendoorn, or any
system speaking CANopen / BACnet / analog / digital open-close) decides *what position*
greenhouse windows, screens, vents and flaps should be at. The Orion-MC translates that
single group-level demand into commands for the individual motors in that group — via
CAN-Local to H1MC/H2MC motor-controller modules or CAN-IO-07-07 modules — handles
calibration, runtime, high/low speed, delays and interlocks, monitors each motor for
faults, and reports an aggregated group position and alarm state back upstream.

To the horticulture computer, an entire motor group looks like **one single motor**.
That abstraction is the product's core value, and it is the thing that must be preserved
on the new platform.

---

## Scope decision log

| Date | Decision | Where recorded |
|---|---|---|
| 2026-10-01 | Upstream interface reduced to **digital open/close** and **0–10 V analog** only. CANopen backbone, BACnet and the Hoogendoorn protocol removed from scope. **CAN Local to H1MC/H2MC explicitly retained** (separate, downstream link). | `02-system-context.md` §2.1/2.2/2.5, `03-product-functions.md` F-01, `06-migration-scope.md` §6.1a, `08-open-questions.md` scope decision log |

---

## Status

Nothing in this folder is `Approved`. All statements are static analysis of source code
plus reading of official manuals. **No hardware was run, no measurement was taken.**
Promotion to `Approved` requires sign-off per
[`../../v-model-templates/REVERSE-ENGINEERING-WORKFLOW.md`](../../v-model-templates/REVERSE-ENGINEERING-WORKFLOW.md).
