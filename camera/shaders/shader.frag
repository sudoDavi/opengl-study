#version 330 core
out vec4 FragColor;

in vec3 userColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float texture2mix;

void main() {
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(-texCoord.x, texCoord.y)), texture2mix);
}
