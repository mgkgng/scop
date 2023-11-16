#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace scop {

class App {
    public:
        GLFWwindow* window;

        App() {}
        ~App() { glfwTerminate(); }
        
        void init() {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                exit(1);
            }

            window = glfwCreateWindow(960, 640, "SCOP", nullptr, nullptr);
            if (!window) {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                exit(1);
            }

            glfwMakeContextCurrent(window);

            if (glewInit() != GLEW_OK) {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                exit(1);
            }
        }
};

}