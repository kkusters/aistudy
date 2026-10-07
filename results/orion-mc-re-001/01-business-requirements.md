# Business Requirements (Candidate): Differential Pressure Monitoring

**Evidence status:** `Inferred`. None of these is approved. Each requires
confirmation by the product owner.

| ID | Candidate requirement | Rationale inferred from code | Confirmation needed |
|---|---|---|---|
| BR-010 | The controller shall monitor differential pressure at multiple points in the air-handling installation. | Dedicated configurable multi-sensor acquisition feature | Which installation functions depend on these values? |
| BR-020 | The installer shall be able to configure how many differential pressure sensors are present and where they are addressed. | Configurable count, first address, and bus mapping | Is 16 a market requirement or a historical limit? |
| BR-030 | The operator shall be notified when a configured differential pressure sensor fails to communicate. | Per-sensor alarm registration | What operator action is expected? |
| BR-040 | Differential pressure values shall be presented to the operator on the controller display. | Values bound to display screen `c__disp_F3_0.c` | Are values also required over BACnet or CANopen? |
| BR-050 | Differential pressure monitoring behavior shall remain equivalent after migration to the new platform. | Migration objective | Which deviations are acceptable improvements? |

## Decisions required from the product owner

1. Is the intended acquisition rate 100 ms, or is the current continuous
   polling behavior required by the installation? See OI-010.
2. Is differential pressure used for control, or for indication and alarms
   only? This determines the safety and timing rigor of SR-020.
3. Should analog differential pressure sensors be supported on the new
   platform? See OI-020.
4. What is the required behavior when a sensor is lost: hold last value,
   invalidate, or force a safe state? See OI-030.
