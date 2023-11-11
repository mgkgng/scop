#pragma once

#include <GL/glew.h>
#include <vector>

#include "Parser.hpp"

namespace scop {

class Mesh {
    public:
        Mesh(scop::Parser const &parser);
        ~Mesh();

        void draw() const;

    private:
        GLuint _vao, _vbo;

        
};

}