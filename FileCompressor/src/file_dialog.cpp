#include "file_dialog.hpp"
#include <algorithm>

FileDialog::FileDialog() {
    current_path = std::filesystem::current_path().string();
    refresh_directory();
}

void FileDialog::refresh_directory() {
    current_directory_files.clear();
    for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
        current_directory_files.push_back(entry);
    }

    // Sort: directories first, then files
    std::sort(current_directory_files.begin(), current_directory_files.end(),
        [](const auto& a, const auto& b) {
            if (a.is_directory() != b.is_directory()) {
                return a.is_directory();
            }
            return a.path().filename() < b.path().filename();
        });
}

void FileDialog::show() {
    if (!show_dialog) return;

    ImGui::Begin("File Browser", &show_dialog);

    // Current path display
    ImGui::Text("Current Path: %s", current_path.c_str());

    // Go up button
    if (ImGui::Button("..")) {
        std::filesystem::path path(current_path);
        if (path.has_parent_path()) {
            current_path = path.parent_path().string();
            refresh_directory();
        }
    }

    // File list
    ImGui::BeginChild("Files", ImVec2(0, 300), true);
    for (const auto& entry : current_directory_files) {
        const auto& path = entry.path();
        std::string filename = path.filename().string();
        std::string label = entry.is_directory() ? "[Dir] " + filename : filename;

        if (ImGui::Selectable(label.c_str(), false)) {
            if (entry.is_directory()) {
                current_path = path.string();
                refresh_directory();
            }
            else {
                selected_file = path.string();
                show_dialog = false;
            }
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void FileDialog::open() {
    show_dialog = true;
    refresh_directory();
}

void FileDialog::close() {
    show_dialog = false;
}
