#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "parsing/Parser.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <obj file>" << std::endl;
        return -1;
    }
    
    try {
        scop::Parser parser(argv[1]);

        std::ostringstream oss;
        oss << parser;
        std::cout << oss.str() << std::endl;

    } catch (std::exception const &e) {
        std::cerr << "Failed to parse file: " << argv[1] << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    

    // if (!glfwInit()) {
    //     std::cerr << "Failed to initialize GLFW" << std::endl;
    //     return -1;
    // }

    // GLFWwindow* window = glfwCreateWindow(640, 480, "SCOP", nullptr, nullptr);
    // if (!window) {
    //     std::cerr << "Failed to create GLFW window" << std::endl;
    //     glfwTerminate();
    //     return -1;
    // }

    // glfwMakeContextCurrent(window);

    // if (glewInit() != GLEW_OK) {
    //     std::cerr << "Failed to initialize GLEW" << std::endl;
    //     return -1;
    // }

    // while (!glfwWindowShouldClose(window)) {
    //     glClear(GL_COLOR_BUFFER_BIT);

    //     // Render your 3D objects here

    //     glfwSwapBuffers(window);
    //     glfwPollEvents();
    // }

    // glfwTerminate();
    // return 0;
}
