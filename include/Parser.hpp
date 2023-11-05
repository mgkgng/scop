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
};

struct Face {
    std::vector<int> vertexIndices, textureIndices, normalIndices;

    enum VertexType { VERTEX_ONLY, VERTEX_TEX, VERTEX_NORMAL, VERTEX_TEX_NORMAL };
    enum VertexIndex { VERTEX_INX, TEX_INX, NORMAL_INX };

    Face(std::istringstream &iss, size_t &lineNb, std::array<int, 3> &counts) {
        if (counts[VERTEX_INX] == 0)
            throw std::runtime_error("Error parsing face element: vertex index not defined.");

        std::string vertexStr;
        VertexType type = VERTEX_ONLY;
        bool firstVertex = true;

        while (iss >> vertexStr) {
            std::istringstream vertexStream(vertexStr);
            int vIndex = -1, vtIndex = -1, vnIndex = -1;
            char slash;
            
            // Parse vertex index
            if (!(vertexStream >> vIndex))
                throw std::runtime_error("Error parsing face element: vertex index.");
            vertexIndices.push_back(vIndex > 0 ? vIndex - 1 : counts[VERTEX_INX] + vIndex);

            // Check for texture coordinate index
            if (vertexStream.peek() == '/') {
                vertexStream >> slash;
                
                // Check if texture coordinates are provided
                if (vertexStream.peek() != '/') {
                    if (!(vertexStream >> vtIndex))
                        throw std::runtime_error("Error parsing face element: texture index.");
                    if (counts[TEX_INX] == 0)
                        throw std::runtime_error("Error parsing face element: texture index not defined.");
                    textureIndices.push_back(vtIndex > 0 ? vtIndex - 1 : counts[TEX_INX] + vtIndex);
                }
                
                // Check for normal index
                if (vertexStream.peek() == '/') {
                    vertexStream >> slash;
                    if (!(vertexStream >> vnIndex))
                        throw std::runtime_error("Error parsing face element: normal index.");
                    if (counts[NORMAL_INX] == 0)
                        throw std::runtime_error("Error parsing face element: normal index not defined.");
                    normalIndices.push_back(vnIndex > 0 ? vnIndex - 1 : counts[NORMAL_INX] + vnIndex);
                }
            }
        }

        if (vertexIndices.empty())
            throw std::runtime_error("Error parsing face element: vertex index not defined.");
    }
    
};

class Parser {
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
                    std::cout << "Normal" << std::endl;
                } else if (CHECK(prefix, FACE)) {
                    checkElemOrder(FACE_TYPE, vertexDef, faceDef, lineNb);
                    std::cout << "Face" << std::endl;
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

        std::vector<Vertex> const &getVertices() const {
            return _vertices;
        }

        std::vector<TexCoord> const &getTexCoords() const {
            return _texCoords;
        }

        std::vector<Normal> const &getNormals() const {
            return _normals;
        }

        std::vector<Face> const &getFaces() const {
            return _faces;
        }

        std::vector<unsigned int> const &getIndices() const {
            return _indices;
        }

    private:
        Parser() = delete;

        bool checkElemOrder(int type, bool &vertexDef, bool &faceDef, size_t &lineNb) {
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

        std::vector<Vertex> _vertices;
        std::vector<TexCoord> _texCoords;
        std::vector<Normal> _normals;
        std::vector<Face> _faces;
        std::vector<unsigned int> _indices;
};

}