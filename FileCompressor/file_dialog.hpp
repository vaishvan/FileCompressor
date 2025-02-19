#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <imgui.h>

class FileDialog {
private:
    bool show_dialog = false;
    std::string current_path;
    std::string selected_file;
    std::vector<std::filesystem::directory_entry> current_directory_files; // need to modify this;
                                                                           // cant open the dialog again after once

public:
    FileDialog();
    void show();
    bool is_shown() const { return show_dialog; }
    void open();
    void close();
    const std::string& get_selected_file() const { return selected_file; }
    void refresh_directory();
};