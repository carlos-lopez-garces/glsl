#version 410

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform mat3 NormalMatrix;

// Comes in view space.
uniform vec4 LightPosition;

layout (location = 0) out vec3 outgoingRadiance;

void main() {
	vec4 viewLightPosition = LightPosition;
	vec4 viewVertexPosition = ModelViewMatrix * vec4(VertexPosition, 1.0);
	vec3 viewVertexNormal = normalize(NormalMatrix * VertexNormal);
	vec3 viewLightDirectionFromVertex = normalize(vec3(viewLightPosition - viewVertexPosition));

	vec3 coolColor = vec3(0.0, 0.0, 0.55);
	vec3 warmColor = vec3(0.3, 0.3, 0.0);
	vec3 highlightColor = vec3(1.0, 1.0, 1.0);

	// Color temperature interpolator.
	float t = (dot(viewVertexNormal, viewLightDirectionFromVertex) + 1) / 2;
	
	vec3 r = 2 * dot(viewVertexNormal, viewLightDirectionFromVertex) * viewVertexNormal - viewLightDirectionFromVertex;

	// Highlight interpolator.
	float s = clamp(100 * dot(r, vec3(viewVertexPosition)) - 97, 0.0, 1.0);

	vec3 interpolatedColor = t*warmColor + (1-t)*coolColor;

	s = 0.0;
	outgoingRadiance = s*highlightColor + (1-s)*(interpolatedColor);

	gl_Position = MVP * vec4(VertexPosition, 1.0);
}