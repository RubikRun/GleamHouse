#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uOpacity;

void main()
{
   vec4 texColor = texture(uTexture, vTexCoord);
   FragColor = mix(vec4(1.0, 1.0, 1.0, 0.0), texColor, min(uOpacity, 1.0));
}
