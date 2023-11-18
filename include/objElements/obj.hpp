#pragma once

#include <iostream>
#include <unordered_map>

#include "parsingUtils.hpp"

enum ElemType {
    VERTEX,
    TEXCOORD,
    NORMAL,
    FACE,
    LINE,
    OBJ,
    GROUP,
    SMOOTHING_GROUP,
    MATLIB,
    USEMTL,
    UNKNOWN // default case
};

const std::unordered_map<std::string, ElemType> elemMap = {
    {"v", VERTEX},
    {"vt", TEXCOORD},
    {"vn", NORMAL},
    {"f", FACE},
    {"l", LINE},
    {"o", OBJ},
    {"g", GROUP},
    {"s", SMOOTHING_GROUP},
    {"mtllib", MATLIB},
    {"usemtl", USEMTL}
};

ElemType getElemType(const std::string& prefix) {
    auto it = elemMap.find(prefix);
    if (it != elemMap.end()) {
        return it->second;
    }
    return UNKNOWN;
}

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