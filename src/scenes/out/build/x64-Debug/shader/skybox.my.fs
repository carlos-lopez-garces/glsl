#version 430

in vec3 position;

out vec4 fragColor;

uniform samplerCube cubemap;

void main() {
	vec4 cubeMapColor = texture(cubemap, position);
	vec4 gammaCorrectedColor = pow(cubeMapColor, vec4(1.0/2.2));
	fragColor = gammaCorrectedColor;
}