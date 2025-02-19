#include "compressor.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

void FileCompressor::clearFileDialog() {
    fileDialog.close();
}

void FileCompressor::compressFile(const std::string& filename) {
    try {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            throw CompressionError("Failed to open file: " + filename);
        }

        std::string content((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();

        std::string compressed;
        std::filesystem::path filepath(filename);
        std::string extension = currentAlgorithm == 0 ? ".huf" : ".rle";

        if (currentAlgorithm == 0) {
            compressed = huffman.compress(content);
        }
        else {
            compressed = rle.compress(content);
        }

        std::string originalExt = filepath.extension().string();
        if (originalExt.empty()) originalExt = ".txt";  // Default extension if none

        std::string outFilename = filepath.string() + extension;
        std::ofstream outFile(outFilename, std::ios::binary);
        if (!outFile) {
            throw CompressionError("Failed to create compressed file");
        }

        char extLength = static_cast<char>(originalExt.length());
        outFile.write(&extLength, 1);
        outFile.write(originalExt.c_str(), originalExt.length());

        outFile.write(compressed.c_str(), compressed.size());
        outFile.close();

        snprintf(statusMessage, sizeof(statusMessage), "Successfully compressed: %s", filename.c_str());
    }
    catch (const CompressionError& e) {
        snprintf(statusMessage, sizeof(statusMessage), "Compression error: %s", e.what());
    }
    catch (const std::exception& e) {
        snprintf(statusMessage, sizeof(statusMessage), "Unexpected error: %s", e.what());
    }
}

void FileCompressor::decompressFile(const std::string& filename) {
    try {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            throw CompressionError("Failed to open file: " + filename);
        }

        char extLength;
        inFile.read(&extLength, 1);
        std::string originalExt(extLength, '\0');
        inFile.read(&originalExt[0], extLength);

        std::string content((std::istreambuf_iterator<char>(inFile)),
            std::istreambuf_iterator<char>());
        inFile.close();

        std::string decompressed;
        std::filesystem::path filepath(filename);
        std::string extension = filepath.extension().string();

        try {
            if (extension == ".huf") {
                decompressed = huffman.decompress(content);
                currentAlgorithm = 0;
            }
            else if (extension == ".rle") {
                decompressed = rle.decompress(content);
                currentAlgorithm = 1;
            }
            else {
                throw CompressionError("Unknown compression format");
            }
        }
        catch (const CompressionError& e) {
            throw CompressionError("File is corrupted: " + std::string(e.what()));
        }

        std::filesystem::path outPath = filepath.parent_path() /
            (filepath.stem().string() + "_decompressed" + originalExt);

        std::ofstream outFile(outPath.string(), std::ios::binary);
        if (!outFile) {
            throw CompressionError("Failed to create output file: " + outPath.string());
        }

        outFile.write(decompressed.c_str(), decompressed.size());
        outFile.close();

        snprintf(statusMessage, sizeof(statusMessage), "Successfully decompressed: %s", outPath.string().c_str());
    }
    catch (const CompressionError& e) {
        snprintf(statusMessage, sizeof(statusMessage), "Decompression error: %s", e.what());
    }
    catch (const std::exception& e) {
        snprintf(statusMessage, sizeof(statusMessage), "Unexpected error: %s", e.what());
    }
}

void FileCompressor::renderGUI() {
    ImGui::Begin("File Compressor");

    if (ImGui::RadioButton("Compress", !isDecompressing)) {
        isDecompressing = false;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Decompress", isDecompressing)) {
        isDecompressing = true;
    }

    if (!isDecompressing) {
        ImGui::Combo("Compression Algorithm", &currentAlgorithm, algorithms, IM_ARRAYSIZE(algorithms));
    }

    if (ImGui::Button("Select Files")) {
        fileDialog.open();
    }

    if (fileDialog.is_shown()) {
        fileDialog.show();
        if (!fileDialog.get_selected_file().empty()) {
            // Check if file is already in the list
            std::string selected = fileDialog.get_selected_file();
            if (std::find(selectedFiles.begin(), selectedFiles.end(), selected) == selectedFiles.end()) {
                selectedFiles.push_back(selected);
            }
            clearFileDialog();
        }
    }

    if (!selectedFiles.empty()) {
        ImGui::Text("Selected Files:");
        ImGui::BeginChild("Files List", ImVec2(0, 200), true);
        for (size_t i = 0; i < selectedFiles.size(); i++) {
            ImGui::PushID(static_cast<int>(i));

            std::filesystem::path filepath(selectedFiles[i]);
            std::string filename = filepath.filename().string();
            ImGui::Text("%s", filename.c_str());

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("%s", selectedFiles[i].c_str());
            }

            ImGui::SameLine();
            if (ImGui::Button("Remove")) {
                selectedFiles.erase(selectedFiles.begin() + i);
                i--;
            }

            ImGui::PopID();
        }
        ImGui::EndChild();

        if (isDecompressing) {
            if (ImGui::Button("Decompress Files")) {
                for (const auto& file : selectedFiles) {
                    decompressFile(file);
                }
            }
        }
        else {
            if (ImGui::Button("Compress Files")) {
                for (const auto& file : selectedFiles) {
                    compressFile(file);
                }
            }
        }
    }

    ImGui::TextWrapped("%s", statusMessage);

    ImGui::End();
}