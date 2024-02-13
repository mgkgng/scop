#pragma once

struct MeshVertex {
    std::array<float, 4> position;
    std::array<float, 3> normal;
    std::array<float, 3> texCoord;
};