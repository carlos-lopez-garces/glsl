#version 410

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

uniform struct MaterialInfo {
	// Ka and Kd have the same value. Both are the surface's reflectivity coefficient.
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

uniform struct LightInfo {
    // Ambient, diffuse, and specular light intensities.
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec4 Position;
} Light;

layout (location = 0) out vec3 outgoingRadiance;

void main() {
	vec4 viewVertexPosition = ModelViewMatrix * VertexPosition;
	vec3 viewVertexNormal = normalize(NormalMatrix * VertexNormal);
	vec4 viewLightPosition = Light.Position;
	vec3 viewLightDirection = normalize(vec3(viewLightPosition - viewVertexPosition));

	vec3 specularReflectionDirection = reflect(-viewLightDirection, viewVertexNormal);
	vec3 viewCameraDirection = normalize(vec3(-viewVertexPosition));
	float specularFalloff = pow(clamp(dot(specularReflectionDirection, viewCameraDirection), 0.0, 1.0), Material.Shininess);

	vec3 ambientComponent = Material.Ka * Light.La;

	vec3 diffuseComponent = Material.Kd * Light.Ld * clamp(dot(viewVertexNormal, viewLightDirection), 0.0, 1.0);

	vec3 specularComponent = Material.Ks * Light.Ls * specularFalloff;

	outgoingRadiance = ambientComponent + diffuseComponent + specularComponent;

	gl_Position = MVP * VertexPosition;
}