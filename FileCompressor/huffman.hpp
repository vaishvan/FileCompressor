#pragma once
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include "compression_error.hpp"

struct HuffmanNode {
    char data;
    unsigned freq;
    HuffmanNode* left, * right;

    HuffmanNode(char data, unsigned freq)
        : data(data), freq(freq), left(nullptr), right(nullptr) {
    }

    ~HuffmanNode() {
        delete left;
        delete right;
    }
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
    void validateCompressedData(const std::string& input);

    void serializeTree(HuffmanNode* root, std::string& output);
    HuffmanNode* deserializeTree(const std::string& data, size_t& pos);

    HuffmanNode* buildTree(const std::string& input);

public:
    std::string compress(const std::string& input);
    std::string decompress(const std::string& input);
};