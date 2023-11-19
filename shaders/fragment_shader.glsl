#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main() {
    // Implement lighting calculations here
    // For example, using the Phong lighting model
    vec3 ambientLight = material.ambient * vec3(0.1); // Example ambient light calculation
    FragColor = vec4(ambientLight, 1.0);
}
