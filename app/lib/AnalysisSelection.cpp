#include "AnalysisSelection.hpp"

#include <algorithm>
#include <filesystem>
#include <utility>

AnalysisSelection::AnalysisSelection(bool images_only,
                                     bool documents_only,
                                     Classify classify,
                                     Include include)
    : images_only_(images_only),
      documents_only_(documents_only),
      classify_(std::move(classify)),
      include_(std::move(include))
{
}

bool AnalysisSelection::includes(const FileEntry& entry) const
{
    if (include_ && !include_(entry)) {
        return false;
    }
    if (!images_only_ && !documents_only_) {
        return true;
    }
    if (entry.type != FileType::File) {
        return false;
    }
    const Kind kind = classify_(entry);
    return (images_only_ && kind == Kind::Image) ||
           (documents_only_ && kind == Kind::Document);
}

bool AnalysisSelection::includes(const CategorizedFile& entry) const
{
    const auto directory = std::u8string(entry.file_path.begin(), entry.file_path.end());
    const auto file_name = std::u8string(entry.file_name.begin(), entry.file_name.end());
    const auto path = std::filesystem::path(directory) / std::filesystem::path(file_name);
    const auto utf8_path = path.u8string();
    return includes(FileEntry{std::string(utf8_path.begin(), utf8_path.end()),
                              entry.file_name,
                              entry.type});
}

void AnalysisSelection::retain(std::vector<FileEntry>& entries) const
{
    std::erase_if(entries, [this](const FileEntry& entry) { return !includes(entry); });
}

void AnalysisSelection::retain(std::vector<CategorizedFile>& entries) const
{
    std::erase_if(entries, [this](const CategorizedFile& entry) { return !includes(entry); });
}
