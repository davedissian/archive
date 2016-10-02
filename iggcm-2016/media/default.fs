#version 330 core

in vec3 oWorldPos;
in vec3 oNormal;

// Format:
// | R | G | B | A |
// | Diffuse   |
// | Position  |
// | Normal    |
layout (location = 0) out vec4 gb0;
layout (location = 1) out vec4 gb1;
layout (location = 2) out vec4 gb2;

void main()
{
    gb0 = vec4(1.0, 1.0, 1.0, 1.0);
    gb1.rgb = oWorldPos;
    gb2.rgb = normalize(oNormal);
}
