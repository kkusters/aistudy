# Business Requirements: Overtemperature Monitor

**Product:** Example Motor Controller
**Status:** Reverse-engineered draft
**Source:** Existing firmware behavior; stakeholder approval pending

## Scope

The firmware shall protect the product from sustained overtemperature while
avoiding nuisance alarms caused by brief temperature excursions.

## Requirements

| ID | Requirement | Rationale | Priority | Source |
|---|---|---|---|---|
| BR-010 | The product shall warn the host when sustained internal temperature could reduce product life or create unsafe operation. | Protect the product and its surroundings. | Must | Inferred from alarm output and trip logic |
| BR-020 | The product shall not generate nuisance temperature alarms during brief excursions or normal sensor noise. | Avoid unnecessary shutdown and service calls. | Must | Inferred from consecutive-sample filtering and hysteresis |
| BR-030 | Temperature protection behavior shall remain equivalent when the product moves to a new MCU platform. | Permit platform migration without changing customer-visible behavior. | Must | Product migration goal |

## Constraints and assumptions

- Temperature limits require review by the hardware and safety owners.
- This example is not a complete functional-safety specification.
- The sensor accuracy, placement, and thermal response are system properties,
  not properties of the software module alone.

## Acceptance links

| Requirement | Acceptance tests |
|---|---|
| BR-010 | AT-010 |
| BR-020 | AT-020 |
| BR-030 | AT-010, AT-020 |
