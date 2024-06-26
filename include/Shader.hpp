#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Matrix.hpp"
// #include "BMP.hpp"

class Shader {
    public:
        GLuint _textureId;

        Shader(
            const char *vertexPath, 
            const char *fragmentPath, 
            bool hasNormals, 
            float *textureState,
            BMP &texture
        ) {
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

            glUseProgram(_id);

            GLuint hasNormalsLoc = glGetUniformLocation(_id, "hasNormals");
            glUniform1i(hasNormalsLoc, hasNormals);
            GLint textureStateLoc = glGetUniformLocation(_id, "textureStateLoc");
			glUniform1f(textureStateLoc, *textureState);

            loadTexture(texture);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            std::cout << "Shader created successfully" << std::endl;

        }
        ~Shader() { glDeleteProgram(_id); }

        void loadTexture(BMP &texture) {
            glGenTextures(1, &_textureId);
            glBindTexture(GL_TEXTURE_2D, _textureId);

            /* Set texture parameters */
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            /* Upload texture data */
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data.data());
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void use() {
            glUseProgram(_id);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, _textureId);
        }
        GLuint getId() const { return _id; }

        void update(bool hasNormals) {
            if (hasNormals) {
                GLuint hasNormalsLoc = glGetUniformLocation(_id, "hasNormals");
                glUniform1i(hasNormalsLoc, 1);
            }
        }

        void setMat4(const std::string &name, Matrix &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, mat.get_data());
        }

        void setVec3(const std::string &name, const std::array<float, 3> &vec) const {
            glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, vec.data());
        }

        void setTexture(const std::string &name, int textureUnit) const {
            glUniform1i(glGetUniformLocation(_id, name.c_str()), textureUnit);
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