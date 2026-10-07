# Embedded Software Reverse-Engineering Workflow

## 1. Objective

Convert an existing embedded product and its source code into a controlled,
testable, traceable set of:

- Business requirements
- System requirements
- Software architecture
- Module designs
- Characterization and validation tests

The workflow captures existing behavior without automatically declaring every
legacy behavior correct. The resulting requirements become the input for
implementing the product on a new platform.

## 2. Core rule

Maintain three separate descriptions throughout the work:

1. **Current implementation:** What the code contains.
2. **Observed behavior:** What the product actually does.
3. **Intended requirement:** What an authorized stakeholder decides the
   product must do.

Code proves implementation, not intent. Only an approved requirement defines
future product behavior.

## 3. Roles

One person may perform multiple roles, but approvals should identify the role
under which a decision was made.

| Role | Responsibility |
|---|---|
| Reverse-engineering lead | Controls scope, evidence, IDs, and traceability |
| Embedded developer | Analyzes code, architecture, timing, and resources |
| Hardware engineer | Confirms circuits, peripheral behavior, and limits |
| Test engineer | Creates characterization and validation tests |
| Product/domain owner | Confirms customer-visible intent and priorities |
| Quality/safety/security owner | Reviews applicable risks and compliance |

## 4. Evidence classification

Tag every derived statement with one of these classifications:

| Status | Meaning |
|---|---|
| `Observed` | Reproduced by executing or measuring the product |
| `Implemented` | Present in code but not yet confirmed on the product |
| `Documented` | Found in an existing controlled document |
| `Inferred` | Probable intent requiring stakeholder confirmation |
| `Approved` | Accepted as a requirement by an authorized owner |
| `Defect candidate` | Existing behavior may be incorrect or unintended |
| `TBD` | Information is missing; an owner and due date are required |

Do not mark an item `Approved` solely because the current code implements it.

## 5. Required inputs

Collect as many of these as are available:

- Source repository and exact revision
- Reproducible build instructions
- Compiler, linker, toolchain, and build options
- Hardware schematics, BOM, PCB revision, and datasheets
- Product variants and configuration data
- Bootloader, RTOS, middleware, and third-party library versions
- User manuals, interface specifications, and customer documentation
- Existing requirements, design notes, and test reports
- Manufacturing, calibration, and service procedures
- Field defects, customer complaints, and known limitations
- Regulatory, safety, cybersecurity, and environmental constraints
- Reference products and suitable test equipment

Missing inputs are not blockers by default. Record them as evidence gaps and
reduce confidence accordingly.

## 6. Workflow overview

```text
Scope and baseline
        |
        v
Build and run reference product
        |
        v
Inventory code and hardware
        |
        v
Select one vertical feature slice
        |
        v
Characterize observable behavior
        |
        v
Document module design from code
        |
        v
Recover architecture and interfaces
        |
        v
Derive system requirements
        |
        v
Recover business intent
        |
        v
Define right-side validation tests
        |
        v
Review, approve, and baseline
        |
        v
Repeat for next feature
```

## 7. Detailed workflow

### Phase 0 — Define scope

Choose a bounded product, variant, and feature set. Do not begin with the whole
codebase unless it is very small.

**Activities**

- Identify the product and firmware variants in scope.
- Select the first externally visible feature.
- List explicitly excluded features and variants.
- Assign document owners and reviewers.
- Decide applicable standards and required rigor.

**Outputs**

- Scope statement
- Feature list
- Assigned roles
- Initial risks and evidence gaps

**Exit criteria**

- A reviewer can state exactly which product behavior is being recovered.

### Phase 1 — Freeze the reference baseline

Create a reproducible reference point before interpreting behavior.

**Record**

| Item | Example |
|---|---|
| Firmware revision | Git commit or released binary checksum |
| Hardware | Board revision and populated variant |
| Build | Compiler version, flags, linker script |
| Configuration | Product options, calibration/NVM contents |
| Dependencies | RTOS, bootloader, libraries |
| Test unit | Serial number and modifications |

**Outputs**

- Baseline record
- Reproducible firmware image
- Known-differences list for other variants

**Exit criteria**

- The reference firmware can be rebuilt or its released binary is preserved.
- The reference product can be run in a known configuration.

### Phase 2 — Build and execute the reference

Establish that the team can observe behavior safely.

**Activities**

- Build with the original toolchain.
- Record warnings without immediately changing the code.
- Program and start the reference product.
- Capture startup logs, reset cause, and normal operating state.
- Identify test points and safe stimulus limits.

**Outputs**

- Build log
- Startup observation
- List of unavailable or unsafe tests

**Exit criteria**

- At least one known product function can be reproduced and measured.

