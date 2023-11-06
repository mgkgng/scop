#pragma once

#include "sharedParsing.hpp"

namespace scop {

struct Line {
    std::vector<size_t> vertexIndices;

    Line(std::istringstream& iss, size_t &lineNb, size_t &vertexCount) {
        size_t index;
        while (iss >> index) {
            if (index > vertexCount)
                throw std::runtime_error("Error parsing line element: vertex index out of range. line: " + std::to_string(lineNb));
            vertexIndices.push_back(index);
        }
        if (iss.fail())
            throw std::runtime_error("Error parsing line element: invalid vertex index. line: " + std::to_string(lineNb));
        if (vertexIndices.size() < 2)
            throw std::runtime_error("Error parsing line element: not enough vertices. line: " + std::to_string(lineNb));
    }
    
    // Utility to print or serialize the line.
    friend std::ostream& operator<<(std::ostream& os, const Line& l) {
        os << "l";
        for (auto index : l.vertexIndices) {
            os << " " << index;
        }
        return os;
    }

};

}