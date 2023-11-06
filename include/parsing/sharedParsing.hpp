#pragma once

#include <iostream>
#include <sstream>
#include <vector>

#define VERTEX "v"
#define TEXCOORD "vt"
#define NORMAL "vn"
#define FACE "f"
#define LINE "l"
#define OBJ "o"
#define GROUP "g"
#define SMOOTHING_GROUP "s"
#define MATLIB "mtllib"
#define USRMTL "usemtl"

#define CHECK(s, prefix) s == prefix

#define VERTEX_TYPE 0
#define FACE_TYPE 1
#define LINE_TYPE 2

enum VertexIndex { VERTEX_INX, TEX_INX, NORMAL_INX };

// OBJ file structure:
// Object (o)
// │
// ├── Vertices (v)
// ├── Texture Coordinates (vt)
// ├── Normals (vn)
// │
// └── Group (g)
//     │
//     └── Faces (f)
//         │
//         ├── Material Name (usemtl)
//         └── Smoothing Group (s)

void checkIssParse(std::istringstream &iss, size_t &lineNb, std::string const &errorMsg) {
    if (iss.fail()) {
        std::cerr << "Failed to parse line " << lineNb << std::endl;
        throw std::runtime_error(errorMsg);
    }

    char extra;
    iss >> std::noskipws >> extra;
    if (!iss.eof()) {
        std::cerr << "Wrong format for line " << lineNb << std::endl;
        throw std::runtime_error(errorMsg);
    }
}