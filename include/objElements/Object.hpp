#pragma once

#include "obj.hpp"
#include "Vertex.hpp"
#include "TexCoord.hpp"
#include "Normal.hpp"
#include "Group.hpp"

#include <unordered_map>
#include <optional>

struct Object {
    std::string _name;

    std::vector<Vertex   > _vertices;
    std::vector<TexCoord > _texCoords;
    std::vector<Normal   > _normals;

    std::unordered_map<std::string, Group> _groups;

    Object() = default;
    Object(std::string const &name) : _name(name) {}

    void addVertex(std::vector<std::string> const &tokens, size_t &lineNb) { _vertices.emplace_back(tokens, lineNb); }
    void addTexCoord(std::vector<std::string> const &tokens, size_t &lineNb) { _texCoords.emplace_back(tokens, lineNb); }
    void addNormal(std::vector<std::string> const &tokens, size_t &lineNb) { _normals.emplace_back(tokens, lineNb); }
    void addFace(std::vector<std::string> const &tokens, size_t &lineNb, std::array<size_t, 3> &geometryElemCounts, Material *mat, int &smoothingGroup) {
        if (currentGroup == nullptr) {
            _groups.emplace("default", Group(""));
            currentGroup = &_groups[""];
        }
        currentGroup->addFace(tokens, lineNb, geometryElemCounts, mat, smoothingGroup);
    }
    void addLine(std::vector<std::string> const &tokens, size_t &lineNb, size_t &vertexCount) {
        if (currentGroup == nullptr) {
            _groups.emplace("default", Group(""));
            currentGroup = &_groups[""];
        }
        currentGroup->addLine(tokens, lineNb, vertexCount);
    }
    void addGroup(std::vector<std::string> const &tokens, size_t &lineNb) {
        if (tokens.size() != 2) {
            std::cerr << "Invalid group: " << lineNb << std::endl;
            throw std::exception();
        }
        _groups.emplace(tokens[1], Group(tokens[1]));
        currentGroup = &_groups[tokens[1]];
    }

    friend std::ostream& operator<<(std::ostream& os, const Object& object) {
        os << "object name: " << object._name << std::endl;
        if (!object._name.empty())
            os << "o " << object._name << std::endl;
        for (const auto& v : object._vertices)
            os << v << std::endl;
        for (const auto& tc : object._texCoords)
            os << tc << std::endl;
        for (const auto& n : object._normals)
            os << n << std::endl;
        for (const auto& g : object._groups)
            os << g.second << std::endl;
        return os;
    }

    Vertex getVertexByIndex(size_t index) const { return _vertices[index]; }
    TexCoord getTexCoordByIndex(size_t index) const { return _texCoords[index]; }
    Normal getNormalByIndex(size_t index) const { return _normals[index]; }

    private:
        Group *currentGroup = nullptr;
 
};