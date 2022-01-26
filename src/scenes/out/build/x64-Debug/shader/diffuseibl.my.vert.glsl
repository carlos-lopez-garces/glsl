#version 430

layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 TexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;

layout (location = 0) out vec2 UVCoord;
layout (location = 1) out vec3 WorldNormal;

void main() {
  UVCoord = TexCoord;
  WorldNormal = vec3(ModelMatrix * vec4(VertexNormal, 0.0));
  gl_Position = MVP * VertexPosition;
}