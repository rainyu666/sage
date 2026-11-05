# SAGE experiment data
## Dataset
`benchmark-48/subjects.csv` indexes all 48 cases. Each case directory contains:
- `case.c`: a formatted version of the source program used in the experiment. Formatting and header comments have been standardized for readability; executable code is unchanged. The exact source text supplied to the model is retained in the `request` fields of `process-and-verification.json`.
- `profile.json`: experimental inputs defining observable variables, operation count, interrupt delivery and state projection.

Derived data is stored separately under `models-and-results/benchmark-derived/<case-directory>/`, using the same case directory names as `benchmark-48/`:
- `anchors.json`: source anchors extracted for the modeling task.
- `reference-traces.json`: source execution states for every declared schedule.
- `baseline-public-traces.json`: the public execution traces available to the adapted baselines.

Source hashes and line numbers in `anchors.json` refer to the source text retained in the archived requests, rather than the formatted `case.c` files.

## Model generation and verification
`models-and-results/<method>/run-<1|2|3>/P<01..48>/` contains each individual program instance:
- `initial-model.json`: the initial generated model, where applicable.
- `final-model.json`: the final generated model, where applicable. Single-pass Direct has only this model; without-refinement has only the initial model.
- `process-and-verification.json`: original requests and responses, stage names, available structural feedback, verification decisions and discrepancy witnesses. `model_files` identifies the readable model files. The requests refer to `response-schemas.json`.
- `initial-model.pml` / `final-model.pml`: archived Promela models beside their corresponding JSON models, where export succeeded.
- `initial-trace-check.pml` / `final-trace-check.pml`: corresponding deterministic SPIN trace checks.
- `model-traces.json`: observable state sequences for initial/final candidates, including recorded or replayed execution failures. Main-experiment traces are preserved from the original archive; external-baseline model traces are deterministic replays.

All 1,152 final instances and all 2,449 generation-stage records are retained, including failed cases. The without-refinement condition retains its own archived samples.

## Results and SPIN validation
`results/outcomes.csv` records full consistency and logical calls for each method, run and subject. `results/summary.csv` aggregates these results by method.

`results/spin-validation/` contains the archived SPIN backend validation results:
- `spin-results.jsonl`: per-candidate validation statuses and execution logs.
- `summary.json`: SPIN version, validation scope and aggregate statuses for 1,728 candidates (1,724 trace matches, one execution error and three invalid IR candidates).
- `spin-model-index.csv`: maps candidate identifiers to archived Promela model and trace-check files; paths are relative to this experiment directory.
