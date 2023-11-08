#include "glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "parsing/Parser.hpp"

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // see how we directly give a vec3 to vec4's constructor
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // we declare an output variable called FragColor of type vec4
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // set the output variable to a dark-red color
"}\n\0";

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <obj file>" << std::endl;
        return -1;
    }
    
    try {
        scop::Parser parser(argv[1]);
        std::cout << "Obj file successfully parsed." << std::endl;
        // std::ostringstream oss;
        // oss << parser;
        // std::cout << oss.str() << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Error. Failed to parse file: " << argv[1] << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };

    GLFWwindow* window = glfwCreateWindow(640, 480, "SCOP", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window); // Context: object that stores all of OpenGL's state

    gladLoadGL();

    glViewport(0, 0, 640, 480);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram); 
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO); // Generate a VAO
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the newly created buffer to the GL_ARRAY_BUFFER target
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.07f, 0.63f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); // Clean the back buffer and assign new color to it.
    glfwSwapBuffers(window); // Swap the back buffer with the front buffer.

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.63f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); // Clean the back buffer and assign new color to it.
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window); // Swap the back buffer with the front buffer.
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
