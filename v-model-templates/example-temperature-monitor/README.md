# Worked Example: Overtemperature Monitor

This example shows how an existing embedded C function can be documented
bottom-up and then used as the specification for a platform port.

## Product behavior

A controller samples a temperature sensor every 100 ms. It activates an alarm
after three consecutive samples at or above 80.0 C and clears the alarm after
five consecutive samples at or below 75.0 C.

## Files and V-Model links

| V-Model layer | Example artifact |
|---|---|
| Business requirements | `01-business-requirements.md` |
| System design | `02-system-requirements.md` |
| Architecture design | `03-architecture-design.md` |
| Module design (LLD) | `04-module-design.md` |
| Coding | `05-coding/temp_monitor.c` and `.h` |
| Unit validation | `06-unit-test/test_temp_monitor.c` |
| Integration validation | `07-integration-test.md` |
| System validation | `08-system-test.md` |
| Acceptance validation | `09-acceptance-test.md` |
| Cross-references | `traceability-matrix.csv` |

## Reverse-engineering walkthrough

1. Read `temp_monitor.c` and identify its inputs, outputs, thresholds, counters,
   state, and callback.
2. Record those facts in `04-module-design.md` without guessing product intent.
3. Capture observable module behavior as unit tests.
4. Identify the scheduler, sensor adapter, alarm output, and their interfaces in
   `03-architecture-design.md`.
5. Generalize implementation facts into externally measurable system
   requirements in `02-system-requirements.md`.
6. Ask why those behaviors exist and record the product intent in
   `01-business-requirements.md`. Mark inferred statements until a stakeholder
   approves them.
7. Define validation on the right side of the V and link every item in
   `traceability-matrix.csv`.

## Run the unit tests

From this directory:

```sh
make test
```

## Porting to a new platform

Keep `temp_monitor.c` unchanged. Replace the platform scheduler, temperature
sensor adapter, and alarm GPIO implementation described by AR-010 and AR-030.
Then rerun unit tests unchanged and execute IT-010 through AT-020 on the new
target.
