#version 460 core

in vec2 TexCoord0;
out vec4 FragColor;
uniform sampler2D gSampler;

uniform bool useTexture;
uniform vec3 solidColor;

void main() {
    if (useTexture) {
        FragColor = texture(gSampler, TexCoord0);
    } else {
        FragColor = vec4(solidColor, 1.0);
    }
}
