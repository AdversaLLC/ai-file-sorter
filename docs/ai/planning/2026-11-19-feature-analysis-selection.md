# Analysis selection plan

- [x] Record selection requirements and design constraints.
- [x] Add failing tests for cached, pending, and final review selection.
- [x] Deepen the analysis selection module and update GUI and headless callers.
- [x] Run focused and full checks; review the diff and complete the records.

Existing local edits in the analysis and UI files are preserved. The current
branch is `feature-analysis-selection`.

All planned tasks are complete. The follow-up verification failures were
resolved: Windows-only test assumptions were corrected, the current release
note test now matches the packaged content, and selection UI messages were added to
all 14 translation catalogs. The desktop app was opened with an isolated test
profile. With two cached test files and a `pdf` filter, its review dialog
showed only `selected.pdf` and excluded `other.txt`.
