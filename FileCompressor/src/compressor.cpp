#include "compressor.hpp"
#include <fstream>
#include <iostream>

void FileCompressor::compressFile(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        snprintf(statusMessage, sizeof(statusMessage), "Failed to open file: %s", filename.c_str());
        return;
    }

    std::string content((std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());
    inFile.close();

    std::string compressed;
    if (currentAlgorithm == 0) {
        compressed = huffman.compress(content);
    }
    else {
        compressed = rle.compress(content);
    }

    std::string outFilename = filename + ".compressed";
    std::ofstream outFile(outFilename, std::ios::binary);
    if (!outFile) {
        snprintf(statusMessage, sizeof(statusMessage), "Failed to create compressed file");
        return;
    }

    outFile.write(compressed.c_str(), compressed.size());
    outFile.close();

    snprintf(statusMessage, sizeof(statusMessage), "Successfully compressed: %s", filename.c_str());
}

void FileCompressor::renderGUI() {
    ImGui::Begin("File Compressor");

    // Algorithm selection
    ImGui::Combo("Compression Algorithm", &currentAlgorithm, algorithms, IM_ARRAYSIZE(algorithms));

    // File selection button
    if (ImGui::Button("Select File")) {
        fileDialog.open();
    }

    // Show file dialog if active
    if (fileDialog.is_shown()) {
        fileDialog.show();
        if (!fileDialog.get_selected_file().empty()) {
            selectedFiles.push_back(fileDialog.get_selected_file());
        }
    }

    // Selected files list
    ImGui::Text("Selected Files:");
    for (size_t i = 0; i < selectedFiles.size(); i++) {
        ImGui::Text("%s", selectedFiles[i].c_str());
        ImGui::SameLine();
        if (ImGui::Button(("Remove##" + std::to_string(i)).c_str())) {
            selectedFiles.erase(selectedFiles.begin() + i);
            i--;
        }
    }

    // Compress button
    if (ImGui::Button("Compress Files") && !selectedFiles.empty()) {
        for (const auto& file : selectedFiles) {
            compressFile(file);
        }
    }

    // Status message
    ImGui::Text("%s", statusMessage);

    ImGui::End();
}