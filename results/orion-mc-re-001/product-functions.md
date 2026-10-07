# Product Functions (Recovered): Orion MC

> ## ⚠️ SUPERSEDED
>
> This file was written from **source code alone**, before official vendor documentation
> was available. Its product-level framing has since been **refuted**.
>
> **Correction:** the Orion-MC does **not** compute climate setpoints and is **not** a
> climate controller. It is a **communication interface and subordinate position
> controller** that receives a position demand per motor group from a horticulture
> computer (Priva / Hoogendoorn) and translates it into individual motor commands.
>
> The "largest open question" recorded below — whether this unit computes setpoints
> itself — is now **RESOLVED: it does not.** Confirmed both by the manuals and by
> `MotorgroupGetTargetPosition()` (`c__motorgroep.c:301`), which contains no control law.
>
> **Read [`../orion-mc-re-002/`](../orion-mc-re-002/) instead** for the corrected
> product definition, function list and migration scope.
>
> The per-function code evidence below remains accurate and useful; only the
> product-level interpretation was wrong.

**Commit:** `d454c03`
**Evidence status:** `Implemented` — derived from static source analysis only.
No hardware was run and no stakeholder confirmed these statements.

## Summary

Orion MC is a greenhouse horticulture **position and motor controller**: a panel
controller with a graphical LCD that drives motorized greenhouse equipment,
supervises it for faults, and reports to a site supervisory computer.
It executes position demands; it does not originate them.

| # | Function | Scale / scope |
|---|---|---|
| F-01 | Motorized equipment control | 64 motors, 32 groups; vents, screens, fans, dampers |
| F-02 | Air handling and ventilation | 32 air-mixing units in 4 groups |
| F-03 | EC fan and actuator integration | 256 devices, 10 vendor protocols |
| F-04 | Measurement and sensing | 16 differential pressure sensors, analog/digital IO |
| F-05 | Alarm, supervision and safety | ~60 alarm codes, 2 severities, watchdog |
| F-06 | Operator interface | ~120 screens, 4 languages, password levels |
| F-07 | Communication | CAN backbone, CANopen, BACnet, Ethernet, RS232, RS485 |
| F-08 | Configuration and commissioning | EEPROM, 8 IO board variants, licensed modules |
| F-09 | Time, scheduling and logging | RTC, runtime counters, SD logging |

Best boundary for the platform port: the `TmbDeviceType` abstraction in F-03,
which already isolates 10 fan and actuator brands behind one interface.

Largest open question: ~~whether this unit computes climate setpoints itself or
executes setpoints from a supervisory computer.~~ **RESOLVED** — it executes
setpoints from a horticulture computer. See `../orion-mc-re-002/01-product-definition.md`.

## Product identification

| Property | Finding | Evidence |
|---|---|---|
| Domain | Greenhouse horticulture position and motor control (no climate regulation) | `TYPE_RAAM` (vents), `TYPE_DOEK` (screens), `CONTROL_CABRIOKAS`, `luchtmengkast` |
| Product family | Orion, computer identifier `ORION = 12` | `comm/serial/ct_computer.h` |
| Role | Panel controller with local operator display | LCD, key handling, menu screens |
| Market | International: Dutch, German, English, Spanish | `common/c__string.c`, ~1535 UI strings |
| Licensing | Per-feature module entitlements stored per unit | "welke modules, optie de klant gekocht heeft", `ct_data.h:270` |

The product drives motorized greenhouse equipment (roof vents, shade/energy
screens, fans, dampers), supervises it for faults, presents it to an operator,
and reports to a supervisory system.

## F-01 Motorized equipment control

Central function of the product.

| Aspect | Finding | Evidence |
|---|---|---|
| Capacity | Up to 64 motors in up to 32 groups | `MAX_MOTOR 64`, `MAX_GROUP 32` |
| Equipment types | Window/vent, screen/cloth, fan, damper | `TYPE_RAAM`, `TYPE_DOEK`, `TYPE_VENT`, `TYPE_KLEP` |
| Operating modes | Automatic, Manual, Off | `TOperationMode` |
| Commands | Open, Close, Stop | `TRunningMode` |
| Group control | Motors are driven as a group toward a setpoint; group average position is tracked | `TMotorgroup.Setpoint`, `PositionAvg` |
| Positioning | Position feedback with pulse/encoder counting | `TPulseSystem`, encoder alarm codes |
| Synchronization | Motors in a group are kept aligned; deviation is detected | `acNotSynchronous`, `acSpeedNotEqual`, `GelijkloopregelingFout` |
| Speed | High-speed option per group | `TMotorgroup.HiSpeed` |
| Staging | Start delay between motors | `MOTOR_DELAY 30` |

