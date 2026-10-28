#version 460 core

in  vec2    TexCoord0;
out vec4    FragColor;
in  vec3    Normal0;

uniform sampler2D   gSampler;
uniform bool        useShadeless;
uniform bool        useTexture;
uniform vec3        solidColor;
uniform vec3        lightDir;

void main() {
    vec3 n = normalize(Normal0);
    float ndotl = max(dot(n, lightDir), 0.0);

    float ambient = 0.2;
    float brightness = ambient + (1.0 - ambient) * ndotl;

    /* use texture */
    if (useTexture) {
        if (useShadeless) {
            FragColor = texture(gSampler, TexCoord0);
        } else {
            FragColor = vec4(texture(gSampler, TexCoord0).rgb * brightness, 1.0);
        }

    /* use solid color */
    } else {
        if (useShadeless) {
            FragColor = vec4(solidColor, 1.0);
        } else {
            FragColor = vec4(solidColor * brightness, 1.0);
        }
    }

}
