#pragma once
#include <string>
#include <unordered_map>
#include <queue>

struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode* left, * right;

    HuffmanNode(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

struct CompareNode {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

class HuffmanCompressor {
private:
    std::unordered_map<char, std::string> huffmanCodes;
    void generateHuffmanCodes(HuffmanNode* root, std::string str);

public:
    std::string compress(const std::string& input);
    std::string decompress(const std::string& input); // To be implemented
};
