#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <fstream>

#include "sharedParsing.hpp"
#include "Object.hpp"

namespace scop {

class Parser {

    public:
        Parser(std::string const &path) {
            std::ifstream file(path);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << path << std::endl;
                return;
            }

            std::string line;
            size_t lineNb = 1;
            bool vertexDef = false, faceDef = false, lineDef = false;
            std::array<size_t, 3> geometryElemCounts{0, 0, 0};
            std::string currentMaterial = "";
            int currentSmoothingGroup = 0;

            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') {
                    lineNb++;
                    continue;
                }

                std::istringstream ss(line);
                std::string prefix;
                ss >> prefix;
                
                if (CHECK(prefix, VERTEX)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineDef, lineNb);
                    checkObjExist();
                    currentObject->vertices.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, TEXCOORD)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineDef, lineNb);
                    checkObjExist();
                    currentObject->texCoords.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, NORMAL)) {
                    checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineDef, lineNb);
                    checkObjExist();
                    currentObject->normals.emplace_back(ss, lineNb);
                } else if (CHECK(prefix, FACE)) {
                    if (!faceDef) {
                        geometryElemCounts[VERTEX_INX] = currentObject->vertices.size();
                        geometryElemCounts[TEX_INX] = currentObject->texCoords.size();
                        geometryElemCounts[NORMAL_INX] = currentObject->normals.size();
                    }
                    checkElemOrder(FACE_TYPE, vertexDef, faceDef, lineDef, lineNb);
                    checkObjExist();
                    currentObject->addFace(ss, lineNb, geometryElemCounts, currentMaterial, currentSmoothingGroup);
                } else if (CHECK(prefix, LINE)) {
                    if (!lineDef)
                        geometryElemCounts[VERTEX_INX] = currentObject->vertices.size();
                    checkElemOrder(LINE_TYPE, vertexDef, faceDef, lineDef, lineNb);
                    checkObjExist();
                    currentObject->addLine(ss, lineNb, geometryElemCounts[VERTEX_INX]);
                } else if (CHECK(prefix, OBJ)) {
                    std::string name;
                    ss >> name;
                    checkIssParse(ss, lineNb, "Failed to parse object");
                    _objects.emplace(name, Object(name));

                    currentObject = &_objects[name];
                    vertexDef = faceDef = false;
                    geometryElemCounts = {0, 0, 0};
                } else if (CHECK(prefix, GROUP)) {
                    checkObjExist();
                    currentObject->addGroup(ss, lineNb);
                } else if (CHECK(prefix, SMOOTHING_GROUP)) {
                    std::string sGroup;
                    ss >> sGroup;
                    if (sGroup == "off")
                        currentSmoothingGroup = 0;
                    else {
                        try {
                            currentSmoothingGroup = std::stoi(sGroup);
                        } catch (std::exception &e) {
                            std::cerr << "Failed to parse smoothing group on line " << lineNb << std::endl;
                            throw std::exception();
                        }
                    }
                    checkIssParse(ss, lineNb, "Failed to parse smoothing group");
                } else if (CHECK(prefix, MATLIB)) {
                    std::string matLib;
                    ss >> matLib;
                    checkIssParse(ss, lineNb, "Failed to parse material library");
                    _materialLibraries.push_back(matLib);
                } else if (CHECK(prefix, USRMTL)) {
                    ss >> currentMaterial;
                    checkIssParse(ss, lineNb, "Failed to parse material name");
                } else {
                    std::cerr << "Prefix not suppported: " << prefix << " on line " << lineNb << std::endl;
                    throw std::exception();
                }
                lineNb++;
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const Parser& parser) {
            for (auto const &m : parser._materialLibraries) {
                os << "mtllib " << m << std::endl;
            }
            for (auto const &o : parser._objects) {
                os << o.second << std::endl;
            }
            return os;
        }


    private:
        Parser() = delete;

        std::unordered_map <std::string, Object> _objects;
        std::vector<std::string> _materialLibraries;

        void checkObjExist() {
            if (currentObject == nullptr) {
                _objects.emplace("default", Object(""));
                currentObject = &_objects[""];
            }
        }

        void checkElemOrder(int type, bool &vertexDef, bool &faceDef, bool &lineDef, size_t &lineNb) {
            if (type == VERTEX_TYPE) {
                if (faceDef || lineDef) {
                    std::cerr << "Vertex definition after face definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Vertex definition after face definition");
                }
                vertexDef = true;
            } else if (type == FACE_TYPE) {
                if (!vertexDef) {
                    std::cerr << "Face definition before vertex definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Face definition before vertex definition");
                }
                faceDef = true;
            } else if (type == LINE_TYPE) {
                if (!vertexDef) {
                    std::cerr << "Line definition before vertex definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Line definition before vertex definition");
                }
                lineDef = true;
            }
        }

        Object *currentObject = nullptr;
};

}