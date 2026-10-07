# Integration Test Plan / Report

**Traces to:** Architecture Design (AR-xxx)
**Test environment:** <target board / HIL rig / evaluation board>

## 1. Reverse-Engineering Notes
- How were module interactions historically verified (existing test rig,
  manufacturing test jig, bench script)? Reference/attach.

## 2. Test Setup
- Hardware used:
- Instrumentation (logic analyzer, oscilloscope, debugger, power analyzer):
- Build/firmware version under test:

## 3. Integration Test Cases
| IT ID | AR ID(s) | Modules involved | Description | Steps | Expected Result | Pass/Fail |
|-------|----------|--------------------|--------------|-------|--------------------|-----------|
| IT-010 | AR-010 | sensor_driver + i2c_hal | Sensor read over real I2C bus | 1. Power on 2. Trigger read | Value within calibrated range | |

## 4. Interface Conformance Checks
| Interface | Standard/Protocol | Test method | Result |
|-----------|---------------------|--------------|--------|

## 5. Timing/Performance Verification
| Measurement | Requirement (SR/AR) | Measured value | Pass/Fail |
|-------------|------------------------|------------------|-----------|

## 6. Fault Injection Tests
| Scenario | Expected behavior | Result |
|----------|---------------------|--------|
| Sensor disconnected | Error reported, no crash | |
| Brown-out during flash write | Recovers cleanly on reboot | |

## 7. Traceability
- Upstream: AR-xxx
- Downstream feeds: layer 08 System Test

## 8. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
