#pragma once

#include "obj.hpp"

namespace scop {

struct Line {
    std::vector<size_t> vertexIndices;

    Line(std::vector<std::string> const &tokens, size_t &lineNb, size_t &vertexCount) {
        if (tokens.size() < 3) {
            std::cerr << "Invalid line: " << lineNb << std::endl;
            throw std::exception();
        }
        for (size_t i = 1; i < tokens.size(); ++i) {
            int index = std::stoi(tokens[i]);
            if (index > (int) vertexCount)
                throw std::runtime_error("Error parsing line element: vertex index out of range. line: " + std::to_string(lineNb));
            vertexIndices.push_back(index);
        }
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