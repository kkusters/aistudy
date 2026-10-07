# aistudy

V-Model process definition for embedded software development, plus an ongoing
reverse-engineering study of the Hotraco Horti **Orion-MC** controller.

The work has two phases:

1. **Reverse engineer** the existing Orion-MC codebase upward from code to
   requirements.
2. **Re-implement** the same product on a new platform, driven by those
   requirements rather than by the legacy source.

## Layout

| Folder | Contents |
|--------|----------|
| `v-model-templates/` | The process itself: blank templates for all nine V-Model layers, the reverse-engineering workflow, and a worked example. Start here. |
| `inputs/orion-mc/` | Legacy Orion-MC firmware source (Tasking C166, XC161CJ). Imported snapshot — the original Bitbucket history is not part of this repo. |
| `inputs/orion-mc-docs/` | Vendor manuals, training material and meeting notes used as evidence. |
| `results/orion-mc-re-002/` | **Current** analysis of the Orion-MC product — evidence-based, with the 2026-10-01 scope decision applied throughout. |
| `results/orion-mc-re-001/` | First reverse-engineering pass. Product-level conclusions superseded by `-002`; its module-level work on `c__sensoren.c` remains valid. |

`inputs/` is read-only evidence. `results/` holds analysis output, one numbered
folder per reverse-engineering run, so successive passes can be compared rather
than overwritten.

## Where to start

- **New to the process?** `v-model-templates/README.md`, then
  `v-model-templates/REVERSE-ENGINEERING-WORKFLOW.md`.
- **Want the product picture?** `results/orion-mc-re-002/README.md`.
- **Want the active work item?** `results/orion-mc-re-002/04-window-control-slice.md`
  — the window-control slice is the phase-1 target.

## Ground rules

Two conventions make the rest of the repo readable:

**Never promote implementation facts or legacy defects into approved
requirements.** What the code does is evidence, not intent. Every statement in
the analysis documents therefore carries an evidence status: `Implemented`,
`Observed`, `Inferred`, `Documented`, `Confirmed`, `Conflict`, `Approved`,
`Defect candidate` or `TBD`. Only `Approved` items are requirements.

**The scope decision of 2026-10-01 is authoritative.** The upstream interface
to the horticulture computer is reduced to digital open/close and 0–10 V analog
only; CAN Backbone (CANopen), BACnet and the Hoogendoorn protocol are out of
scope. CAN *Local* to the H1MC/H2MC motor modules is **retained** — it is a
different bus from the removed CAN Backbone, despite both being CANopen. See
`results/orion-mc-re-002/06-migration-scope.md` §6.1a for the full record.
