#version 410

layout (location = 0) in vec3 outgoingRadiance;

layout (location = 0) out vec3 fragColor;

void main() {
	fragColor = outgoingRadiance;
}