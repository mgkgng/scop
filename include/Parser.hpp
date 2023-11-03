#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace scop {

struct Vertex {
    float x, y, z;

    Vertex(std::istringstream &ss, size_t lineNb) {
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

    TexCoord(std::istringstream &ss) {
        ss >> u >> v;
        if (ss.fail()) {
            std::cerr << "Failed to parse texCoord" << std::endl;
            throw std::runtime_error("Failed to parse texCoord");
        }
        char extra;
        ss >> std::noskipws >> extra;
        if (!ss.eof()) {
            std::cerr << "Failed to parse texCoord" << std::endl;
            throw std::runtime_error("Failed to parse texCoord");
        }
    }

};

struct Normal {
    float x, y, z;
};

struct Face {
    unsigned int vertexIndex[3], texCoordIndex[3], normalIndex[3];


};

class Parser {
    public:
        Parser(std::string const &path) {
            parseObj(path);
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

        void parseObj(std::string const &path) {

            std::ifstream file(path);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << path << std::endl;
                return;
            }

            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#')
                    continue;

                std::istringstream ss(line); // create a stream of the line
                std::string prefix; // the first word of the line
                ss >> prefix; // get the first word of the line
                
                if (prefix == "v") {
                    _vertices.emplace_back(ss);
                } else if (prefix == "vt") {
                    _texCoords.emplace_back(ss);
                } else if (prefix == "vn") {
                    _normals.emplace_back(ss);
                } else if (prefix == "f") {
                    _faces.emplace_back(ss);
                } else if (prefix == "s") {

                } else if (prefix == "usemtl") {
                        
                } else if (prefix == "mtllib") {
            
                    
                } else {
                    std::cerr << "Unknown prefix: " << prefix << std::endl;
                    throw std::exception();
                }
            }
        }

    private:
        std::vector<Vertex> _vertices;
        std::vector<TexCoord> _texCoords;
        std::vector<Normal> _normals;
        std::vector<Face> _faces;
        std::vector<unsigned int> _indices;
};

}