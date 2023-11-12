#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <fstream>

#include "objElements/Object.hpp"

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

                auto tokens = split(line, ' ');
                if (tokens.size() < 2) {
                    std::cerr << "Invalid line: " << lineNb << std::endl;
                    throw std::exception();
                }

                ElemType eType = getElemType(tokens[0]);
                switch (eType) {
                    case VERTEX:
                        addGeometryElement(tokens, lineNb, VERTEX, vertexDef, faceDef, lineDef);
                        break;

                    case TEXCOORD:
                        addGeometryElement(tokens, lineNb, TEXCOORD, vertexDef, faceDef, lineDef);
                        break;

                    case NORMAL:
                        addGeometryElement(tokens, lineNb, NORMAL, vertexDef, faceDef, lineDef);
                        break;

                    case FACE:
                        if (!faceDef) {
                            geometryElemCounts[VERTEX_INX] = currentObject->_vertices.size();
                            geometryElemCounts[TEX_INX] = currentObject->_texCoords.size();
                            geometryElemCounts[NORMAL_INX] = currentObject->_normals.size();
                        }
                        checkElemOrder(FACE_TYPE, vertexDef, faceDef, lineDef, lineNb);
                        checkObjExist();
                        currentObject->addFace(tokens, lineNb, geometryElemCounts, currentMaterial, currentSmoothingGroup);
                        break;

                    case LINE:
                        if (!lineDef)
                            geometryElemCounts[VERTEX_INX] = currentObject->_vertices.size();
                        checkElemOrder(LINE_TYPE, vertexDef, faceDef, lineDef, lineNb);
                        checkObjExist();
                        currentObject->addLine(tokens, lineNb, geometryElemCounts[VERTEX_INX]);
                        break;

                    case OBJ:
                        if (tokens.size() != 2) {
                            std::cerr << "Invalid object format on line " << lineNb << std::endl;
                            throw std::exception();
                        }
                        _objects.emplace(tokens[1], Object(tokens[1]));
                        currentObject = &_objects[tokens[1]];
                        vertexDef = faceDef = false;
                        geometryElemCounts = {0, 0, 0};
                        break;

                    case GROUP:
                        checkObjExist();
                        currentObject->addGroup(tokens, lineNb);
                        break;

                    case SMOOTHING_GROUP:
                        if (tokens.size() != 2) {
                            std::cerr << "Invalid smoothing group format on line " << lineNb << std::endl;
                            throw std::exception();
                        }
                        if (tokens[1] == "off")
                            currentSmoothingGroup = 0;
                        else {
                            try {
                                currentSmoothingGroup = std::stoi(tokens[1]);
                            } catch (std::exception &e) {
                                std::cerr << "Failed to parse smoothing group on line " << lineNb << std::endl;
                                throw std::invalid_argument("Failed to parse smoothing group");
                            }
                        }
                        break;

                    case MATLIB:
                        if (tokens.size() != 2) {
                            std::cerr << "Invalid material library format on line " << lineNb << std::endl;
                            throw std::exception();
                        }
                        _materialLibraries.push_back(tokens[1]);
                        break;

                    case USEMTL:
                        if (tokens.size() != 2) {
                            std::cerr << "Invalid material name format on line " << lineNb << std::endl;
                            throw std::exception();
                        }
                        currentMaterial = tokens[1];
                        break;
                    case UNKNOWN:
                        std::cerr << "Unknown prefix: " << tokens[0] << " on line " << lineNb << std::endl;
                        throw std::exception();
                        break;

                }

                lineNb++;
            }
        }

        std::unordered_map<std::string, Object> const &getObjects() const { return _objects; }

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
        Parser() {}

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

        void addGeometryElement(std::vector<std::string> const &tokens, size_t lineNb, int elemType, bool &vertexDef, bool &faceDef, bool &lineDef) {
            checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineDef, lineNb);
            checkObjExist();
            if (elemType == VERTEX)
                currentObject->_vertices.emplace_back(tokens, lineNb);
            else if (elemType == TEXCOORD)
                currentObject->_texCoords.emplace_back(tokens, lineNb);
            else
                currentObject->_normals.emplace_back(tokens, lineNb);
        }

        Object *currentObject = nullptr;
};

}