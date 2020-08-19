#version 430

in vec4 Position;
in vec3 Normal;
in vec3 WorldReflectDir;
in vec3 ViewReflectDir;

out vec4 fragColor; 

uniform vec4 MaterialColor;
uniform float ReflectFactor;
uniform samplerCube cubeMap;

void main() {
	vec4 cubeMapColor = texture(cubeMap, WorldReflectDir);
	// Gamma-corrected.
	vec4 gammaCorrectedColor = pow(cubeMapColor, vec4(1.0/2.2));
    fragColor = mix(MaterialColor, gammaCorrectedColor, ReflectFactor);
}