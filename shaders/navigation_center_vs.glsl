#version 460

layout (location = 0) in vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3    pan;
uniform float   zoom;

void main()
{
    vec3 pos = (Position * zoom * 0.1) + pan;
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
