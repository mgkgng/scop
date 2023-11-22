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

    std::unique_ptr<Parser> parser;
    std::unique_ptr<Shader> shader;

    try {
        parser = std::make_unique<Parser>(argv[1]);
        std::cout << "Parsing done successfully" << std::endl;
        // std::cout << *parser << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to parse file: " << argv[1] << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    auto objects = parser->getObjects();
    App app(objects);

    try {
        shader = std::make_unique<Shader>("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
        std::cout << "Shader compilation done successfully" << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to compile shaders" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    app.run([&]() {    
        shader->use();
        // shader.setVec3("material.ambient", glm::vec3(material._ambient.r, material._ambient.g, material._ambient.b));
        // shader.setVec3("material.diffuse", glm::vec3(material._diffuse.r, material._diffuse.g, material._diffuse.b));
        // shader.setVec3("material.specular", glm::vec3(material._specular.r, material._specular.g, material._specular.b));
        // shader.setFloat("material.shininess", material._specularExponent);

        shader->setMat4("model", app._transform->model);
        shader->setMat4("view", app._transform->view);
        shader->setMat4("projection", app._transform->projection);

        glBindVertexArray(app._mesh->getVao());

        app._mesh->draw();
    });

    return 0;
}