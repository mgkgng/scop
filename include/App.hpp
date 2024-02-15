#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <memory>

#include "Transform.hpp"
#include "Mesh.hpp"
#include "Parser.hpp"

#define WIDTH 960.0f
#define HEIGHT 720.0f
#define TRANSITION_SPEED 0.05f


class App {
    public:
        bool isDragging = false;
        bool isInTransition = false;
        bool hasNormals = false;
        float textureState = 0.0f;
        double lastX = 0.0, lastY = 0.0;
        float _textureTarget;
        std::unique_ptr<Mesh> _mesh;
        std::unique_ptr<Transform> _transform;

        App(const std::unordered_map<std::string, Object> &objects) {
            init();
            _mesh = std::make_unique<Mesh>(objects);
            _transform = std::make_unique<Transform>(WIDTH, HEIGHT);
            std::cout << "App created successfully" << std::endl;
        }
        ~App() { glfwTerminate(); }

        void init() {
            std::cout << "Initializing SCOP App" << std::endl;
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                exit(1);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

            _window = glfwCreateWindow(WIDTH, HEIGHT, "SCOP", nullptr, nullptr);
            if (!_window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(1);
            }

            glfwMakeContextCurrent(_window);
            glfwSetWindowUserPointer(_window, this);
            glfwSetKeyCallback(_window, App::keyCallback);
            glfwSetMouseButtonCallback(_window, App::mouseButtonCallback);
            glfwSetCursorPosCallback(_window, App::cursorPositionCallback);
            glfwSetScrollCallback(_window, App::scrollCallback);

            if (glewInit() != GLEW_OK) {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                exit(1);
            }

            glEnable(GL_DEPTH_TEST);
            std::cout << "App initialized" << std::endl;
        }

        void run(std::function<void()> loop) {
            while (!glfwWindowShouldClose(_window)) {
                glClearColor(231.0f / 255.0f, 87.0f / 255.0f, 51.0f / 255.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                loop();
                glfwSwapBuffers(_window);
                glfwPollEvents();
            }
        }

       static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            // Handle key events here
            (void) scancode;
            (void) mods;

            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                switch (key) {
                    case GLFW_KEY_ESCAPE:
                        glfwSetWindowShouldClose(window, GLFW_TRUE);
                        break;
                    case GLFW_KEY_T:
                        if (app) app->applyTexture();
                        break;
                    case GLFW_KEY_UP:
                    case GLFW_KEY_W:
                        if (app) app->_transform->modelMat.move(0.f, 1.f, 0.f);
                        break;
                    case GLFW_KEY_DOWN:
                    case GLFW_KEY_S:
                        if (app) app->_transform->modelMat.move(0.f, -1.f, 0.f);
                        break;
                    case GLFW_KEY_LEFT:
                    case GLFW_KEY_A:
                        if (app) app->_transform->modelMat.move(-1.f, 0.f, 0.f);
                        break;
                    case GLFW_KEY_RIGHT:
                    case GLFW_KEY_D:
                        if (app) app->_transform->modelMat.move(1.f, 0.f, 0.f);
                        break;
                    // Add more cases as needed
                }
            }
        }

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            (void) mods;

            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                if (action == GLFW_PRESS) {
                    glfwGetCursorPos(window, &app->lastX, &app->lastY);
                    app->isDragging = true;
                } else if (action == GLFW_RELEASE) {
                    app->isDragging = false;
                }
            }

        }

        static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            double deltaX = xpos - app->lastX;
            double deltaY = ypos - app->lastY;
            app->lastX = xpos;
            app->lastY = ypos;

            if (app->isDragging)
                app->moveCamera(deltaX, deltaY);
        }

        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
            (void) xoffset;
    
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (app)
                app->handleZoom(yoffset);
        }

        void handleZoom(double yoffset) {
            float dz = yoffset * 0.1f;
        
            _transform->modelMat.move(0.0f, 0.0f, dz);
        }

        void moveCamera(double deltaX, double deltaY) {
            const float sensitivity = 0.005f;

            float angleX = static_cast<float>(deltaY) * sensitivity;
            float angleY = static_cast<float>(deltaX) * sensitivity;

            _transform->modelMat.rotate(angleY, 0.0f, 1.0f, 0.0f);
            _transform->modelMat.rotate(angleX, 1.0f, 0.0f, 0.0f);
        }

        	void applyTexture() {
                if (isInTransition) return;
                _textureTarget = (textureState == 0.0f) ? 1.0f : 0.0f;
                isInTransition = true;
            }

            void updateTextureTransition(GLuint _id) {
                    /* Move is_texture_enabled towards target value */ 
                    if (textureState < _textureTarget) {
                        textureState += TRANSITION_SPEED;
                        if (textureState > _textureTarget) {
                            textureState = _textureTarget;
                            isInTransition = false;
                        }
                    } else {
                        textureState -= TRANSITION_SPEED;
                        if (textureState < _textureTarget) {
                            textureState = _textureTarget;
                            isInTransition = false;
                        }
                    }

                    std::cout << "textureState: " << textureState << std::endl;
                    // Update the uniform variable in the shader
                    GLint textureStateLoc = glGetUniformLocation(_id, "textureState");
                    glUniform1f(textureStateLoc, textureState);
            }


    private:
        GLFWwindow* _window;

};