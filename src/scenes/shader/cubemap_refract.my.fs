#version 430

in vec3 RefractedDirection;
in vec3 ReflectedDirection;

layout (location = 0) out vec4 FragColor;

uniform samplerCube cubemap;

uniform struct MaterialInfo {
	float Eta;
	float ReflectionFactor;
} Material;

void main() {
	vec4 refractionColor = texture(cubemap, RefractedDirection);
	vec4 reflectionColor = texture(cubemap, ReflectedDirection);
	vec4 interpolatedColor = mix(refractionColor, reflectionColor, Material.ReflectionFactor);
	vec4 gammaCorrectedColor = pow(interpolatedColor, vec4(1.0/2.2));
	FragColor = gammaCorrectedColor;
}