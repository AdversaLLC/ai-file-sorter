# Analysis selection design

The analysis selection module owns file eligibility for cached results, pending
analysis, and final review. GUI and headless hosts each supply their selection
policy at the same seam. Cached results use their full path so explicit headless
file selection can be applied before cache state is interpreted.

Image-only and document-only settings are the same policy for cached and new
entries. The module does not own cache reconciliation, file moves, or LLM
routing. The workflow continues to use `ResultsCoordinator` for scanning and
matching. Tests exercise selection through its interface with real file entries.
