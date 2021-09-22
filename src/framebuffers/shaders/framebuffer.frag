#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{
	vec4 screenPixel = texture(screenTexture, TexCoords);
	float average = (screenPixel.r + screenPixel.g + screenPixel.b) / 3.0;
	FragColor = vec4(average, average, average, 1.0);
}
