#pragma once

#include "obj.hpp"

struct Normal {
    float x, y, z;

    Normal(std::vector<std::string> const &tokens, size_t &lineNb) {
        if (tokens.size() != 4) {
            std::cerr << "Invalid normal: " << lineNb << std::endl;
            throw std::exception();
        }
        x = std::stof(tokens[1]);
        y = std::stof(tokens[2]);
        z = std::stof(tokens[3]);
    }

    friend std::ostream& operator<<(std::ostream& os, const Normal& n) {
        os << "vn " << n.x << " " << n.y << " " << n.z;
        return os;
    }
};