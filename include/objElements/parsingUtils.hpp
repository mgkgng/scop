#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <variant>

using IntOrFloat = std::variant<int, float>;

struct Bound {
    IntOrFloat _lower, _upper;

    Bound(IntOrFloat lower, IntOrFloat upper) : _lower(lower), _upper(upper) {}
    ~Bound() {}

    bool isInside(IntOrFloat value) const {
        return value >= _lower && value <= _upper;
    }
};

std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty()) { // Optionally skip empty tokens
            tokens.push_back(token);
        }
    }
    return tokens;
}

std::pair<bool, float> isFloat(const std::string& s) {
    std::istringstream iss(s);
    float f;
    iss >> std::noskipws >> f;
    return std::make_pair(iss.eof() && !iss.fail(), f);
}

std::pair<bool, int> isInteger(const std::string& s) {
    std::istringstream iss(s);
    int i;
    iss >> std::noskipws >> i;
    return std::make_pair(iss.eof() && !iss.fail(), i);
}