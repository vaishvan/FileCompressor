#pragma once
#include <string>

class RLECompressor {
public:
    std::string compress(const std::string& input);
    std::string decompress(const std::string& input); // To be implemented
};
