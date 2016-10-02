#version 330 core

uniform mat4 wvp;
uniform mat4 world;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 oWorldPos;
out vec3 oNormal;

void main()
{
    gl_Position = wvp * vec4(position, 1.0);
    oWorldPos = (world * vec4(position, 1.0)).xyz;
    oNormal = (world * vec4(normal, 0.0)).xyz;
}