### Control variants

| Variant | Purpose | Evidence |
|---|---|---|
| `CONTROL_NORMAL` | Standard group control | `ch_motorgroep.h` |
| `CONTROL_DUALSCREEN` | Two screens on one bed, coordinated as a pair | `TDualScreen`, "2 doeken op 1 bed" / "2 Schirme an 1 Anlage" |
| `CONTROL_CABRIOKAS` | Cabrio (fully opening roof) greenhouse control | `devices/c__cabriokas.c` |

## F-02 Air handling and ventilation

| Function | Finding | Evidence |
|---|---|---|
| Air mixing units | Up to 32 units in 4 groups | `MAX_LUCHTMENGKAST 32`, `MAX_LUCHTMENGKAST_GROEP 4` |
| Controlled sections | Outside damper, inside damper, heating, exhaust fan, supply fan | `TLuchtmengkast` members |
| Damper strategy | Inside/outside mixing, or recirculation | `TYPE_KLEP_BINNEN_BUITEN`, `TYPE_KLEP_RECIRCULATIE` |
| Exhaust/top damper triggers | On underpressure, or coupled to damper position | `AFBLAASVENT_OP_ONDERDRUK`, `BOVENKLEP_GEKOPPELD_AAN_KLEP` |
| Heating | Cyclic heating control per unit | `CyclusTimerVerwarming` |
| Frost protection | Frost monitoring input | `DigInVorst`, `VorstBewaking` |
| Temperature inputs | Supply air and mixed air temperature | `AnaInInblaastemp`, `AnaInMengtemp` |
| Fan enable | Up to 16 external enable/interlock inputs | `TOptVrijgaveVent`, `MAX_VRIJGAVE 16` |

## F-03 EC fan and actuator integration

The controller speaks to commercial fan and actuator products over Modbus
RS485, supporting up to 256 devices (`MAX_DEVICE 256`).

| Supported device | Identifier |
|---|---|
| ebm-papst ebmBUS | `dtEbmBus` |
| ebm-papst Modbus | `dtEbmModbus` |
| ZIEHL-ABEGG ECblue Premium | `dtECbluePremium` |
| ZIEHL-ABEGG ECblue Modbus | `dtECblueModbus` |
| Belimo actuator | `dtBelimoModbus` |
| Rosenberg / Rosenberg Gen 3 | `dtRosenberg`, `dtRosenbergGen3` |
| Climafan | `dtClimafan` |
| Nicotra Gebhardt | `dtNicotraGebhardt` |
| Differential pressure sensor | `dtDptMod` |

Per device the controller supports open-loop and closed-loop operation,
setpoint write, actual value read, device temperatures (motor, power module,
electronics), ramp-up, connection supervision and device reset.

## F-04 Measurement and sensing

| Function | Evidence |
|---|---|
| Differential pressure measurement, up to 16 sensors | `MAX_SENSOREN 16`, `app/c__sensoren.c` |
| Analog inputs, including 0–5 V and 0–20 mA ranges | IO board ADC modules, UI strings |
| Digital inputs including limit switches, alarm and enable contacts | `SENSOR_TYPE_EINDSCHAKELAAR`, `DigInAlarm` |
| Analog outputs for modulating equipment | IO board DAC modules |
| Pulse/counter inputs | `IO_12_06_count` |

## F-05 Alarm, supervision and safety

| Function | Evidence |
|---|---|
| Two alarm severities: urgent ("hard") and non-urgent ("zacht") | `alarm_disp_ico` states, UI strings "Alarm dringend" / "Alarm zacht" |
| Active alarm list and alarm history with acknowledgement | "Alarmen Actief", "Alarmen Historie", "Alarmen Wissen" |
| Emergency stop detection | `MOTOR_EMCY_BIT_1` "Noodstop actief" |
| Motor thermal protection | `MOTOR_EMCY_BIT_2`, `acThermalOpen`, `acThermalClose` |
| Drive/encoder fault detection | `acEncFailure`, `acNoPulses`, `acNoFeedback` |
| Position deviation alarm and warning | `acDeviationPositionAlarm`, `ecDeviationPositionWarning` |
| Limit switch supervision | `acLimitSwitchNotReached`, `acLimitSwitchesNotEqual` |
| Wrong-direction detection | `acWrongDirection` |
| Communication loss per device and per bus | `acErrorCommunication`, `LostCommunication` |
| Supply/overload monitoring | UI string "24V supply overload" |
| Hardware watchdog | `WDI_Trigger()` called every loop pass, `system/c__wdi.c` |
| CPU trap/exception handling | `system/c__htrap.c` |
| Emergency power warning | `acWarningEmergencyPower` |

