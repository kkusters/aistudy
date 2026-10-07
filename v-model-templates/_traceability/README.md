# Traceability Matrix

`traceability-matrix.csv` is the single source of truth linking every layer
of the V (one row per lowest-level item, e.g. per MD/UT pair, or per BR if
no lower-level item exists yet).

Columns: `BR_ID, SR_ID, AR_ID, MD_ID, UT_ID, IT_ID, ST_ID, AT_ID, Status, Notes`

Status values: `Draft`, `Reverse-Engineered`, `Reviewed`, `Approved`, `Ported`.

## Workflow
- **Reverse engineering:** add rows bottom-up as you write 06→05→04→03→02→01;
  it's fine for upstream columns (SR/BR) to be blank initially — fill them
  once inferred.
- **Forward engineering (new platform):** filter this matrix to find every
  MD/UT row tied to platform-specific AR items; those are the only rows that
  need new content. Rows where MD/UT are unaffected by the platform change
  can be marked `Ported` and reused as-is.
- Keep one matrix per product; if variants diverge significantly, add a
  `Variant` column rather than forking the file.
