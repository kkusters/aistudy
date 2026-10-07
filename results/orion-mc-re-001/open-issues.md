# Open Issues, Defect Candidates and TBDs

| ID | Type | Description | Evidence | Disposition | Owner |
|---|---|---|---|---|---|
| OI-010 | Defect candidate | The 100 ms gate never resets, so sensor polling runs on every superloop pass instead of every 100 ms. This increases RS485 bus load and CPU use unpredictably. | `c__sensoren.c` has no `TimerReset`; `c__timer.c:352` shows `TimerExpired` is a pure query; `c__motor.c` and `c__device.c` show the correct idiom | Pending: Correct or Preserve | TBD |
| OI-020 | TBD | `TOptSensoren.Type` (Modbus/analog) is never read by the module; only `dtDptMod` is used. | `ct_data.h:945`, `c__sensoren.c` | Investigate | TBD |
| OI-030 | TBD | On disconnect, the previous value silently remains in `val_hr_alg.Sensoren.Drukverschil[i]` with no explicit invalidation. | `c__sensoren.c` else-branch | Investigate | TBD |
| OI-040 | Robustness | Loop bound uses configuration only; no defensive `MAX_SENSOREN` clamp protects the 16-entry output array. UI clamping is the only barrier. | `c__sensoren.c`, `c__disp_opt_sensoren_2.c:57` | Improve on new platform | TBD |
| OI-050 | TBD | `mbDeviceCreateAlarm()` is re-invoked every pass for every sensor; idempotency assumed but unverified. | `c__sensoren.c` | Investigate | TBD |
| OI-060 | Evidence gap | No hardware measurement exists; all findings are static-analysis only. | `00-baseline.md` | Measure on reference unit | TBD |
| OI-070 | Evidence gap | Unit, scaling, and valid range of the sensor value are unknown. | `mbDeviceGetActualValue` returns raw `int` | Obtain datasheet | TBD |

## Rule

No item above may be ported to the new platform as an approved requirement
until its disposition is recorded and reviewed.
