# Module Design (LLD): `sensoren`

**Files:** `inputs/orion-mc/sources/app/c__sensoren.c`, `inputs/orion-mc/sources/app/ch_sensoren.h`
**Commit:** `d454c03`
**Evidence status:** `Implemented` (static analysis only)

## Purpose

Periodically acquire differential-pressure readings from up to 16 Modbus
RS485 sensors, register an alarm channel for each configured sensor, establish
the Modbus connection when absent, and publish the latest value for display
and application use.

## Public interface

| Function | Contract as implemented |
|---|---|
| `void InitSensoren(void)` | Sets the module timer to a 100 ms interval. Performs no other initialization. |
| `void MainSensoren(void)` | Must be called repeatedly from the superloop. When the timer reports expiry, iterates configured sensors and performs alarm registration, connection, or value read. |

## Configuration consumed

`opt_app.Sensoren` of type `TOptSensoren` (`ct_data.h:943`):

| Field | Type | Use in this module |
|---|---|---|
| `Drukverschil` | `unsigned char` | Number of configured sensors; loop bound |
| `Type` | `unsigned char` | Modbus/analog selector — **not read by this module** |
| `FirstAddress` | `unsigned char` | Modbus address of the first sensor |
| `RS485Bus[MAX_SENSOREN]` | `s_board_IO_on_off[]` | Bus/IO mapping passed to `mbDeviceConnect` |

`MAX_SENSOREN` is 16 (`ct_data.h:232`).

## Output produced

`val_hr_alg.Sensoren.Drukverschil[MAX_SENSOREN]`, type `int`, one entry per
sensor index. Consumed by display screen `c__disp_F3_0.c`.

## Internal state

| Item | Meaning |
|---|---|
| `static TTimer Timer_100ms` | Module-local timer; interval 100 ms, never reset after expiry |

## Module requirements

| ID | Requirement as implemented | Evidence |
|---|---|---|
| MD-010 | Initialization shall set the module timer to a 100 ms interval. | `InitSensoren()` |
| MD-020 | The module shall address sensor `i` as `FirstAddress + i` for `i` in `0 .. Drukverschil-1`. | `MainSensoren()` loop |
| MD-030 | The module shall process a sensor only when its address is greater than 0 and less than 248. | `DeviceAddressLegal()` |
| MD-040 | For each legal address, the module shall register a Modbus alarm with function code `DRUKVERSCHIL_MB_AL`, index `i`, and value `i + 1`. | `mbDeviceCreateAlarm()` call |
| MD-050 | If the device is not connected, the module shall request a connection of type `dtDptMod` using the configured RS485 bus table, and shall not read a value in that iteration. | `if (!mbDeviceConnected(...))` branch |
| MD-060 | If the device is connected, the module shall store its actual value in `val_hr_alg.Sensoren.Drukverschil[i]`. | `else` branch |

## Behavior notes and deviations

### N-010 Timer is never reset (defect candidate)

`TimerExpired()` is a pure query (`c__timer.c:352`); it does not restart the
timer. `MainSensoren()` never calls `TimerReset()` or `TimerRestart()`.

Consequence: the 100 ms interval delays only the first execution. From then on
the condition remains permanently true and the sensor loop runs on **every
superloop pass**, at an unspecified and load-dependent rate.

The established idiom elsewhere in this codebase pairs the two calls, for
example `c__motor.c` and `c__device.c`:

```c
if (TimerExpired(&timer)) {
    TimerReset(&timer);
    ...
}
```

Disposition required: `Correct` or `Preserve`. Do not port silently.

### N-020 `Type` configuration is ignored

`TOptSensoren.Type` selects Modbus or analog, but this module always connects
with `dtDptMod`. Either analog sensors are handled elsewhere, or the selector
is unimplemented. Classification: `TBD`.

### N-030 No defensive bound against `MAX_SENSOREN`

The loop is bounded only by `opt_app.Sensoren.Drukverschil`, an `unsigned char`
that may hold up to 255, while the output array holds 16 entries. The UI clamps
the value to `MAX_SENSOREN` (`c__disp_opt_sensoren_2.c:57`), so the overflow is
prevented by configuration entry, not by this module. A corrupted EEPROM
configuration would overflow `val_hr_alg.Sensoren.Drukverschil`.

Recommendation for the new platform: bound the loop with `MAX_SENSOREN`
regardless of configuration.

### N-040 No explicit stale-value or disconnect handling

When a device is not connected, the previous value remains in the output array
unchanged. Whether the application treats this as valid is not determined by
this module. Requires `mbDevice` analysis and stakeholder confirmation.

### N-050 Alarm re-registration every pass

`mbDeviceCreateAlarm()` is called on every iteration for every legal address,
not only once at startup. Idempotency is assumed but unverified.

## Execution context

- Called from the single superloop context in `c__main.c`; not from an ISR.
- Not reentrant; relies on module-static timer state.
- Performs no dynamic allocation.
- Execution time depends on `mbDevice` calls and is unmeasured.

## Porting classification

| Element | Classification |
|---|---|
| Sensor indexing and address mapping | Portable |
| Address validity rule | Portable |
| Poll scheduling | Reassess — see N-010 |
| `mbDevice` connect/read/alarm calls | Adapt |
| `s_board_IO_on_off` RS485 bus mapping | Replace |
| Output array publication | Portable |
