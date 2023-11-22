#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "MeshVertex.hpp"

class Transform {
    public:
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 rotation;
        glm::vec3 objectCenter;

        float fov = 45.0f;
        float aspectRatio = 960.0f / 640.0f; // Default aspect ratio
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

        Transform() {}
        Transform(const std::vector<MeshVertex> vertices): model(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), rotation(glm::vec3(0.0f)) {
            objectCenter = calculateObjectCenter(vertices);

            std::cout << "Transform created successfully" << std::endl;
        }

        void updateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
            this->rotation = rotation;
            model = glm::mat4(1.0f);

            // Translate to origin (object's center)
            model = glm::translate(model, -objectCenter);

            // Apply rotation
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            
            // Translate back to original position
            model = glm::translate(model, objectCenter);

            // Apply translation and scale
            model = glm::translate(model, position);
            model = glm::scale(model, scale);
        }

        void updateViewMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
            view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        }

        void updateProjectionMatrix() {
            projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
        
        void reset() {
            model = glm::mat4(1.0f);
            view = glm::mat4(1.0f);
            projection = glm::mat4(1.0f);
        }

        glm::vec3 calculateObjectCenter(const std::vector<MeshVertex>& vertices) {
            glm::vec3 center(0.0f, 0.0f, 0.0f);
            for (const auto& vertex : vertices)
                center += glm::vec3(vertex.position); // Assuming each vertex has a 'position' attribute
            center /= static_cast<float>(vertices.size());
            return center;
        }

        void setFOV(float f) {
            fov = f;
            updateProjectionMatrix();
        }
};
