#pragma once

#include "sharedParsing.hpp"

namespace scop {

struct Vertex {
    float x, y, z;

    Vertex(std::istringstream &iss, size_t &lineNb) {
        iss >> x >> y >> z;
        checkIssParse(iss, lineNb, "Failed to parse vertex");
    }

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << "v " << v.x << " " << v.y << " " << v.z;
        return os;
    }
};

}