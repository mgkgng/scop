#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

#include "Transform.hpp"

namespace scop {

class App {
    public:
        App() {}
        ~App() { glfwTerminate(); }

        void init() {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                exit(1);
            }

            _window = glfwCreateWindow(960, 640, "SCOP", nullptr, nullptr);
            if (!_window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(1);
            }

            glfwMakeContextCurrent(_window);

            glfwSetKeyCallback(_window, App::keyCallback);
            glfwSetMouseButtonCallback(_window, App::mouseButtonCallback);
            glfwSetCursorPosCallback(_window, App::cursorPositionCallback);
            glfwSetScrollCallback(_window, App::scrollCallback);

            if (glewInit() != GLEW_OK) {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                exit(1);
            }
        }

        void run(std::function<void()> loop) {
            while (!glfwWindowShouldClose(_window)) {
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
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            // Handle mouse button events here
        }

        static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
            // Handle cursor position events here
        }

        static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
            // Handle scroll events here
        }

    private:
        GLFWwindow* _window;

};

}