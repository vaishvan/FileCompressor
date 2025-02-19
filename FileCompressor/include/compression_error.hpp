#pragma once
#include <stdexcept>
#include <string>

class CompressionError : public std::runtime_error {
public:
    explicit CompressionError(const std::string& message)
        : std::runtime_error(message) {}
};