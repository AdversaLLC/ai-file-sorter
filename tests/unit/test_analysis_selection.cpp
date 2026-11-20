#include <catch2/catch_test_macros.hpp>

#include "AnalysisSelection.hpp"
#include "Types.hpp"

#include <string>
#include <vector>

namespace {

AnalysisSelection make_selection(bool images_only,
                                 bool documents_only,
                                 const std::string& selected_path)
{
    return AnalysisSelection(
        images_only,
        documents_only,
        [](const FileEntry& entry) {
            if (entry.file_name.ends_with(".jpg")) {
                return AnalysisSelection::Kind::Image;
            }
            if (entry.file_name.ends_with(".pdf")) {
                return AnalysisSelection::Kind::Document;
            }
            return AnalysisSelection::Kind::Other;
        },
        [selected_path](const FileEntry& entry) {
            return selected_path.empty() || entry.full_path == selected_path;
        });
}

CategorizedFile cached(const std::string& directory, const std::string& name)
{
    CategorizedFile entry{};
    entry.file_path = directory;
    entry.file_name = name;
    entry.type = FileType::File;
    entry.category = "Documents";
    return entry;
}

} // namespace

TEST_CASE("Analysis selection applies host path choice to cached and scanned files")
{
    const auto selection = make_selection(false, false, "/root/selected.pdf");
    CHECK(selection.includes(cached("/root", "selected.pdf")));
    CHECK_FALSE(selection.includes(cached("/root", "other.pdf")));
    CHECK(selection.includes(FileEntry{"/root/selected.pdf", "selected.pdf", FileType::File}));
    CHECK_FALSE(selection.includes(FileEntry{"/root/other.pdf", "other.pdf", FileType::File}));
}

TEST_CASE("Analysis selection applies image-only mode to cached and scanned files")
{
    const auto selection = make_selection(true, false, "");
    CHECK(selection.includes(cached("/root", "photo.jpg")));
    CHECK_FALSE(selection.includes(cached("/root", "paper.pdf")));
    CHECK_FALSE(selection.includes(FileEntry{"/root/folder", "folder", FileType::Directory}));
    CHECK(selection.includes(FileEntry{"/root/photo.jpg", "photo.jpg", FileType::File}));
    CHECK_FALSE(selection.includes(FileEntry{"/root/paper.pdf", "paper.pdf", FileType::File}));
}

TEST_CASE("Analysis selection keeps same-named recursive entries distinct")
{
    const auto selection = make_selection(false, false, "/root/nested/sample.pdf");
    std::vector<FileEntry> scanned{
        {"/root/sample.pdf", "sample.pdf", FileType::File},
        {"/root/nested/sample.pdf", "sample.pdf", FileType::File}};
    std::vector<CategorizedFile> cached_entries{
        cached("/root", "sample.pdf"), cached("/root/nested", "sample.pdf")};

    selection.retain(scanned);
    selection.retain(cached_entries);

    REQUIRE(scanned.size() == 1);
    CHECK(scanned.front().full_path == "/root/nested/sample.pdf");
    REQUIRE(cached_entries.size() == 1);
    CHECK(cached_entries.front().file_path == "/root/nested");
}

TEST_CASE("Analysis selection excludes unselected entries from final review scan")
{
    const auto selection = make_selection(false, false, "/root/selected.pdf");
    std::vector<FileEntry> final_scan{
        {"/root/selected.pdf", "selected.pdf", FileType::File},
        {"/root/other.pdf", "other.pdf", FileType::File}};

    selection.retain(final_scan);

    REQUIRE(final_scan.size() == 1);
    CHECK(final_scan.front().file_name == "selected.pdf");
}

TEST_CASE("Analysis selection preserves UTF-8 paths for cached file choice")
{
    const std::string name = "旅行.pdf";
    const auto selection = make_selection(false, false, "/root/" + name);

    CHECK(selection.includes(cached("/root", name)));
    CHECK_FALSE(selection.includes(cached("/root/nested", name)));
}
