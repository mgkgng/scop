#pragma once

#include "sharedParsing.hpp"
#include "Vertex.hpp"
#include "TexCoord.hpp"
#include "Normal.hpp"
#include "Group.hpp"

#include <unordered_map>

namespace scop {

struct Object {
    std::string _name;

    std::vector<Vertex   > vertices;
    std::vector<TexCoord > texCoords;
    std::vector<Normal   > normals;

    std::unordered_map<std::string, Group> groups;

    Object() = default;
    Object(std::string const &name) : _name(name) {}

    void addVertex(std::istringstream &iss, size_t &lineNb) { vertices.emplace_back(iss, lineNb); }
    void addTexCoord(std::istringstream &iss, size_t &lineNb) { texCoords.emplace_back(iss, lineNb); }
    void addNormal(std::istringstream &iss, size_t &lineNb) { normals.emplace_back(iss, lineNb); }
    void addFace(std::istringstream &iss, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts) {
        if (currentGroup == nullptr) {
            groups.emplace("default", Group(""));
            currentGroup = &groups[""];
        }
        currentGroup->addFace(iss, lineNb, geometryElemCounts);
    }
    void addGroup(std::istringstream &iss, size_t &lineNb) {
        std::string name;
        iss >> name;
        checkIssParse(iss, lineNb, "Failed to parse group");
        groups.emplace(name, Group(name));
        currentGroup = &groups[name];
    }

    friend std::ostream& operator<<(std::ostream& os, const Object& object) {
            for (const auto& v : object.vertices)
                os << v << std::endl;
            for (const auto& tc : object.texCoords)
                os << tc << std::endl;
            for (const auto& n : object.normals)
                os << n << std::endl;
            for (const auto& g : object.groups)
                os << g.second << std::endl;
            return os;
        }


    private:
        Group *currentGroup = nullptr;
};

}