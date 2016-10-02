#version 330 core

uniform mat4 wvp;

layout (location = 0) in vec3 position;

void main()
{
    gl_Position = wvp * vec4(position, 1.0);
}
