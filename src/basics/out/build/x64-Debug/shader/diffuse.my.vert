#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

// Diffuse reflectivity coefficient.
uniform vec3 Kd;
// Light intensity.
uniform vec3 Ld;
// Comes in view space.
uniform vec4 LightPosition;

layout (location = 0) out vec3 outgoingRadiance;

void main() {
	vec4 viewLightPosition = LightPosition;
	vec4 viewVertexPosition = ModelViewMatrix * vec4(VertexPosition, 1.0);
	vec3 viewVertexNormal = normalize(NormalMatrix * VertexNormal);
	vec3 viewLightDirectionFromVertex = normalize(vec3(viewLightPosition - viewVertexPosition));

	float cosNL = dot(viewLightDirectionFromVertex, viewVertexNormal);

	outgoingRadiance = Ld * Kd * max(cosNL, 0.0);

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}