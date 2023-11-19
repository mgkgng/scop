#pragma once

#include <vector>

#include "mtlElements.hpp"

struct RGB {
    float r, g, b;
};

struct Material {
    std::string _name;
    RGB _ambient = {0.0f, 0.0f, 0.0f};
    RGB _diffuse = {0.0f, 0.0f, 0.0f};
    RGB _specular = {0.0f, 0.0f, 0.0f};
    RGB _transmissionFilter = {0.0f, 0.0f, 0.0f};
    float _specularExponent = 0.0f;
    float _dissolve = 1.0f;
    float _opticalDensity = 1.0f;
    size_t _illumination = 0;

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
            case TRANSMISSION_FILTER:
                _transmissionFilter = {std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3])};
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
            case DISSOLVE:
                _dissolve = std::stof(tokens[1]);
                break;
            case TRANSPARENT:
                _dissolve = 1 - std::stof(tokens[1]);
                break;
            case OPTICAL_DENSITY:
                _opticalDensity = std::stof(tokens[1]);
                break;
            case ILLUMINATION:
                _illumination = std::stoi(tokens[1]);
                break;
            default:
                std::cerr << "Invalid value type: " << lineNb << std::endl;
                throw std::exception();
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Material& m) {
        os << "newmtl " << m._name << "\n";
        os << "Ka " << m._ambient.r << " " << m._ambient.g << " " << m._ambient.b << "\n";
        os << "Kd " << m._diffuse.r << " " << m._diffuse.g << " " << m._diffuse.b << "\n";
        os << "Ks " << m._specular.r << " " << m._specular.g << " " << m._specular.b << "\n";
        os << "Ns " << m._specularExponent << "\n";
        os << "d " << m._dissolve << "\n";
        os << "illum " << m._illumination << "\n";
        return os;
    }

};