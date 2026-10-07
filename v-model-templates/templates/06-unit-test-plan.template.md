# Unit Test Plan / Report — <module_name>

**Traces to:** Module Design (MD-xxx)
**Test framework:** <Unity/CMock/GoogleTest/Ceedling/on-target vs. host>

## 1. Reverse-Engineering Notes
- If tests didn't exist: were these written by black-box probing of the
  function (inputs/outputs observed) or white-box (reading source)?
- Coverage tool/result if measured:

## 2. Test Environment
- Host-based (mocked HAL) or on-target?
- Mocking strategy for dependencies:

## 3. Test Cases
| UT ID | MD ID | Description | Preconditions | Input | Expected Output | Pass/Fail | Notes |
|-------|-------|--------------|-----------------|-------|-------------------|-----------|-------|
| UT-010 | MD-010 | sensor_read returns error when I2C NACK | I2C mocked to NACK | call sensor_read() | returns -EIO | | |

## 4. Boundary / Edge Cases Checklist
- [ ] Min/max input values
- [ ] Null/invalid pointers
- [ ] Timeout paths
- [ ] Concurrent access (if reentrant)
- [ ] Power-loss / reset mid-operation (if applicable)

## 5. Coverage Summary
| Metric | Target | Actual |
|--------|--------|--------|
| Statement coverage | | |
| Branch/decision coverage | | |
| MC/DC (if safety-critical) | | |

## 6. Traceability
- Upstream: MD-xxx
- This file itself is the validation counterpart of layer 04 Module Design (LLD).

## 7. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
