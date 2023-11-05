#pragma once

#include "sharedParsing.hpp"

namespace scop {

struct Normal {
    float x, y, z;

    Normal(std::istringstream &iss, size_t &lineNb) {
        iss >> x >> y >> z;
        checkIssParse(iss, lineNb, "Failed to parse normal");
    }

    friend std::ostream& operator<<(std::ostream& os, const Normal& n) {
        os << "vn " << n.x << " " << n.y << " " << n.z;
        return os;
    }
};

}