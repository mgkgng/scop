#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragPos;
out vec3 normal;

uniform int hasNormals;

void main() {
    gl_Position = gl_Position = projection * view * model * aPos;
    fragPos = aPos;

    if (hasNormals > 0)
        normal = mat3(model) * aNormal;
    else
        normal = vec3(0.0, 0.0, 1.0);
}