# Analysis selection requirements

The normal GUI analysis and headless analysis share one workflow. Selection must
give the same answer for cached entries, newly scanned entries, and the final
review scan. GUI extension selection and headless explicit paths must remain
respected. Recursive scans must distinguish duplicate names by full path.

The headless runtime contract in `docs/headless-runtime-contract.md` remains
stable: only selected files appear in review and apply output. Analysis must
remain local to the desktop application, and no file mutation occurs during
selection.
