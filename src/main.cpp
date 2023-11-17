#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

#include "App.hpp"
#include "Parser.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <obj file>" << std::endl;
        return -1;
    }

    std::unique_ptr<scop::Parser> parser;
    std::unique_ptr<scop::Shader> shader;

    try {
        parser = std::make_unique<scop::Parser>(argv[1]);
        std::cout << "Parsing done successfully" << std::endl;
        // std::cout << *parser << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to parse file: " << argv[1] << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    scop::App app;
    app.init();

    try {
        shader = std::make_unique<scop::Shader>("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
        std::cout << "Shader compilation done successfully" << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to compile shaders" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    auto objects = parser->getObjects();
    auto mesh = std::make_unique<scop::Mesh>(objects["Cube"]);

    scop::Transform transform;

    float angle = 0.0f;

    app.run([&]() {    
        angle += 0.05f;
        if (angle > 360.0f) angle = 0.0f;

        transform.updateModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, angle, angle), glm::vec3(1.0f, 1.0f, 1.0f));

        shader->use();
        shader->setMat4("model", transform.model);
        shader->setMat4("view", transform.view);
        shader->setMat4("projection", transform.projection);

        glBindVertexArray(mesh->getVao());

        mesh->draw();
    });

    return 0;
}