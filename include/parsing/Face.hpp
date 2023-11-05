#pragma once

#include "sharedParsing.hpp"

#include <vector>
#include <array>

namespace scop {

struct Face {
    std::vector<int> vertexIndices, textureIndices, normalIndices;

    enum VertexType { VERTEX_ONLY, VERTEX_TEX, VERTEX_NORMAL, VERTEX_TEX_NORMAL };

    Face(std::istringstream &iss, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts) {
        if (geometryElemCounts[VERTEX_INX] == 0)
            throw std::runtime_error("Error parsing face element: vertex index not defined. line: " + std::to_string(lineNb));

        std::string vertexStr;
        VertexType type = VERTEX_ONLY;
        bool firstVertex = true;

        while (iss >> vertexStr) {
            std::istringstream vertexStream(vertexStr);
            int vIndex, vtIndex, vnIndex;
            char slash;
            
            // Parse vertex index
            if (!(vertexStream >> vIndex))
                throw std::runtime_error("Error parsing face element: vertex index. line: " + std::to_string(lineNb));
            vertexIndices.push_back(vIndex > 0 ? vIndex - 1 : geometryElemCounts[VERTEX_INX] + vIndex);

            // Check for texture coordinate index
            if (vertexStream.peek() == '/') {
                vertexStream >> slash;
                
                // Check if texture coordinates are provided
                if (vertexStream.peek() != '/') {
                    if (!(vertexStream >> vtIndex))
                        throw std::runtime_error("Error parsing face element: texture index. line: " + std::to_string(lineNb));
                    if (geometryElemCounts[TEX_INX] == 0)
                        throw std::runtime_error("Error parsing face element: texture index not defined. line: " + std::to_string(lineNb));
                    textureIndices.push_back(vtIndex > 0 ? vtIndex - 1 : geometryElemCounts[TEX_INX] + vtIndex);
                }
                
                // Check for normal index
                if (vertexStream.peek() == '/') {
                    vertexStream >> slash;
                    if (!(vertexStream >> vnIndex))
                        throw std::runtime_error("Error parsing face element: normal index. line: " + std::to_string(lineNb));
                    if (geometryElemCounts[NORMAL_INX] == 0)
                        throw std::runtime_error("Error parsing face element: normal index not defined. line: " + std::to_string(lineNb));
                    normalIndices.push_back(vnIndex > 0 ? vnIndex - 1 : geometryElemCounts[NORMAL_INX] + vnIndex);
                }
            }

            // Check if all vertices have the same type
            if (firstVertex) {
                if (textureIndices.empty() && normalIndices.empty())
                    type = VERTEX_ONLY;
                else if (!textureIndices.empty() && normalIndices.empty())
                    type = VERTEX_TEX;
                else if (textureIndices.empty() && !normalIndices.empty())
                    type = VERTEX_NORMAL;
                else
                    type = VERTEX_TEX_NORMAL;
                firstVertex = false;
            } else {
                if (type == VERTEX_ONLY && (!textureIndices.empty() || !normalIndices.empty()))
                    throw std::runtime_error("Error parsing face element: vertex type mismatch. line: " + std::to_string(lineNb));
                else if (type == VERTEX_TEX && (!textureIndices.empty() || normalIndices.empty()))
                    throw std::runtime_error("Error parsing face element: vertex type mismatch. line: " + std::to_string(lineNb));
                else if (type == VERTEX_NORMAL && (textureIndices.empty() || !normalIndices.empty()))
                    throw std::runtime_error("Error parsing face element: vertex type mismatch. line: " + std::to_string(lineNb));
                else if (type == VERTEX_TEX_NORMAL && (textureIndices.empty() || normalIndices.empty()))
                    throw std::runtime_error("Error parsing face element: vertex type mismatch. line: " + std::to_string(lineNb));
            }
        }

        if (vertexIndices.empty())
            throw std::runtime_error("Error parsing face element: vertex index not defined. line: " + std::to_string(lineNb));
    }

    friend std::ostream& operator<<(std::ostream& os, const Face& f) {
        os << "f";
        for (size_t i = 0; i < f.vertexIndices.size(); ++i) {
            os << " " << (f.vertexIndices[i] + 1); // OBJ indices are 1-based
            if (!f.textureIndices.empty() || !f.normalIndices.empty()) {
                os << "/";
                if (i < f.textureIndices.size()) {
                    os << (f.textureIndices[i] + 1);
                }
                if (!f.normalIndices.empty()) {
                    os << "/";
                    if (i < f.normalIndices.size()) {
                        os << (f.normalIndices[i] + 1);
                    }
                }
            }
        }
        return os;
    }
};

}