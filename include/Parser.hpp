#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

#define VERTEX "v"
#define TEXCOORD "vt"
#define NORMAL "vn"
#define FACE "f"
#define OBJ "o"
#define GROUP "g"
#define SMOOTHING_GROUP "s"
#define MATLIB "mtllib"
#define USRMTL "usemtl"

#define CHECK(s, prefix) s == prefix

#define VERTEX_TYPE 0
#define FACE_TYPE 1

enum VertexIndex { VERTEX_INX, TEX_INX, NORMAL_INX };

namespace scop {

struct Vertex {
    float x, y, z;

    Vertex(std::istringstream &ss, size_t &lineNb) {
        ss >> x >> y >> z;
        if (ss.fail()) {
            std::cerr << "Failed to parse vertex on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse vertex");
        }
        char extra;
        ss >> std::noskipws >> extra;
        if (!ss.eof()) {
            std::cerr << "Failed to parse vertex on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse vertex");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << "v " << v.x << " " << v.y << " " << v.z;
        return os;
    }
};

struct TexCoord {
    float u, v;

    TexCoord(std::istringstream &ss, size_t &lineNb) {
        ss >> u >> v;
        if (ss.fail()) {
            std::cerr << "Failed to parse texCoord on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse texCoord");
        }
        char extra;
        ss >> std::noskipws >> extra;
        if (!ss.eof()) {
            std::cerr << "Failed to parse texCoord on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse texCoord");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const TexCoord& t) {
        os << "vt " << t.u << " " << t.v;
        return os;
    }
};

struct Normal {
    float x, y, z;

    Normal(std::istringstream &ss, size_t &lineNb) {
        ss >> x >> y >> z;
        if (ss.fail()) {
            std::cerr << "Failed to parse normal on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse normal");
        }
        char extra;
        ss >> std::noskipws >> extra;
        if (!ss.eof()) {
            std::cerr << "Failed to parse normal on line " << lineNb << std::endl;
            throw std::runtime_error("Failed to parse normal");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Normal& n) {
        os << "vn " << n.x << " " << n.y << " " << n.z;
        return os;
    }
};

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

class Parser {
    std::vector<Vertex> _vertices;
    std::vector<TexCoord> _texCoords;
    std::vector<Normal> _normals;
    std::vector<Face> _faces;

    public:
        Parser(std::string const &path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << path << std::endl;
                return;
            }

            std::string line;
            size_t lineNb = 1;
            bool vertexDef = false, faceDef = false;
            std::array<size_t, 3> geometryElemCounts{0, 0, 0};

            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') {
                    lineNb++;
                    continue;
                }

                std::istringstream ss(line);
                std::string prefix;
                ss >> prefix;
                
                if (CHECK(prefix, VERTEX)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                    _vertices.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, TEXCOORD)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                    _texCoords.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, NORMAL)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                    _normals.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, FACE)) {
                    if (!faceDef) {
                        geometryElemCounts[VERTEX_INX] = _vertices.size();
                        geometryElemCounts[TEX_INX] = _texCoords.size();
                        geometryElemCounts[NORMAL_INX] = _normals.size();
                    }
                    checkElemOrder(FACE_TYPE, vertexDef, faceDef, lineNb);
                    _faces.emplace_back(ss, lineNb, geometryElemCounts);
                } else if (CHECK(prefix, OBJ)) {
                    std::cout << "Object name" << std::endl;
                } else if (CHECK(prefix, GROUP)) {
                    std::cout << "Group name" << std::endl;
                } else if (CHECK(prefix, SMOOTHING_GROUP)) {
                    std::cout << "Smoothing group" << std::endl;
                } else if (CHECK(prefix, MATLIB)) {
                    std::cout << "Material library" << std::endl;
                } else if (CHECK(prefix, USRMTL)) {
                    std::cout << "Use material" << std::endl;
                } else {
                    std::cerr << "Prefix not suppported: " << prefix << " on line " << lineNb << std::endl;
                    throw std::exception();
                }
                lineNb++;
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const Parser& parser) {
            for (const auto& v : parser._vertices)
                os << v << std::endl;
            for (const auto& tc : parser._texCoords)
                os << tc << std::endl;
            for (const auto& n : parser._normals)
                os << n << std::endl;
            for (const auto& f : parser._faces)
                os << f << std::endl;
            return os;
        }


    private:
        Parser() = delete;

        void checkElemOrder(int type, bool &vertexDef, bool &faceDef, size_t &lineNb) {
            if (type == VERTEX_TYPE) {
                if (faceDef) {
                    std::cerr << "Vertex definition after face definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Vertex definition after face definition");
                }
                vertexDef = true;
            } else {
                if (!vertexDef) {
                    std::cerr << "Face definition before vertex definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Face definition before vertex definition");
                }
                faceDef = true;
            }
        }

        std::vector<unsigned int> _indices;
};

}