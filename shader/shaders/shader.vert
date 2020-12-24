#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexPos;

uniform vec3 offsetPos;

void main() {
	vertexPos = vec3(aPos.x + offsetPos.x, -aPos.y + offsetPos.y, aPos.z + offsetPos.z);
    gl_Position = vec4(vertexPos, 1.0);
}
