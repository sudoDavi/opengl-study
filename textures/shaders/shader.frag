#version 330 core
out vec4 FragColor;

in vec3 userColor;
in vec2 texCoord;

uniform sampler2D userTexture;

void main() {
    FragColor = texture(userTexture, texCoord) * vec4(userColor, 1.0);
}
