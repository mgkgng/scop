#pragma once

#include "sharedParsing.hpp"

namespace scop {

struct TexCoord {
    float u, v;

    TexCoord(std::istringstream &iss, size_t &lineNb) {
        iss >> u >> v;
        checkIssParse(iss, lineNb, "Failed to parse tex coord");
    }

    friend std::ostream& operator<<(std::ostream& os, const TexCoord& t) {
        os << "vt " << t.u << " " << t.v;
        return os;
    }
};

}