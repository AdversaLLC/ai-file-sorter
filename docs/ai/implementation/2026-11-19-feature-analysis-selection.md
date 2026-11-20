# Analysis selection implementation

Status: implemented and verified.

`AnalysisSelection` now decides eligibility for cached results, pending scan
entries, and the final review scan. It receives the host choice and the existing
image/document classification behavior. `AnalysisCoordinator` applies it at all
three stages and no longer repeats the mode filters or remaps cache indices
after filtering. The GUI passes extension choice through the shared callback;
the headless host passes normalized explicit paths through the same callback.

The old headless scan, preview, and final review post-filters were removed.
This keeps the selected-path rule in one place while preserving the documented
review/apply contract. `AnalysisWorkflowContext` changed only inside this
repository; both in-repo hosts were updated together. `app/CMakeLists.txt`
registers the new test. `.gitignore` allows the feature documents to be tracked.

Edge cases covered: cached entries, image-only mode, directories, same-named
recursive entries, UTF-8 cached paths, and final review selection. The existing
`ResultsCoordinator` continues to own name-versus-full-path matching.

The exact source plus the pre-existing local edits built successfully in a
separate worktree with the repository's pinned llama.cpp submodule. No changes
were made to the source checkout's local llama.cpp symlinks. The focused
headless and selection checks passed. Follow-up changes in
`test_windows_cuda_probe.cpp`, `test_ggml_runtime_paths.cpp`, and
`test_main_app_translation.cpp` corrected platform and release-note test
assumptions. Fourteen translation catalogs now include the five selection UI strings.
The full CTest suite then passed both registered targets.

## Design alignment

- Requirements: cached, pending, and final scans use `AnalysisSelection`;
  explicit headless paths use full-path keys, and GUI extensions use filename
  and file type. The headless selected-file command group passed.
- Design: the module owns eligibility while `ResultsCoordinator` retains scan
  and match logic. No selection path mutates files. Both in-repo hosts use the
  same workflow callback seam.
- Deviation: the workflow supplies the image/document classifier so the module
  remains independent of LLM and file analysis code.
- Desktop check: the built app opened in an isolated test profile. With two
  cached files and `pdf` in the extension field, the review dialog showed
  only `selected.pdf`, excluding `other.txt`. No model request or file move
  was needed for this check.
