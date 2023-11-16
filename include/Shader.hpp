#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace scop {

class Shader {
    public:
        Shader(const char *vertexPath, const char *fragmentPath) {
            std::string vertexSource = getFileString(vertexPath);
            std::string fragmentSource = getFileString(fragmentPath);

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            compileShader(vertexShader, vertexSource.c_str());
            compileShader(fragmentShader, fragmentSource.c_str());

            _id = glCreateProgram();
            glAttachShader(_id, vertexShader);
            glAttachShader(_id, fragmentShader);
            glLinkProgram(_id);

            GLint success;
            GLchar infoLog[512];
            glGetProgramiv(_id, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(_id, 512, nullptr, infoLog);
                std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }
        ~Shader() { glDeleteProgram(_id); }

        void use() { glUseProgram(_id); }
        GLuint getId() const { return _id; }

        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
        }
    private:
        GLuint _id;

        Shader() {};
        Shader(Shader const &src) = delete;

        std::string getFileString(const char *path) {
            std::ifstream file;
            std::stringstream ss;

            file.open(path);
            if (!file.is_open()) {
                std::cerr << "Failed to open file: " << path << std::endl;
                throw std::exception();
            }
            
            ss << file.rdbuf();
            file.close();

            return ss.str();
        }

        void compileShader(GLuint shader, const char *shaderSource) {
            glShaderSource(shader, 1, &shaderSource, nullptr);
            glCompileShader(shader);

            GLint success;
            GLchar infoLog[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
};

}