#pragma once
#include <string>

class RLECompressor {
public:
    std::string compress(const std::string& input);

    std::string decompress(const std::string& input) {
        std::string decompressed;
        std::string count;

        for (size_t i = 0; i < input.length(); i++) {
            if (std::isdigit(input[i])) {
                count += input[i];
            }
            else {
                int repeat = std::stoi(count);
                decompressed.append(repeat, input[i]);
                count.clear();
            }
        }

        return decompressed;
    }
};
