#pragma once

#include "Types.hpp"

#include <functional>
#include <vector>

/** File eligibility shared by cached intake, pending analysis, and review scans. */
class AnalysisSelection {
public:
    enum class Kind { Image, Document, Other };
    using Classify = std::function<Kind(const FileEntry&)>;
    using Include = std::function<bool(const FileEntry&)>;

    AnalysisSelection(bool images_only,
                      bool documents_only,
                      Classify classify,
                      Include include = {});

    bool includes(const FileEntry& entry) const;
    bool includes(const CategorizedFile& entry) const;
    void retain(std::vector<FileEntry>& entries) const;
    void retain(std::vector<CategorizedFile>& entries) const;

private:
    bool images_only_;
    bool documents_only_;
    Classify classify_;
    Include include_;
};
