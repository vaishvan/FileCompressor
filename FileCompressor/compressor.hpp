#pragma once
#include <string>
#pragma once
#include <vector>
#include <optional>
#include "huffman.hpp"
#include "rle.hpp"
#include "file_dialog.hpp"

class FileCompressor {
private:
    std::vector<std::string> selectedFiles;
    int currentAlgorithm = 0;
    const char* algorithms[2] = { "Huffman Compression", "RLE Compression" };
    char statusMessage[256] = "";
    FileDialog fileDialog;
    HuffmanCompressor huffman;
    RLECompressor rle;
    bool isDecompressing = false;

    void compressFile(const std::string& filename);
    void decompressFile(const std::string& filename);
    void clearFileDialog();

public:
    void renderGUI();
};