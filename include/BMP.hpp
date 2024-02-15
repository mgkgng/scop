#pragma once

#include <vector>

struct BMP {
    unsigned int width, height;
    std::vector<unsigned char> data;
};