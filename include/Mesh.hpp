#pragma once

#include <GL/glew.h>
#include <vector>

#include "Parser.hpp"
#include "MeshVertex.hpp"

class Mesh {
    public:
        Mesh(Object const &obj) {
            std::cout << "Creating mesh..." << std::endl;
            
            parseObj(obj);

            glGenVertexArrays(1, &_vao);
            glBindVertexArray(_vao);

            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(MeshVertex), _vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, position));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, normal));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (void *)offsetof(MeshVertex, texCoord));
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);

            std::cout << "Mesh created successfully" << std::endl;
        }
        
        ~Mesh() {
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
        }

        void draw() const {
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
            glBindVertexArray(0);
        }

        void parseObj(Object const &obj) {
            for (auto const &group : obj._groups) {
                for (auto const &face : group.second.faces) {
                    for (size_t i = 0; i < face.vertexCount; i++) {
                        MeshVertex meshVertex;
                        auto vertex = obj.getVertexByIndex(face.vertexIndices[i]);
                        meshVertex.position = std::array<float, 4>{vertex.x, vertex.y, vertex.z, vertex.w};
                        if (!face.textureIndices.empty()) {
                            auto texCoord = obj.getTexCoordByIndex(face.textureIndices[i]);
                            meshVertex.texCoord = std::array<float, 3>{texCoord.u, texCoord.v, texCoord.w};
                        }
                        if (!face.normalIndices.empty()) {
                            auto normal = obj.getNormalByIndex(face.normalIndices[i]);
                            meshVertex.normal = std::array<float, 3>{normal.x, normal.y, normal.z};
                        }
                        _vertices.push_back(meshVertex);
                    }
                }
            }
            _vertexCount = _vertices.size();
        }

        GLuint getVao() const { return _vao; }
        std::vector<MeshVertex> const &getVertices() const { return _vertices; }

    private:
        GLuint _vao, _vbo;
        std::vector<MeshVertex> _vertices;
        size_t _vertexCount;

        
};