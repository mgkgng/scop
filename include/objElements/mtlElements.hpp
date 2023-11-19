#pragma once

#include <unordered_map>
#include <string>
#include <iostream>
#include <functional>

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
    OPTICAL_DENSITY,
    ILLUMINATION,
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
    {"Ni", OPTICAL_DENSITY},
    {"illum", ILLUMINATION}
};

const std::unordered_map<MtlElemType, size_t> mtlElemSize = {
    {NEW_MAT, 1},
    {AMBIENT, 3},
    {DIFFUSE, 3},
    {SPECULAR, 3},
    {SPECULAR_EXPONENT, 1},
    {DISSOLVE, 1},
    {TRANSPARENT, 1},
    {TRANSMISSION_FILTER, 3},
    {OPTICAL_DENSITY, 1},
    {ILLUMINATION, 1}
};

const std::unordered_map<MtlElemType, Bound> mtlElemBounds = {
    {AMBIENT, Bound(0.f, 1.f)},
    {DIFFUSE, Bound(0.f, 1.f)},
    {SPECULAR, Bound(0.f, 1.f)},
    {SPECULAR_EXPONENT, Bound(0.f, 1000.f)},
    {DISSOLVE, Bound(0.f, 1.f)},
    {TRANSPARENT, Bound(0.f, 1.f)},
    {TRANSMISSION_FILTER, Bound(0.f, 1.f)}, // Transmission filter in CIE XYZ or with spectral curve file not supported
    {OPTICAL_DENSITY, Bound(0.001f, 10.f)},
    {ILLUMINATION, Bound(0, 10)}
};

const std::unordered_map<MtlElemType, std::function<std::pair<bool, IntOrFloat>(std::string)>> mtlElemCheckTypeFuncs = {
    {NEW_MAT, nullptr},
    {AMBIENT, isFloat},
    {DIFFUSE, isFloat},
    {SPECULAR, isFloat},
    {SPECULAR_EXPONENT, isFloat},
    {DISSOLVE, isFloat},
    {TRANSPARENT, isFloat},
    {TRANSMISSION_FILTER, isFloat},
    {OPTICAL_DENSITY, isFloat},
    {ILLUMINATION, isInteger}
};

MtlElemType getMtlElemType(const std::string& prefix) {
    auto it = mtlElemMap.find(prefix);
    if (it != mtlElemMap.end())
        return it->second;
    return UNKNOWN_;
}

bool checkElemValid(MtlElemType elemType, std::vector<std::string> &tokens, size_t lineNb) {
    if (tokens.size() - 1 != mtlElemSize.at(elemType)) {
        std::cerr << "Invalid number of arguments for " << elemType << ": " << lineNb << std::endl;
        return false;
    }

    if (mtlElemCheckTypeFuncs.at(elemType) == nullptr)
        return true;

    for (size_t i = 1; i < tokens.size(); i++) {
        auto res = mtlElemCheckTypeFuncs.at(elemType)(tokens[i]);
        if (!res.first) {
            std::cerr << "Invalid value for " << elemType << ": " << lineNb << std::endl;
            return false;
        }

        if (!mtlElemBounds.at(elemType).isInside(res.second)) {
            std::cerr << "Invalid value for " << elemType << ": " << lineNb << std::endl;
            return false;
        }
    }
    return true;
}