### Phase 3 — Inventory the implementation

Map the system before documenting details.

**Activities**

- Identify entry points, initialization order, tasks, superloops, and ISRs.
- List modules and their apparent responsibilities.
- Extract calls between modules and external library dependencies.
- Record peripherals, buses, pins, DMA channels, interrupts, and timers.
- Record memory regions, persistent storage, and boot/update paths.
- Identify configuration flags and product variants.
- Search for constants, timeouts, limits, state machines, and fault handling.

**Outputs**

- Code/module inventory
- Hardware/software boundary
- Task and interrupt table
- Dependency diagram
- Initial platform-specific versus portable classification

**Exit criteria**

- Every executable module has an owner or an `Unknown` classification.
- Main control and data paths are identified.

### Phase 4 — Select a vertical feature slice

Analyze one behavior end-to-end instead of completing one document layer for
the entire product.

Good first slices include:

- One sensor measurement
- One actuator command
- One communication message
- One alarm or diagnostic
- One operating-mode transition

Avoid beginning with bootloaders, broad communication stacks, or complex
safety functions unless they are the primary objective.

**Feature record**

| Question | Answer |
|---|---|
| What external event starts the behavior? | |
| What output can a user or connected system observe? | |
| Which code entry point handles it? | |
| Which modules and hardware are involved? | |
| What normal and abnormal cases exist? | |

**Exit criteria**

- The slice has a clear external trigger and observable result.

### Phase 5 — Characterize current behavior

Create black-box or grey-box tests before refactoring or porting.

**Test**

- Nominal operation
- Minimum, maximum, and boundary values
- Threshold equality
- Timing, timeout, and sequencing
- State transitions and restart behavior
- Invalid or missing inputs
- Communication loss and corrupted data
- Sensor/actuator failure
- Brownout, reset, and power loss where safe
- Repetition, long duration, and counter rollover

Record raw measurements separately from conclusions.

**Outputs**

- Characterization tests
- Captured logs, traces, or measurements
- Observed-behavior statements
- Differences between observed and implemented behavior

**Exit criteria**

- Important behavior can be repeated.
- Each result identifies firmware, hardware, configuration, and equipment.

### Phase 6 — Document module design

Use `templates/04-module-design.template.md` for each module in the
feature slice.

**Document**

- Public APIs and callbacks
- Preconditions and postconditions
- Units, ranges, and invalid values
- Internal state and state transitions
- Algorithms and constants
- Dependencies and shared resources
- Execution context and reentrancy
- Error handling and recovery
- Timing and memory use
- Hardware-specific implementation points

Assign `MD-xxx` IDs to behavior worth preserving or explicitly changing.

**Outputs**

- Module design documents
- Unit-level characterization tests
- Questions and defect candidates

**Exit criteria**

- Each MD requirement has code evidence and a unit test or a documented reason
  why it cannot be unit-tested.

### Phase 7 — Recover architecture

Generalize module relationships into responsibilities and boundaries.

Use `templates/03-architecture-design.template.md`.

**Document**

- Software layers and allowed dependencies
- Tasks, threads, callbacks, and ISRs
- Data and control flow
- Inter-module interfaces
- External protocols
- Error propagation and fault containment
- Watchdog and reset strategy
- Persistent-data ownership
- Platform abstraction boundaries

Assign `AR-xxx` IDs to important structural decisions.

**Porting classification**

| Classification | Meaning |
|---|---|
| Portable | Product logic expected to remain unchanged |
| Adapt | Interface remains; implementation changes |
| Replace | Platform-owned implementation must be rewritten |
| Reassess | Existing decision may not suit the new platform |

**Exit criteria**

- Every module in the slice has a defined architectural responsibility.
- Hardware-specific code has a visible boundary.

### Phase 8 — Derive system requirements

Translate implementation details into externally measurable behavior. Use
`templates/02-system-requirements.template.md`.

Write each requirement as:

> **The system shall [observable response] when [condition], within [limit].**

For each `SR-xxx`, record:

- Upstream product need, if known
- Trigger and operating mode
- Observable response
- Units, tolerance, and timing
- Failure behavior
- Verification method
- Evidence and confidence

Do not include function names, registers, RTOS APIs, or MCU peripherals unless
they are genuine external constraints.

**Exit criteria**

- Each system requirement has an objective pass/fail test.
- Implementation details have been removed unless necessary.
- Uncertain intent is marked `Inferred`, not `Approved`.

### Phase 9 — Recover business and stakeholder intent

Use interviews, manuals, field evidence, and domain knowledge to determine why
the system behavior exists. Use
`templates/01-business-requirements.template.md`.

**Ask**

