#pragma once

#include "obj.hpp"

namespace scop {

struct Vertex {
    float x, y, z, w = 1.0f;

    Vertex(std::vector<std::string> const &tokens, size_t &lineNb) {
        if (tokens.size() < 4) {
            std::cerr << "Error: Not enough vertex data at line " << lineNb << ".\n";
            throw std::runtime_error("Parsing error: not enough vertex data.");
        }

        if (tokens.size() > 5) {
            std::cerr << "Error: Too much vertex data at line " << lineNb << ".\n";
            throw std::runtime_error("Parsing error: too much vertex data.");
        }

        bool hasWeight = tokens.size() == 5;

        for (int i = 1; i < 4 + hasWeight; i++) {
            if (!isFloat(tokens[i])) {
                std::cerr << "Error: Failed to parse vertex coordinates at line " << lineNb << ".\n";
                throw std::runtime_error("Parsing error: vertex coordinates.");
            }
        }

        x = std::stof(tokens[1]);
        y = std::stof(tokens[2]);
        z = std::stof(tokens[3]);
        if (hasWeight)
            w = std::stof(tokens[4]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << "v " << v.x << " " << v.y << " " << v.z;
        if (v.w != 1.0f)
            os << " " << v.w;
        return os;
    }
};

}