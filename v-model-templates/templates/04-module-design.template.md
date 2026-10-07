# Module Design Document (LLD) — <module_name>

**File(s) analyzed:** <path/to/file.c, path/to/file.h>
**Traces to:** Architecture Design (AR-xxx)

## 1. Reverse-Engineering Notes
- Analyzed by reading: <function names, commit hash>
- Tools used (static analysis, debugger, code coverage):
- Ambiguous/undocumented behavior found (list explicitly, mark TODO):

## 2. Purpose
- One paragraph: what this module does and why it exists.

## 3. Public Interface
| Function/API | Signature | Pre-conditions | Post-conditions | Requirement ID (MD) |
|---------------|-----------|------------------|-------------------|-----------------------|
| e.g. sensor_read() | `int sensor_read(uint16_t *out_mv)` | I2C initialized | returns 0 on success, fills out_mv | MD-010 |

## 4. Internal Design
- Internal state / static variables (list each, meaning, valid range):
- State machine (if any) — states, transitions, guard conditions (link diagram):
- Algorithms (describe or pseudocode; note magic numbers/constants and their origin):

## 5. Data Structures
| Struct/Enum | Fields | Meaning | Persistence (RAM/Flash/NVM) |
|-------------|--------|---------|-------------------------------|

## 6. Dependencies
| Depends on | Direction | Reason |
|------------|-----------|--------|

## 7. Timing / Resource Usage
- Worst-case execution time (measured/estimated):
- Stack usage:
- Called from (ISR context? task context?):

## 8. Error Handling
- Error codes returned and meaning:
- Failure modes and recovery:

## 9. Coding Standard Deviations (if reverse-engineered from legacy code)
- Note any deviations from the project coding standard (your filled-in copy of
  `templates/05-coding-standard.template.md`) found in this module, and whether
  to fix now or preserve as-is for behavioral parity.

## 10. Unit Test Plan Summary (detail in layer 06 Unit Test)
| MD ID | Linked Unit Test ID(s) |
|-------|---------------------------|

## 11. Porting Notes (for new platform)
- Hardware-specific parts to replace:
- Suggested abstraction point:

## 12. Traceability
- Upstream: AR-xxx
- Downstream: UT-xxx

## 13. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
