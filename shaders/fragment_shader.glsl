#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 fragPos;
in vec3 normal;

// uniform Material material;
uniform int hasNormals;
uniform float isTextureEnabled;
uniform sampler2D textureSampler;

void main() {
    vec4 baseColor;
    vec4 textureColor;

    // Base color when no texture or normals
    if (hasNormals == 0) {
        float greyScale = (1.0 + gl_PrimitiveID % 4) / 5.0;
        baseColor = vec4(greyScale, greyScale, greyScale, 1.0);
    } else {
        // Default base color
        baseColor = vec4(1.0, 1.0, 1.0, 1.0);
    }

    // Calculate texture color
    float scaleFactor = 1.0;
    vec2 texCoords = mod(fragPos.xy * scaleFactor, 1.0);
    textureColor = texture(textureSampler, texCoords);

    // Transition
    vec4 colorOutput = mix(baseColor, textureColor, isTextureEnabled);

    // Apply lighting if normals are present
    if (hasNormals > 0) {
        vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0));
        float diff = max(dot(normal, lightDirection), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        FragColor = vec4(diffuse, 1.0) * colorOutput;
    } else {
        FragColor = colorOutput;
    }
}
