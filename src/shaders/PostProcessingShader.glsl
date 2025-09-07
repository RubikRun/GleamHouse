#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

const float baseLight = 0.004;

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
// List of light flags indicating if light is a star (0.0 or 1.0)
uniform float uLightIsStarFlags[MAX_LIGHTS];

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

    // Accumulate non-star lightness
    vec3 lightSum = vec3(0.0);
    for (int i = 0; i < uLightsCount; i++)
    {
        if (uLightIsStarFlags[i] > 0.0)
        {
            continue;
        }
        float distance = length(fragInWindow - uLightPositions[i]);
        float attenuation = gaussianFalloff(distance, uLightRadii[i], uLightSharpnesses[i]);
        lightSum += uLightColors[i] * uLightIntensities[i] * attenuation;
    }

    // Non-star lightness caps at 100%,
    // so cap it at 1.0 minus baseLight because we will always add baseLight at the end
    lightSum = min(lightSum, 1.0 - baseLight);

    // Accumulate star lightness
    for (int i = 0; i < uLightsCount; i++)
    {
        if (uLightIsStarFlags[i] < 1.0)
        {
            continue;
        }
        float distance = length(fragInWindow - uLightPositions[i]);
        float attenuation = gaussianFalloff(distance, uLightRadii[i], uLightSharpnesses[i]);
        lightSum += uLightColors[i] * uLightIntensities[i] * attenuation;
    }

    vec3 finalColor = baseColor * (lightSum + baseLight);
    FragColor = vec4(finalColor, 1.0);
}
