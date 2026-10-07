# Coding Standard & Conventions

**Product:** <name/model number>
**Applies to:** <language(s), e.g. C99/C11, MISRA subset>

## 1. Reverse-Engineering Notes
- This document may be written *after* the fact by observing conventions
  actually used in the existing codebase, then deciding which to keep,
  formalize, or fix for the next platform.
- Tool used to scan style (clang-format config, cppcheck, PC-lint, MISRA
  checker) and its config file location:

## 2. Toolchain / Build
| Item | Value |
|------|-------|
| Compiler + version | |
| Build system (Make/CMake/IDE project) | |
| Static analysis tool(s) | |
| Version control | |
| CI system | |

## 3. Language Standard & Compliance
- Language standard (e.g. C11, C++17):
- Coding guideline followed (MISRA-C:2012, CERT-C, Barr Group, in-house):
- Deviations permitted and process to record them:

## 4. Naming Conventions
| Item | Convention | Example |
|------|------------|---------|
| Functions | | |
| Variables | | |
| Constants/Macros | | |
| Types | | |
| Files | | |

## 5. File/Module Organization
- Header guard convention:
- One module = one .c/.h pair? Layering rules (app must not call HAL directly, etc.):

## 6. Memory & Resource Rules
- Dynamic allocation policy (allowed/forbidden and why):
- Global variable policy:
- Recursion policy:

## 7. Error Handling Convention
- Return code vs. exception vs. error callback:
- Assert/logging strategy:

## 8. Code Review Checklist
- [ ] Follows naming conventions
- [ ] No new MISRA violations (or documented deviation)
- [ ] Unit tests added/updated
- [ ] Traceability IDs referenced in comments (`// implements MD-010`)
- [ ] No magic numbers without named constant
- [ ] ISR duration within budget (see AR-xxx)

## 9. Traceability Convention in Code
- Recommend a comment tag convention so code can be grepped back to
  requirements, e.g.:
  ```c
  // @implements MD-010, SR-010
  int sensor_read(uint16_t *out_mv) { ... }
  ```

## 10. Revision History
| Version | Date | Author | Change |
|---------|------|--------|--------|
