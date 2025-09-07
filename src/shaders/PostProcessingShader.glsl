#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

// Max allowed number of lights in the scene
const int MAX_LIGHTS = 32;
// Number of lights in the scene
uniform int uLightsCount;

// List of light positions, in window space
uniform vec2 uLightPositions[MAX_LIGHTS];
// List of light intensities (between 0 and 1)
uniform float uLightIntensities[MAX_LIGHTS];
// List of light colors
uniform vec3 uLightColors[MAX_LIGHTS];
// List of light radii, in pixels
uniform float uLightRadii[MAX_LIGHTS];
// List of light sharpnesses (between 0 and 1)
uniform float uLightSharpnesses[MAX_LIGHTS];

// Window's resolution
uniform vec2 uResolution;

float gaussianFalloff(float distance, float radius, float sharpness)
{
    float x = distance / radius;
    return pow(exp(-x * x), sharpness);
}

void main()
{
    vec3 baseColor = texture(screenTexture, texCoords).rgb;
    vec2 fragInWindow = texCoords * uResolution;

    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < uLightsCount; i++)
    {
        float distance = length(fragInWindow - uLightPositions[i]);
        float attenuation = gaussianFalloff(distance, uLightRadii[i], uLightSharpnesses[i]);
        lightSum += uLightColors[i] * uLightIntensities[i] * attenuation;
    }

    vec3 finalColor = baseColor * min(0.004 + lightSum, 1.0);
    FragColor = vec4(finalColor, 1.0);
}
