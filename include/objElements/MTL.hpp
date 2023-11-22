#pragma once

#include <fstream>

#include "Material.hpp"

struct MTL {
    std::unordered_map<std::string, Material> _materials;
    Material *_currentMaterial = nullptr;

    MTL() {}
    MTL(const std::string objPath, const std::string mtlPath) {
        size_t pos = objPath.find_last_of('/');
        std::string filePath = (pos != std::string::npos) ? objPath.substr(0, pos + 1) + mtlPath : mtlPath;


        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        std::string line;
        size_t lineNb = 1;

        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') {
                lineNb++;
                continue;
            }

            auto tokens = split(line, ' ');
            if (tokens.size() < 2) {
                std::cerr << "Invalid line: " << lineNb << std::endl;
                throw std::exception();
            }

            MtlElemType eType = getMtlElemType(tokens[0]);
            switch (eType) {
                case NEW_MAT:
                    if (tokens.size() != 2) {
                        std::cerr << "Invalid material name: " << lineNb << std::endl;
                        throw std::exception();
                    }
                    _materials.emplace(tokens[1], tokens[1]);
                    _currentMaterial = &_materials[tokens[1]];
                    break;

                case AMBIENT:
                case DIFFUSE:
                case SPECULAR:
                case TRANSMISSION_FILTER:
                    if (_currentMaterial == nullptr) {
                        std::cerr << "No material defined on line " << lineNb << std::endl;
                        throw std::exception();
                    }
                    if (!checkElemValid(eType, tokens, lineNb))
                        throw std::exception();
                    _currentMaterial->addRGB(eType, tokens, lineNb);
                    break;

                case SPECULAR_EXPONENT:
                case DISSOLVE:
                case TRANSPARENT:
                case OPTICAL_DENSITY:
                case ILLUMINATION:
                    if (_currentMaterial == nullptr) {
                        std::cerr << "No material defined on line " << lineNb << std::endl;
                        throw std::exception();
                    }
                    if (!checkElemValid(eType, tokens, lineNb))
                        throw std::exception();
                    _currentMaterial->addValue(eType, tokens, lineNb);
                    break;


                case UNKNOWN_:
                    std::cerr << "Unknown prefix: " << tokens[0] << " on line " << lineNb << std::endl;
                    throw std::exception();
                    break;
            }
            lineNb++;
        }

    }

    ~MTL() {}

    friend std::ostream& operator<<(std::ostream& os, const MTL& mtl) {
        for (auto& [name, mat] : mtl._materials)
            os << mat << std::endl;
        return os;
    }

};