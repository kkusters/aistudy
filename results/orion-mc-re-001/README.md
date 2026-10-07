# Orion MC — Reverse-Engineering Working Set

> **Note.** Product-level conclusions in this folder are superseded by
> [`../orion-mc-re-002/`](../orion-mc-re-002/), written after official vendor
> documentation became available. In particular, the slice analysed here
> (`c__sensoren.c`, Modbus sensors) is **out of phase-1 migration scope** — the active
> target is now window / motor-group control. Module-level work below remains valid.

First vertical slice: differential pressure sensor acquisition
(`inputs/orion-mc/sources/app/c__sensoren.c`), analyzed per
`../../v-model-templates/REVERSE-ENGINEERING-WORKFLOW.md`.

| Phase | Artifact | State |
|---|---|---|
| Product overview | `product-functions.md` | 9 main functions recovered from source |
| 0-1 Scope and baseline | `00-baseline.md` | Complete, hardware baseline TBD |
| 3-4 Inventory and slice | `00-baseline.md` | Complete |
| 5 Characterization | `validation-plan.md` | Planned, not executed |
| 6 Module design | `04-module-design.md` | Complete from source |
| 7 Architecture | `03-architecture-design.md` | Complete from source |
| 8 System requirements | `02-system-requirements.md` | Draft, review needed |
| 9 Business intent | `01-business-requirements.md` | Candidate, approval needed |
| 10 Validation | `validation-plan.md` | Draft |
| 11 Disposition | `open-issues.md` | 7 open items |
| 12 Baseline | `traceability-matrix.csv` | Draft |

## Headline finding

The 100 ms polling gate in `MainSensoren()` never resets its timer, so after
the first 100 ms the sensor loop executes on every superloop pass. See OI-010
and MD note N-010. This must be dispositioned before porting.

## Next actions

1. Decide the disposition of OI-010 with the product owner.
2. Execute IT-030 on a reference unit to measure the actual polling rate.
3. Resolve OI-020, OI-030, and OI-070 by analyzing `c__mb_device.c` and the
   sensor datasheet.
4. Extract the portable indexing logic behind a test seam and implement UT-010
   through UT-080.
