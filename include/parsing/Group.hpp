#pragma once

#include "sharedParsing.hpp"
#include "Face.hpp"

namespace scop {

struct Group {
    std::string name;
    std::vector<Face> faces;

    Group() = default;
    Group(std::string const &name) : name(name) {}

    void addFace(std::istringstream &iss, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts) {
        faces.emplace_back(iss, lineNb, geometryElemCounts);
    }

    friend std::ostream& operator<<(std::ostream& os, const Group& g) {
        os << "g " << g.name << std::endl;
        for (const auto& f : g.faces)
            os << f << std::endl;
        return os;
    }
};

}