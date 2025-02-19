#include "rle.hpp"

std::string RLECompressor::compress(const std::string& input) {
    std::string compressed;
    int count = 1;

    for (size_t i = 0; i < input.length(); i++) {
        if (i + 1 < input.length() && input[i] == input[i + 1]) {
            count++;
        }
        else {
            compressed += std::to_string(count) + input[i];
            count = 1;
        }
    }

    return compressed;
}
