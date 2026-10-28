#version 460

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

out vec2 TexCoord0;
out vec3 Normal0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool useTexture;
uniform vec3 solidColor;

void main()
{
    gl_Position = projection * view * model * vec4(Position, 1.0);
    TexCoord0 = vec2(TexCoord.x, 1.0 - TexCoord.y);
    Normal0 = mat3(model) * Normal;
}
