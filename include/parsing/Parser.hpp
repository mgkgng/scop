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

            try {
                std::string line;
                size_t lineNb = 1;
                bool vertexDef = false, faceDef = false;
                std::array<size_t, 3> geometryElemCounts{0, 0, 0};

                while (std::getline(file, line)) {
                    if (line.empty() || line[0] == '#') {
                        lineNb++;
                        continue;
                    }

                    std::istringstream ss(line);
                    std::string prefix;
                    ss >> prefix;
                    
                    if (CHECK(prefix, VERTEX)) {
                        checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                        checkObjExist();
                        currentObject->vertices.emplace_back(ss, lineNb);
                    } else if (CHECK(prefix, TEXCOORD)) {
                        checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                        checkObjExist();
                        currentObject->texCoords.emplace_back(ss, lineNb);
                    } else if (CHECK(prefix, NORMAL)) {
                        checkElemOrder(VERTEX_TYPE, vertexDef, faceDef, lineNb);
                        checkObjExist();
                        currentObject->normals.emplace_back(ss, lineNb);
                    } else if (CHECK(prefix, FACE)) {
                        checkElemOrder(FACE_TYPE, vertexDef, faceDef, lineNb);
                        checkObjExist();
                        if (!faceDef) {
                            geometryElemCounts[VERTEX_INX] = currentObject->vertices.size();
                            geometryElemCounts[TEX_INX] = currentObject->texCoords.size();
                            geometryElemCounts[NORMAL_INX] = currentObject->normals.size();
                        }
                        currentObject->addFace(ss, lineNb, geometryElemCounts);
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
                        std::cout << "Smoothing group" << std::endl;
                    } else if (CHECK(prefix, MATLIB)) {
                        std::string matLib;
                        ss >> matLib;
                        checkIssParse(ss, lineNb, "Failed to parse material library");
                        _materialLibraries.push_back(matLib);
                    } else if (CHECK(prefix, USRMTL)) {
                        std::cout << "Use material" << std::endl;
                    } else {
                        std::cerr << "Prefix not suppported: " << prefix << " on line " << lineNb << std::endl;
                        throw std::exception();
                    }
                    lineNb++;
                }
            } catch (std::exception const &e) {
                std::cerr << "Failed to parse file: " << path << std::endl;
                std::cerr << e.what() << std::endl;
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const Parser& parser) {
            for (auto const &m : parser._materialLibraries) {
                os << "mtllib " << m << std::endl;
            }
            os << std::endl;
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

        void checkElemOrder(int type, bool &vertexDef, bool &faceDef, size_t &lineNb) {
            if (type == VERTEX_TYPE) {
                if (faceDef) {
                    std::cerr << "Vertex definition after face definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Vertex definition after face definition");
                }
                vertexDef = true;
            } else {
                if (!vertexDef) {
                    std::cerr << "Face definition before vertex definition on line " << lineNb << std::endl;
                    throw std::runtime_error("Face definition before vertex definition");
                }
                faceDef = true;
            }
        }

        Object *currentObject = nullptr;
};

}