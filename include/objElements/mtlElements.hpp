#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "parsingUtils.hpp"

enum MtlElemType {
    NEW_MAT,
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    SPECULAR_EXPONENT,
    DISSOLVE,
    TRANSPARENT,
    TRANSMISSION_FILTER,
    ILLUMINATION,
    AMBIENT_TEXTURE,
    DIFFUSE_TEXTURE,
    SPECULAR_TEXTURE,
    SPECULAR_EXPONENT_TEXTURE,
    TRANSMISSION_FILTER_TEXTURE,
    BUMP_TEXTURE,
    DISPLACEMENT_TEXTURE,
    DECAL_TEXTURE,
    REFLECTION_TEXTURE,
    REFRACTION_TEXTURE,
    UNKNOWN_
};

const std::unordered_map<std::string, MtlElemType> mtlElemMap = {
    {"newmtl", NEW_MAT},
    {"Ka", AMBIENT},
    {"Kd", DIFFUSE},
    {"Ks", SPECULAR},
    {"Ns", SPECULAR_EXPONENT},
    {"d", DISSOLVE},
    {"Tr", TRANSPARENT},
    {"Tf", TRANSMISSION_FILTER},
    {"illum", ILLUMINATION},
    {"map_Ka", AMBIENT_TEXTURE},
    {"map_Kd", DIFFUSE_TEXTURE},
    {"map_Ks", SPECULAR_TEXTURE},
    {"map_Ns", SPECULAR_EXPONENT_TEXTURE},
    {"map_Tf", TRANSMISSION_FILTER_TEXTURE},
    {"map_bump", BUMP_TEXTURE},
    {"map_d", DISPLACEMENT_TEXTURE},
    {"decal", DECAL_TEXTURE},
    {"refl", REFLECTION_TEXTURE},
    {"refr", REFRACTION_TEXTURE}
};

MtlElemType getMtlElemType(const std::string& prefix) {
    auto it = mtlElemMap.find(prefix);
    if (it != mtlElemMap.end()) {
        return it->second;
    }
    return UNKNOWN_;
}

const std::unordered_map<MtlElemType, size_t> mtlElemSize = {
    {NEW_MAT, 1},
    {AMBIENT, 3},
    {DIFFUSE, 3},
    {SPECULAR, 3},
    {SPECULAR_EXPONENT, 1},
    {DISSOLVE, 1},
    {TRANSPARENT, 1},
    {TRANSMISSION_FILTER, 3},
    {ILLUMINATION, 1},
    {AMBIENT_TEXTURE, 1},
    {DIFFUSE_TEXTURE, 1},
    {SPECULAR_TEXTURE, 1},
    {SPECULAR_EXPONENT_TEXTURE, 1},
    {TRANSMISSION_FILTER_TEXTURE, 1},
    {BUMP_TEXTURE, 1},
    {DISPLACEMENT_TEXTURE, 1},
    {DECAL_TEXTURE, 1},
    {REFLECTION_TEXTURE, 1},
    {REFRACTION_TEXTURE, 1}
};

struct Bound {
    float _lower, _upper;

    Bound(float lower, float upper) : _lower(lower), _upper(upper) {}
    ~Bound() {}

    bool isInside(float value) const {
        return value >= _lower && value <= _upper;
    }
};

const std::unordered_map<MtlElemType, Bound> mtlElemBounds = {
    {AMBIENT, Bound(0, 1)},
    {DIFFUSE, Bound(0, 1)},
    {SPECULAR, Bound(0, 1)},
    {SPECULAR_EXPONENT, Bound(0, 1000)},
    {DISSOLVE, Bound(0, 1)},
    {TRANSPARENT, Bound(0, 1)},
    {TRANSMISSION_FILTER, Bound(0, 1)},
    {ILLUMINATION, Bound(0, 10)},
    {AMBIENT_TEXTURE, Bound(0, 0)},
    {DIFFUSE_TEXTURE, Bound(0, 0)},
    {SPECULAR_TEXTURE, Bound(0, 0)},
    {SPECULAR_EXPONENT_TEXTURE, Bound(0, 0)},
    {TRANSMISSION_FILTER_TEXTURE, Bound(0, 0)},
    {BUMP_TEXTURE, Bound(0, 0)},
    {DISPLACEMENT_TEXTURE, Bound(0, 0)},
    {DECAL_TEXTURE, Bound(0, 0)},
    {REFLECTION_TEXTURE, Bound(0, 0)},
    {REFRACTION_TEXTURE, Bound(0, 0)}
};

bool checkElemSize(MtlElemType elemType, size_t size, size_t lineNb) {
    if (size != mtlElemSize.at(elemType)) {
        std::cerr << "Invalid number of arguments for " << elemType << ": " << lineNb << std::endl;
        return false;
    }
    return true;
}

bool checkElemBounds(MtlElemType elemType, float value, size_t lineNb) {
    if (!mtlElemBounds.at(elemType).isInside(value)) {
        std::cerr << "Invalid value for " << elemType << ": " << lineNb << std::endl;
        return false;
    }
    return true;
}

bool checkElemBounds(MtlElemType elemType, std::vector<float> &values, size_t lineNb) {
    for (auto &value : values) {
        if (!mtlElemBounds.at(elemType).isInside(value)) {
            std::cerr << "Invalid value for " << elemType << ": " << lineNb << std::endl;
            return false;
        }
    }
    return true;
}