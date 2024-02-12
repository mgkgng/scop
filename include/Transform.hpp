#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "objElements/Vertex.hpp"
#include "Matrix.hpp"

class Transform {
    public:
        Matrix modelMat;
        Matrix viewMat;
        Matrix projectionMat;
        glm::vec3 rotation;
        glm::vec3 objectCenter;
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraUp;

        float fov = 45.0f;
        float aspectRatio = 960.0f / 640.0f; // Default aspect ratio
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        Transform(float width, float height) {
            this->projectionMat.setProjection(fov, width / height, 0.1f, 1000.0f);

            this->modelMat.setModel(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

            float cameraPos[] = {0.0f, 0.0f, 3.0f};
            float targetPos[] = {0.0f, 0.0f, 0.0f};
            float upVector[] = {0.0f, 1.0f, 0.0f};
            Matrix view_matrix;
            view_matrix.setView(cameraPos, targetPos, upVector);
        }

        void reset() {
            this->modelMat.setToIdentity();
            this->viewMat.setToIdentity();
            this->projectionMat.setToIdentity();
        }

        // glm::vec3 calculateObjectCenter(const std::vector<Vertex>& vertices) {
        //     // std::cout << "vertex size: " << vertices.size() << std::endl;
        //     // std::cout << "camera view: " << view[3][0] << ", " << view[3][1] << ", " << view[3][2] << std::endl;
        //     // std::cout << "object scale: " << glm::length(glm::vec3(model[0])) << ", " << glm::length(glm::vec3(model[1])) << ", " << glm::length(glm::vec3(model[2])) << std::endl;
        //     // glm::vec3 center(0.0f, 0.0f, 0.0f);
        //     // for (const auto& vertex : vertices)
        //     //     center += glm::vec3(vertex.x, vertex.y, vertex.z);
        //     // std::cout << "here center: " << center.x << ", " << center.y << ", " << center.z << std::endl;
        //     // center /= static_cast<float>(vertices.size());
        //     // return center;
        // }

};
