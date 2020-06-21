#version 410

layout (location = 0) in vec3 outgoingRadiance;

layout (location = 0) out vec4 fragColor;

void main() {
	fragColor = vec4(outgoingRadiance, 1.0);
}