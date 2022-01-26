#version 430

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 RefractedDirection;
out vec3 ReflectedDirection;

uniform mat4 ModelMatrix;
uniform mat4 MVP;

uniform vec3 WorldCameraPosition;

uniform struct MaterialInfo {
	float Eta;
	float ReflectionFactor;
} Material;

void main() {
	vec3 WorldVertexPosition = vec3(ModelMatrix * VertexPosition);
	// According to the book, it's OK to transform the normal using the
	// model matrix because it doesn't contain non-uniform scaling components.
	vec3 WorldVertexNormal = normalize(vec3(ModelMatrix * vec4(VertexNormal, 0.0)));
	vec3 WorldVertexToCameraDir = normalize(WorldCameraPosition - WorldVertexPosition);

	RefractedDirection = refract(-WorldVertexToCameraDir, WorldVertexNormal, Material.Eta);
	
	ReflectedDirection = reflect(-WorldVertexToCameraDir, WorldVertexNormal);

	gl_Position = MVP * VertexPosition;
}