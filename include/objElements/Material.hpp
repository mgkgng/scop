#pragma once

#include <vector>

#include "mtlElements.hpp"

struct RGB {
    float r, g, b;
};

struct Material {
    std::string _name;
    RGB _ambient;
    RGB _diffuse;
    RGB _specular;
    float _specularExponent;


    Material() {}
    Material(const std::string &name) : _name(name) {}

    void addRGB(MtlElemType elemType, std::vector<std::string> &tokens, size_t lineNb) {
        if (tokens.size() != 4) {
            std::cerr << "Invalid ambient color definition: " << lineNb << std::endl;
            throw std::exception();
        }

        switch (elemType) {
            case AMBIENT:
                _ambient = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
                break;
            case DIFFUSE:
                _diffuse = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
                break;
            case SPECULAR:
                _specular = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
                break;
            default:
                std::cerr << "Invalid RGB type: " << lineNb << std::endl;
                throw std::exception();
        }
    }

    void addValue(MtlElemType elemType, std::vector<std::string> &tokens, size_t lineNb) {
        if (tokens.size() != 2) {
            std::cerr << "Invalid specular exponent definition: " << lineNb << std::endl;
            throw std::exception();
        }

        switch (elemType) {
            case SPECULAR_EXPONENT:
                _specularExponent = std::stof(tokens[1]);
                break;
            default:
                std::cerr << "Invalid value type: " << lineNb << std::endl;
                throw std::exception();
        }
    }

};