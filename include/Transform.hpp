#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace scop {

class Transform {
    public:
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        Transform(): model(glm::mat4(1.0f)), view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {}

        void updateModelMatrix(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, position);
            model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, scale);
        }

        void updateViewMatrix(glm::vec3 cameraPos, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
            view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        }

        void updateProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
            projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }
        
        void setModel(glm::mat4 const &m) { model = m; }
        void setView(glm::mat4 const &v) { view = v; }
        void setProjection(glm::mat4 const &p) { projection = p; }

        void translate(glm::vec3 const &v) { model = glm::translate(model, v); }
        void rotate(float angle, glm::vec3 const &v) { model = glm::rotate(model, angle, v); }
        void scale(glm::vec3 const &v) { model = glm::scale(model, v); }

        void lookAt(glm::vec3 const &eye, glm::vec3 const &center, glm::vec3 const &up) { view = glm::lookAt(eye, center, up); }
        void perspective(float fov, float aspect, float near, float far) { projection = glm::perspective(fov, aspect, near, far); }
        void ortho(float left, float right, float bottom, float top, float near, float far) { projection = glm::ortho(left, right, bottom, top, near, far); }

        void reset() {
            model = glm::mat4(1.0f);
            view = glm::mat4(1.0f);
            projection = glm::mat4(1.0f);
        }

        glm::mat4 getModelViewProjection() const { return projection * view * model; }
        glm::mat4 getModelView() const { return view * model; }
        glm::mat4 getModel() const { return model; }
        glm::mat4 getView() const { return view; }
        glm::mat4 getProjection() const { return projection; }

        glm::vec3 getCameraPosition() const { return glm::vec3(glm::inverse(view)[3]); }

};

}