Approximately 60 distinct alarm codes exist for motor and system faults.

## F-06 Operator interface

| Function | Evidence |
|---|---|
| Graphical LCD with bitmap and pixel rendering | `ui/lcd/`, `c__lcd_bitmap.c`, `c__lcd_pixel.c` |
| Multiple font sizes | `c__lcd_7/10/14/20.c` |
| Keypad navigation with function keys F1–F3 | `ui/input/c__key.c`, `c__disp_F1_0.c`–`F3_0.c` |
| ~120 screens: status, settings, diagnostics, alarms | `ui/screens/` |
| Password-protected access levels | `c__disp_password.c`, `Password_Control()` |
| Multilingual UI, switchable at runtime | `EEP_Taal_Proc()`, four languages |
| Display dimming | `lcd_dimmen_cnt` |
| Diagnostic screens per subsystem | `disp_diag_*` screens for CAN, Ethernet, SD, motors, fans |

## F-07 Communication and supervisory integration

| Interface | Function | Evidence |
|---|---|---|
| CAN backbone | Communication with the site process computer, multi-department | `comm/can/`, `can_backbone_pc` |
| CANopen | DS301 + DS401 device profile, NMT slave | `comm/canopen/` |
| BACnet | Building automation server, optional | `comm/bacnet/`, `Bacnet_Init()` |
| Ethernet + uIP | TCP/IP networking, web server, up to 4 ports | `uip-1.0/`, `Ethernet_Control_Port(0..3)` |
| RS232 serial (2 ports) | PC/computer link | `comm/serial/c__rs232_0.c`, `_1.c` |
| RS485 | Field bus to fans, actuators and sensors | `IO_*_RS485.c`, `c__modbus.c` |
| Distributed IO over CAN | Remote IO boards | `comm/can/c__can_IO.c` |

## F-08 Configuration and commissioning

| Function | Evidence |
|---|---|
| Persistent configuration in EEPROM with change detection | `comp_ram_eep()`, `system/storage/c__eep.c` |
| Installer configuration menus per subsystem | `disp_opt_*` screens |
| Support for 8 IO board variants | `io/` — 05_07, 06_14, 07_07, 08_09, 12_06, EKU, H1MC, H2MC |
| Per-unit licensed feature modules | `module.CanOpen`, `.Ventilatie`, `.Luchtmengkast`, `.BACnet`, `.Schakelgroepen`, `.Klep`, `.Drukverschil`, `.Hoogendoorn` |
| Runtime module presence check | `ModuleCheck()`, `CheckOptions()` |
| Sensor calibration/adjustment | UI strings "Adjust sensor", "Ajuste sensor" |
| Motor installation mode | `acInstallMode` |
| Serial number and version identification | `module.serie_number`, `versie_programma` |

## F-09 Time, scheduling and logging

| Function | Evidence |
|---|---|
| Battery-backed real-time clock over I2C | `system/c__rtc.c`, `RTC_Read()`, `RTC_Write()` |
| Time base: 1 s, 1 min, 1 hour, 1 day events | `Tijd_Check_Flag_1s/1min/1hour/1day` |
| Runtime hours and switch-count per motor | `TMotorStatistics.Runtime`, `.Switches`, `.Failures` |
| SD card data logging with configurable variables and interval | `system/storage/c__sd_log.c`, `SD_Log_Data_Aanmaken()` |
| SD card file management: read, delete, format | `ch_sd.h` |
| Daily log rollover | `SD_Log_Einde_Dag()` |

## Architecture note

All functions run cooperatively in a single non-preemptive superloop in
`common/c__main.c`, paced by a 5 ms system tick, with periodic work gated at
100 ms, 125 ms, 500 ms, 1 s, 1 min, 1 hour and 1 day. The watchdog is
retriggered once per loop pass.

## Confidence and gaps

| Function | Confidence | Reason |
|---|---|---|
| F-01, F-03, F-05, F-06, F-07 | High | Explicit types, enums and named alarm codes |
| F-02, F-04, F-08, F-09 | Medium-high | Clear structures; control strategies not yet traced |
| Climate control strategy | **Not applicable** | `Documented` — the product performs no climate regulation; it is a subordinate position controller |

Open questions for the product owner:

1. ~~Does this controller compute climate setpoints itself, or does it execute
   setpoints issued by a supervisory process computer over the CAN backbone?~~
   **RESOLVED — it executes externally issued setpoints.**
2. Which feature modules are actually sold, and which are legacy?
3. ~~What is the `Hoogendoorn` module — a third-party integration?~~
   **RESOLVED — integration with the Hoogendoorn horticulture computer, one of two
   named target systems alongside Priva.**
4. Are `ORION_OLD` and other computer types still supported in the field?
