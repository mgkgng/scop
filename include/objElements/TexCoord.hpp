#pragma once

#include "obj.hpp"

struct TexCoord {
    float u, v = 0.f, w = 0.f;
 
    TexCoord(std::vector<std::string> const &tokens, size_t &lineNb) {
        (void)lineNb;
        // if (tokens.size() > 3) {
        //     std::cerr << "Invalid texcoord: " << lineNb << std::endl;
        //     throw std::exception();
        // }
        u = std::stof(tokens[1]);
        if (tokens.size() > 2) v = std::stof(tokens[2]);
        if (tokens.size() > 3) w = std::stof(tokens[3]);
    }

    friend std::ostream& operator<<(std::ostream& os, const TexCoord& t) {
        os << "vt " << t.u << " " << t.v;
        return os;
    }
};
