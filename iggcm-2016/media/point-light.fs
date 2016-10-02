#version 330 core

uniform sampler2D gb0;
uniform sampler2D gb1;
uniform sampler2D gb2;
uniform vec2 screenSize;

uniform vec3 lightPosition;

uniform float constant;
uniform float linear;
uniform float exponent;

out vec4 oColour;

vec2 calculateScreenCoord()
{
    return gl_FragCoord.xy / screenSize;
}

void main()
{
    vec2 screenCoord = calculateScreenCoord();

    vec3 colour = texture(gb0, screenCoord).rgb;
    vec3 position = texture(gb1, screenCoord).rgb;
    vec3 normal = texture(gb2, screenCoord).rgb;

    // Calculate shading
    vec3 direction = lightPosition - position;
    float diffuse = max(dot(normal, normalize(direction)), 0.0);
    float distance = length(direction);
    float attenuation = constant + linear + exponent;
    vec3 result = colour * diffuse / attenuation;

    oColour = vec4(colour, 1.0);
}
