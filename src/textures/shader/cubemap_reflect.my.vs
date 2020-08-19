#version 430

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;
out vec3 WorldReflectDir;
out vec3 ViewReflectDir;

uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform vec3 WorldCameraPosition;

void main() {
	vec3 WorldVertexPosition = vec3(ModelMatrix * VertexPosition);
	// According to the book, it's OK to transform the normal using the
	// model matrix because it doesn't contain non-uniform scaling components.
	vec3 WorldVertexNormal = vec3(ModelMatrix * vec4(VertexNormal, 0.0));
	vec3 WorldVertexToCameraDir = WorldCameraPosition - WorldVertexPosition;

	WorldReflectDir = reflect(-WorldVertexToCameraDir, WorldVertexNormal);

	// We compute the direction of reflection in view space too to show that
	// it yields incorrect results. The reflection will be static, even when
	// camera rotates around the teapot. This is because both the camera's
	// position and the vertex position remain the same across frames, so you
	// always compute the same direction vector, which samples the same texel
	// of the cubemap.
	vec3 ViewVertexPosition = vec3(ModelViewMatrix * VertexPosition);
	vec3 ViewVertexNormal = NormalMatrix * VertexNormal;

	ViewReflectDir = reflect(ViewVertexPosition, ViewVertexNormal);

	Normal = NormalMatrix * VertexNormal;
	Position = ModelViewMatrix * VertexPosition;
	gl_Position = MVP * VertexPosition;
}