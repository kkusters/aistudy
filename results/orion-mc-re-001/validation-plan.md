# Validation Plan: Differential Pressure Sensor Acquisition

## Unit tests (portable logic, host-executable)

Extract indexing and validation logic behind a test seam, then verify:

| ID | Requirement | Case | Expected |
|---|---|---|---|
| UT-010 | MD-030 | Address 0 | Rejected |
| UT-020 | MD-030 | Address 1 | Accepted |
| UT-030 | MD-030 | Address 247 | Accepted |
| UT-040 | MD-030 | Address 248 | Rejected |
| UT-050 | MD-020 | `FirstAddress=10`, count 3 | Addresses 10, 11, 12 |
| UT-060 | SR-080 | Configured count 200 | No write beyond 16 entries |
| UT-070 | MD-050 | Device not connected | Connect requested, no value stored |
| UT-080 | MD-060 | Device connected | Value stored at correct index |

## Integration tests (target with RS485 sensors)

| ID | Requirement | Procedure | Expected |
|---|---|---|---|
| IT-010 | SR-050 | Power up with sensor absent, then attach it | Connection established automatically; value appears |
| IT-020 | SR-070 | Disconnect a configured sensor | Per-sensor alarm is raised |
| IT-030 | SR-020, OI-010 | Capture RS485 traffic for 10 s | Measure the real poll period; compare against 100 ms |
| IT-040 | SR-040 | Configure first address so a sensor maps above 247 | No Modbus traffic to that address |

## System tests

| ID | Requirement | Procedure | Expected |
|---|---|---|---|
| ST-010 | SR-010 | Configure 16 sensors | All 16 values acquired and displayed |
| ST-020 | SR-020 | Apply a pressure step | Displayed value updates within the approved period |
| ST-030 | SR-060 | Compare displayed values against a reference instrument | Values agree within approved tolerance |
| ST-040 | SR-090 | Run unit tests on both reference and new-platform toolchains | Both pass without source changes to the portable logic |

## Acceptance tests

| ID | Requirement | Scenario | Expected |
|---|---|---|---|
| AT-010 | BR-010, BR-040 | Commission an installation with differential pressure sensors | Operator sees correct values on the display |
| AT-020 | BR-030 | Fail one sensor during operation | Operator receives an actionable alarm |
| AT-030 | BR-050 | Run reference and new-platform controllers on the same installation | Equivalent externally visible behavior |

## Priority

IT-030 is the highest-value test: it resolves OI-010 by measuring the actual
polling rate against the intended 100 ms.
