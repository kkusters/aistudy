# Embedded Software V-Model Templates

Templates for each layer of the SDLC V-Model, adapted for embedded software in
products. Based on: https://www.geeksforgeeks.org/software-engineering/software-engineering-sdlc-v-model/

## Why this exists

Two use cases, same artifacts:

1. **Reverse engineering (now):** Start from existing code and climb the left
   side of the V — write Unit/Module design, then Architecture, then System
   Design, then Business Requirements — by observing what the code *actually
   does*. Every template has a "Reverse-Engineering Notes" section for this.
2. **Forward engineering (later):** Once requirements exist, re-descend the V
   for a new platform/port, using the same documents as the authoritative
   spec, and regenerate code + tests from them.

## Worked example

See `example-temperature-monitor/` for an end-to-end example containing
requirements, architecture and module design, portable embedded C code,
executable unit tests, validation plans, and a completed traceability matrix.

## Reverse-engineering workflow

Follow `REVERSE-ENGINEERING-WORKFLOW.md` for the stage-gated process from
baseline selection and characterization through requirements approval. It
defines required inputs, evidence statuses, review gates, traceability checks,
and the completion criteria for each feature.

## Layer map (V shape)

```
Verification (left, top→down)          Validation (right, bottom→up)
--------------------------------------  --------------------------------------
01 Business Requirements  <----------->  09 Acceptance Test (UAT)
02 System Design          <----------->  08 System Test
03 Architecture Design    <----------->  07 Integration Test
04 Module Design (LLD)    <----------->  06 Unit Test
05 Coding  ------------------------------------->  (bottom of the V)
```

## Templates

All templates live in `templates/`, one per V-Model layer, numbered so they sort
in V order:

| Layer | Template | Produces IDs |
|-------|----------|--------------|
| 01 Business Requirements | `templates/01-business-requirements.template.md` | BR |
| 02 System Design | `templates/02-system-requirements.template.md` | SR |
| 03 Architecture Design | `templates/03-architecture-design.template.md` | AR |
| 04 Module Design (LLD) | `templates/04-module-design.template.md` | MD |
| 05 Coding | `templates/05-coding-standard.template.md` | — |
| 06 Unit Test | `templates/06-unit-test-plan.template.md` | UT |
| 07 Integration Test | `templates/07-integration-test-plan.template.md` | IT |
| 08 System Test | `templates/08-system-test-plan.template.md` | ST |
| 09 Acceptance Test | `templates/09-acceptance-test-plan.template.md` | AT |

Copy a template out of `templates/` into your project, rename it (drop
`.template`), fill it in, and keep the ID scheme so items cross-reference
cleanly in `_traceability/traceability-matrix.csv`.

Templates 04 and 06 are per-module: expect several filled-in copies of each,
one per module in the system.

## ID scheme (use consistently across all documents)

| Level                  | Prefix | Example   |
|------------------------|--------|-----------|
| Business Requirement   | BR     | BR-010    |
| System Requirement     | SR     | SR-010    |
| Architecture Decision/Interface | AR | AR-010 |
| Module/LLD item        | MD     | MD-010    |
| Unit Test              | UT     | UT-010    |
| Integration Test       | IT     | IT-010    |
| System Test            | ST     | ST-010    |
| Acceptance Test        | AT     | AT-010    |

Every downstream ID must trace to at least one upstream ID (e.g.
`MD-030` implements `AR-010` implements `SR-005` implements `BR-002`).
Fill `_traceability/traceability-matrix.csv` as you go — it's the artifact
that proves V-Model "cross-referencing" and is what you'll diff against when
porting to the new platform.

## Recommended reverse-engineering order

1. **05 Coding** — inventory: list modules/files, build system, toolchain,
   coding standards actually followed (even if informal).
2. **04 Module Design (LLD)** — one file per module/component, derived by
   reading the code: inputs, outputs, internal state, ISRs, timing.
3. **06 Unit Test** — write/record what unit tests exist or should exist for
   each module (backfill if none exist).
4. **03 Architecture Design** — derive block diagram, task/thread map,
   communication buses, memory map, interrupt map from the modules.
5. **07 Integration Test** — how modules are verified together (e.g. on
   target hardware, HIL rig).
6. **02 System Design** — derive external behavior, system states, timing
   budgets, peripherals used, power modes.
7. **08 System Test** — end-to-end test scenarios against system design.
8. **01 Business Requirements** — infer the product-level "why": use cases,
   constraints, certifications, target market, cost/BOM constraints.
9. **09 Acceptance Test (UAT)** — customer-facing acceptance criteria implied
   by the business requirements.

Then, for the **new platform**, walk 01 → 09 top-to-bottom in the normal
forward V-Model direction, reusing 01–03 mostly unchanged (platform-agnostic
requirements/architecture) and rewriting 04–06 (platform-specific
implementation).
