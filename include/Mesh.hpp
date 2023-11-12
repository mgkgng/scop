#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Parser.hpp"

namespace scop {

struct MeshVertex {
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec3 texCoord;
};

class Mesh {
    public:
        Mesh(scop::Object const &obj) {
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
        }
        ~Mesh() {
            glDeleteVertexArrays(1, &_vao);
            glDeleteBuffers(1, &_vbo);
        }

        void draw() const {
            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
            glBindVertexArray(0);
        }

        void parseObj(scop::Object const &obj) {
            for (auto const &group : obj._groups) {
                for (auto const &face : group.second.faces) {
                    for (size_t i = 0; i < face.vertexCount; i++) {
                        MeshVertex meshVertex;
                        std::cout << "hallo" << std::endl;
                        auto vertex = obj.getVertexByIndex(face.vertexIndices[i]);
                        auto texCoord = obj.getTexCoordByIndex(face.textureIndices[i]);
                        auto normal = obj.getNormalByIndex(face.normalIndices[i]);

                        if (vertex.has_value())
                            meshVertex.position = glm::vec4(vertex.value().x, vertex.value().y, vertex.value().z, vertex.value().w);
                        if (texCoord.has_value())
                            meshVertex.texCoord = glm::vec3(texCoord.value().u, texCoord.value().v, texCoord.value().w);
                        if (normal.has_value())
                            meshVertex.normal = glm::vec3(normal.value().x, normal.value().y, normal.value().z);
                        _vertices.push_back(meshVertex);
                    }
                }
            }
        }

    private:
        GLuint _vao, _vbo;
        std::vector<MeshVertex> _vertices;

        
};

}