#pragma once
#include <string>
#include <vector>
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

    void compressFile(const std::string& filename);

public:
    void renderGUI();
};