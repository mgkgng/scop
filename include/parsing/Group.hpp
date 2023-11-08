#pragma once

#include "sharedParsing.hpp"
#include "Face.hpp"
#include "Line.hpp"

namespace scop {

struct Group {
    std::string name;
    std::vector<Face> faces;
    std::vector<Line> lines;

    Group() = default;
    Group(std::string const &name) : name(name) {}

    void addFace(std::vector<std::string> const &tokens, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts, std::string &materialName, int &smoothingGroup) {
        faces.emplace_back(tokens, lineNb, geometryElemCounts, materialName, smoothingGroup);
    }

    void addLine(std::vector<std::string> const &tokens, size_t &lineNb, size_t &vertexCount) {
        lines.emplace_back(tokens, lineNb, vertexCount);
    }

    friend std::ostream& operator<<(std::ostream& os, const Group& g) {
        if (!g.name.empty())
            os << "g " << g.name << std::endl;

        std::string currMatName = "";
        int currSmoothingGroup = -1;
        for (const auto& f : g.faces) {
            if (f.materialName != currMatName) {
                currMatName = f.materialName;
                os << "usemtl " << currMatName << std::endl;
            }
            if (f.smoothingGroup != currSmoothingGroup) {
                currSmoothingGroup = f.smoothingGroup;
                os << "s " << (currSmoothingGroup == 0 ? "off" : std::to_string(currSmoothingGroup)) << std::endl;
            }
            os << f << std::endl;
        }
        return os;
    }
};

}