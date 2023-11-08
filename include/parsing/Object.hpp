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

    void addVertex(std::vector<std::string> const &tokens, size_t &lineNb) { vertices.emplace_back(tokens, lineNb); }
    void addTexCoord(std::vector<std::string> const &tokens, size_t &lineNb) { texCoords.emplace_back(tokens, lineNb); }
    void addNormal(std::vector<std::string> const &tokens, size_t &lineNb) { normals.emplace_back(tokens, lineNb); }
    void addFace(std::vector<std::string> const &tokens, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts, std::string &materialName, int &smoothingGroup) {
        if (currentGroup == nullptr) {
            groups.emplace("default", Group(""));
            currentGroup = &groups[""];
        }
        currentGroup->addFace(tokens, lineNb, geometryElemCounts, materialName, smoothingGroup);
    }
    void addLine(std::vector<std::string> const &tokens, size_t &lineNb, size_t &vertexCount) {
        if (currentGroup == nullptr) {
            groups.emplace("default", Group(""));
            currentGroup = &groups[""];
        }
        currentGroup->addLine(tokens, lineNb, vertexCount);
    }
    void addGroup(std::vector<std::string> const &tokens, size_t &lineNb) {
        if (tokens.size() != 2) {
            std::cerr << "Invalid group: " << lineNb << std::endl;
            throw std::exception();
        }
        groups.emplace(tokens[1], Group(tokens[1]));
        currentGroup = &groups[tokens[1]];
    }

    friend std::ostream& operator<<(std::ostream& os, const Object& object) {
            if (!object._name.empty())
                os << "o " << object._name << std::endl;
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