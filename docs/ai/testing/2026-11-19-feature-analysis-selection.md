# Analysis selection testing

Planned scenarios:

- Cached and newly scanned entries receive the same file-type selection.
- Headless explicit file paths exclude other files, including cached results.
- Recursive scans keep same-named files distinct by full path.
- The final review scan cannot reintroduce excluded entries.

Completed: `test_analysis_selection.cpp` has five cases and 17 assertions.
The first compile failed because `AnalysisSelection` did not exist; after the
implementation, the standalone Catch2 executable passed the initial four cases.
Syntax checks passed for `AnalysisCoordinator.cpp`,
`HeadlessAnalysisWorkflowHost.cpp`, and `MainApp.cpp`.

The repository test target built in `/tmp/aifs-validation`, a separate worktree
with the exact llama.cpp submodule revision. The source checkout's llama.cpp
directory contains local symlinks and cannot configure directly.

- `Analysis selection*`: 5 cases, 17 assertions passed.
- `HeadlessAnalysisCommand*`: 21 cases, 333 assertions passed.
- Selected-file and recursive-path subset: 4 cases, 46 assertions passed.
- The first full CTest run exposed six Windows/runtime/help-text case failures
  and translation sync failure. Windows CUDA ranking cases now run only on
  Windows, Windows runtime path cases use platform-native temporary paths,
  and the What's New test matches the packaged 1.9.2 release note and its
  English fallback. The five selection UI messages were added to all 14 catalogs;
  placeholders were checked and ambiguous translations corrected.
- Final `ctest --test-dir /tmp/aifs-validation/build-tests --output-on-failure`:
  both targets passed (31.05 seconds). Full Catch2 run passed 453 cases and
  5963 assertions before the final CTest run.
- The built desktop app opened in an isolated profile. With two cached test
  files and `pdf` in the File extensions field, its review dialog showed one
  row, `selected.pdf`, and excluded `other.txt`. The run queued one file for
  sorting and made no model request. No files were moved.
- `git diff --check`: passed.
- `npx ai-devkit@latest lint --feature analysis-selection`: 12 checks passed.
