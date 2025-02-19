#include "huffman.hpp"
#include <queue>
#include <stdexcept>

void HuffmanCompressor::generateHuffmanCodes(HuffmanNode* root, std::string str) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = str;
    }
    generateHuffmanCodes(root->left, str + "0");
    generateHuffmanCodes(root->right, str + "1");
}

void HuffmanCompressor::serializeTree(HuffmanNode* root, std::string& output) {
    if (!root) return;

    if (!root->left && !root->right) {
        output += "L";  // L for leaf
        output += root->data;
    }
    else {
        output += "N";  // N for internal node
        serializeTree(root->left, output);
        serializeTree(root->right, output);
    }
}

HuffmanNode* HuffmanCompressor::deserializeTree(const std::string& data, size_t& pos) {
    if (pos >= data.length()) {
        throw CompressionError("Corrupted tree data");
    }

    if (data[pos] == 'L') {
        pos++;
        if (pos >= data.length()) {
            throw CompressionError("Corrupted leaf node");
        }
        HuffmanNode* node = new HuffmanNode(data[pos], 0);
        pos++;
        return node;
    }

    if (data[pos] == 'N') {
        pos++;
        HuffmanNode* node = new HuffmanNode('\0', 0);
        node->left = deserializeTree(data, pos);
        node->right = deserializeTree(data, pos);
        return node;
    }

    throw CompressionError("Invalid tree structure");
}

HuffmanNode* HuffmanCompressor::buildTree(const std::string& input) {
    std::unordered_map<char, unsigned> freq;
    for (char c : input) freq[c]++;

    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNode> pq;
    for (auto pair : freq) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* top = new HuffmanNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        pq.push(top);
    }

    return pq.empty() ? nullptr : pq.top();
}

std::string HuffmanCompressor::compress(const std::string& input) {
    if (input.empty()) {
        throw CompressionError("Cannot compress empty input");
    }

    HuffmanNode* root = buildTree(input);
    if (!root) {
        throw CompressionError("Failed to build Huffman tree");
    }

    huffmanCodes.clear();
    generateHuffmanCodes(root, "");

    std::string output = "H"; // Header marker
    serializeTree(root, output);
    output += "D"; // Data marker

    for (char c : input) {
        if (huffmanCodes.find(c) == huffmanCodes.end()) {
            throw CompressionError("Invalid character encountered");
        }
        output += huffmanCodes[c];
    }

    delete root;
    return output;
}

void HuffmanCompressor::validateCompressedData(const std::string& input) {
    if (input.empty() || input[0] != 'H') {
        throw CompressionError("Invalid compressed data format");
    }

    size_t dataMarker = input.find('D');
    if (dataMarker == std::string::npos) {
        throw CompressionError("Corrupted data structure");
    }
}

std::string HuffmanCompressor::decompress(const std::string& input) {
    try {
        validateCompressedData(input);

        size_t pos = 1;
        HuffmanNode* root = deserializeTree(input, pos);

        if (pos >= input.length() || input[pos] != 'D') {
            throw CompressionError("Invalid data marker");
        }
        pos++;

        std::string decompressed;
        HuffmanNode* current = root;

        for (size_t i = pos; i < input.length(); i++) {
            if (input[i] != '0' && input[i] != '1') {
                throw CompressionError("Invalid bit in compressed data");
            }

            current = (input[i] == '0') ? current->left : current->right;

            if (!current) {
                throw CompressionError("Corrupted data: invalid tree traversal");
            }

            if (!current->left && !current->right) {
                decompressed += current->data;
                current = root;
            }
        }

        if (current != root) {
            throw CompressionError("Incomplete compressed data");
        }

        delete root;
        return decompressed;
    }
    catch (const std::exception& e) {
        throw CompressionError(std::string("Decompression failed: ") + e.what());
    }
}