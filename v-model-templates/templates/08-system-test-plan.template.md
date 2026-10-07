# System Test Plan / Report

**Traces to:** System Requirements (SR-xxx)
**Test environment:** <production-representative unit, environmental chamber, EMC lab>

## 1. Reverse-Engineering Notes
- Existing system test reports/certification test reports used as source:
- Behavior observed but not previously documented as a requirement (flag for
  layer 01 Business Requirements / layer 02 System Design update):

## 2. Test Setup
- Full system configuration (HW rev, FW version, enclosure):
- Test equipment:

## 3. Functional System Test Cases
| ST ID | SR ID | Description | Preconditions | Steps | Expected Result | Pass/Fail |
|-------|-------|--------------|-----------------|-------|--------------------|-----------|
| ST-010 | SR-010 | End-to-end sampling rate verification | Powered, calibrated | Run for 1 hr, log samples | Sample period 100 ms ±5 ms | |

## 4. Non-Functional System Tests
| ST ID | Category | Requirement (SR) | Method | Result |
|-------|----------|---------------------|--------|--------|
| ST-100 | Power consumption | SR-140 | Bench measurement | |
| ST-110 | EMC | SR-150 | EMC lab | |
| ST-120 | Environmental (temp/vibration) | SR-160 | Chamber test | |
| ST-130 | Reliability/soak | SR-110 | 72h burn-in | |

## 5. State/Mode Transition Tests
| ST ID | Transition tested | Expected result | Pass/Fail |
|-------|----------------------|--------------------|-----------|

## 6. Regression Checklist (for platform port)
- [ ] All ST cases re-run unchanged on new platform
- [ ] Any ST case requiring updated pass criteria (document why)
- [ ] Any ST case no longer applicable (document why, get sign-off)

## 7. Traceability
- Upstream: SR-xxx
- Downstream feeds: layer 09 Acceptance Test

## 8. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
