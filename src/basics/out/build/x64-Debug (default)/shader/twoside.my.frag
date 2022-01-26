#version 410

in vec3 outgoingRadiance;

out vec3 fragColor;

void main() {
	fragColor = outgoingRadiance;
}