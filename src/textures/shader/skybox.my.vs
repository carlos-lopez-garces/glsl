#version 430

layout (location = 0) in vec4 VertexPosition;

uniform mat4 ModelMatrix;
uniform mat4 MVP;

out vec3 position;

void main() {
	position = vec3(ModelMatrix * VertexPosition);
	gl_Position = MVP * VertexPosition;
}