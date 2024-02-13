#pragma once

#include <vector>

#include "objElements/Vertex.hpp"
#include "Matrix.hpp"

class Transform {
    public:
        Matrix modelMat;
        Matrix viewMat;
        Matrix projectionMat;

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
};
