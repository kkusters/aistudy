# 00 — Evidence Sources & Method

## Evidence status legend

Used throughout this folder. Taken from the reverse-engineering workflow.

| Status | Meaning |
|--------|---------|
| `Documented` | Stated in official vendor documentation (manual, diagram, meeting note) |
| `Implemented` | Directly readable in source code; file and line cited |
| `Confirmed` | Stated in documentation **and** independently verified in source code |
| `Observed` | Seen in a running system — **none in this folder; no hardware was run** |
| `Inferred` | Reasoned conclusion from the above; not directly stated or executed |
| `Conflict` | Documentation and code disagree; must be resolved before porting |
| `Defect candidate` | Behaviour that appears unintended; requires disposition |
| `TBD` | Unknown; question recorded in `08-open-questions.md` |

> Rule carried over from the workflow: **nothing here may be promoted into an approved
> requirement without an explicit decision.** Implementation facts and legacy defects are
> not requirements by default.

---

## Source material

### A. Official documentation — `inputs/orion-mc-docs/`

20 PDFs plus one `.docx`. Text extracted with `pdftotext -layout` into `/tmp/ormc_txt/`
(~247 000 words). The `MAN_` variants were preferred over `MAN-E_` variants: same content,
but `MAN_` carries proper Unicode (Ω, →) while `MAN-E_` mangles it.

**User manuals** — version `05/ENG/February 2017`, six application variants:

| Variant | File stem |
|---|---|
| Ventilation | `ORION-MC_MAN_V05.00_ENG_Ventilation_HH90042102` |
| Ventilation Groups | `..._Ventilation groups_...` |
| Ventilation Flaps | `..._Ventilation flaps_...` |
| Aeration & Screens (BACnet) | `..._Aeration screens BACnet_...` |
| Aeration & Screens (CANopen) | `..._Aeration screens CANopen_...` |
| Air handling | `..._Air handling_...` |

**Connection diagrams** — topology and field wiring, including
`ORION-MC_DIAGRAM_ENG_Multi Connect_coupled screens`, EC-fan variants, and IO-module wiring.

**Meeting note** — `260929.info window control from twan.docx`, 29 September 2026,
participants Twan / Alexandra / Joris / Kuno. **This is the single most important document
for the migration**: it defines phase-1 scope and explains the virtual-motor concept in the
vendor's own words.

### B. Source code — `inputs/orion-mc/sources/`

Infineon XC161CJ, Tasking C166 toolchain. ~260 `.c` and ~309 `.h` files, regrouped in the
previous session into 16 subfolders. Project file `Orion_MC_V4.pjt`.

**This code has not been compiled.** The Tasking C166 toolchain is Windows-only and is not
available in this environment. All code statements are from static reading.

### C. Not available

- No running hardware, no logs, no oscilloscope traces, no CAN captures
- No formal requirements specification from the vendor
- No test suite or test reports
- No change history with rationale (the git history is a single import)

---

## Method

1. Extracted and read all documentation text.
2. Re-read the main loop, application modules, configuration structures and IO layer.
3. For every significant claim in the documentation, searched the code for the mechanism
   that implements it — and recorded whether it **confirmed**, **refined**, or **conflicted**.
4. Recorded conflicts rather than silently choosing a side.

The direction matters: documentation was used to *ask questions of the code*, not to replace
reading it. Where the manual describes behaviour the code does not appear to implement, that
is reported as a conflict in `05-doc-vs-code-findings.md`, not quietly dropped.
