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
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <obj file> [<texture file>]" << std::endl;
        return -1;
    }

    std::unique_ptr<Parser> parser;
    std::unique_ptr<Shader> shader;

    try {
        parser = std::make_unique<Parser>(argv[1], argc == 3 ? argv[2] : "assets/textures/wood.bmp");
        std::cout << "Parsing done successfully" << std::endl;
        // std::cout << *parser << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to parse file: " << argv[1] << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    auto objects = parser->getObjects();
    if (!objects.size()) {
        std::cerr << "No object found in file: " << argv[1] << std::endl;
        return 1;
    }
    App app(objects);

    try {
        shader = std::make_unique<Shader>(
            "shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl",
            app.hasNormals, 
            &app.textureState,
            parser->getTexture());
        std::cout << "Shader compilation done successfully" << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "Failed to compile shaders" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    app.run([&]() {
        shader->use();

        if (app.isInTransition) {
            glActiveTexture(GL_TEXTURE0); // Activate the first texture unit
            glBindTexture(GL_TEXTURE_2D, shader->_textureId); // Bind the texture
            app.updateTextureTransition(shader->getId());
        }
        shader->setMat4("model", app._transform->modelMat);
        shader->setMat4("view", app._transform->viewMat);
        shader->setMat4("projection", app._transform->projectionMat);

        app._mesh->draw();

    });

    return 0;
}