- Who needs this behavior?
- What risk or user problem does it address?
- What happens if the behavior is removed?
- Which thresholds and timings are contractual?
- Which current limits are merely consequences of the old platform?
- Are known anomalies defects or compatibility requirements?
- What must remain identical on the new platform?
- What may intentionally improve or change?

Assign `BR-xxx` IDs only to stakeholder/product needs, not code mechanics.

**Exit criteria**

- Each retained system behavior traces to a product need or an approved
  technical/regulatory constraint.
- Unwanted legacy behavior is tracked as a defect or change request.

### Phase 10 — Define validation on the right side of the V

Create validation artifacts while evidence is fresh.

| Left-side artifact | Validation counterpart |
|---|---|
| Module design (`MD`) | Unit tests (`UT`) |
| Architecture (`AR`) | Integration tests (`IT`) |
| System requirement (`SR`) | System tests (`ST`) |
| Business requirement (`BR`) | Acceptance tests (`AT`) |

Tests must include:

- Preconditions and configuration
- Required equipment and test environment
- Exact stimuli and procedure
- Measurable expected result
- Tolerance and pass/fail rule
- Evidence to retain

**Exit criteria**

- Every approved requirement has a validation method and linked test ID.

### Phase 11 — Review and disposition differences

Review implementation, observations, and inferred intent together.

For each disagreement, choose one:

| Disposition | Action |
|---|---|
| Preserve | Make current behavior an approved requirement |
| Correct | Specify desired behavior and log current behavior as a defect |
| Improve | Approve a deliberate behavior change for the new platform |
| Remove | Confirm the behavior is obsolete |
| Investigate | Retain `TBD` with owner and due date |

Do not silently change a derived requirement to match stakeholder preference;
retain the evidence and record the approved disposition.

**Exit criteria**

- No unresolved difference is hidden.
- Safety-, security-, and compliance-relevant decisions have appropriate
  reviewers.

### Phase 12 — Baseline the recovered specification

Update `_traceability/traceability-matrix.csv` or the feature-specific matrix.

**Check**

- Every BR traces to at least one SR and AT.
- Every SR traces to a BR/constraint, AR, and ST.
- Every AR traces to an SR, MD, and IT.
- Every MD traces to an AR and UT.
- Every requirement has source, status, owner, and revision history.
- Every `TBD` has an owner and due date.
- Obsolete and defect-candidate behaviors are excluded from the approved
  baseline unless explicitly accepted.

**Outputs**

- Reviewed V-Model documents
- Completed traceability matrix
- Test evidence
- Open-issue register
- Approved baseline for platform migration

## 8. Requirement quality checklist

Before approving a requirement, verify:

- [ ] Uses **shall** for mandatory behavior
- [ ] Contains one primary obligation
- [ ] Identifies the responsible system or component
- [ ] States its trigger or applicable operating condition
- [ ] Has measurable limits, units, and tolerances
- [ ] Defines abnormal behavior where relevant
- [ ] Avoids vague terms such as fast, adequate, normally, or user-friendly
- [ ] Avoids unnecessary implementation details
- [ ] Does not preserve an old-platform limitation without justification
- [ ] Has a unique ID and source
- [ ] Has a validation method and test link
- [ ] Is feasible and does not conflict with another requirement

## 9. Suggested tracking register

Maintain a CSV or issue tracker with:

| Field | Purpose |
|---|---|
| Item ID | Requirement, question, defect, or evidence identifier |
| Feature | Vertical slice |
| Description | Concise statement |
| Source | Code location, measurement, document, or stakeholder |
| Evidence status | Observed, Implemented, Inferred, etc. |
| Confidence | High, medium, or low |
| Owner | Person responsible for resolution |
| Due date | Required for TBD items |
| Disposition | Preserve, Correct, Improve, Remove, Investigate |
| Links | BR/SR/AR/MD and test IDs |

## 10. Recommended iteration size

For the first iteration, choose a feature with approximately:

- 1 externally visible behavior
- 1 to 3 modules
- 5 to 15 module requirements
- 2 to 5 system requirements
- 1 to 3 business requirements
- Enough tests to cover normal, boundary, and failure behavior

Complete its entire vertical trace before expanding scope. This exposes gaps in
the process early and produces a usable example for the rest of the team.

## 11. Completion definition

A feature is reverse-engineered when:

1. Its reference baseline and evidence are identifiable.
2. Existing behavior is reproducible.
3. Code, module design, architecture, system behavior, and product intent are
   distinguished.
4. Uncertainty and suspected defects are visible.
5. Approved requirements are measurable and platform-independent where
   possible.
6. Every approved item has end-to-end traceability and a validation method.
7. The product/domain owner and required engineering reviewers have approved
   the intended behavior for the new platform.

