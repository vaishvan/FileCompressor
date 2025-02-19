#include "huffman.hpp"

void HuffmanCompressor::generateHuffmanCodes(HuffmanNode* root, std::string str) {
    if (!root) return;
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = str;
    }
    generateHuffmanCodes(root->left, str + "0");
    generateHuffmanCodes(root->right, str + "1");
}

std::string HuffmanCompressor::compress(const std::string& input) {
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

    huffmanCodes.clear();
    generateHuffmanCodes(pq.top(), "");

    std::string compressed;
    for (char c : input) {
        compressed += huffmanCodes[c];
    }

    return compressed;
}