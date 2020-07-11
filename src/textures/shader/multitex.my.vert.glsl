#version 430

uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform mat4 ModelViewMatrix;

layout (location = 0) in vec4 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

layout (location = 0) out vec4 Position;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 TexCoord;

void main() {
  Normal = NormalMatrix * VertexNormal;
  TexCoord = VertexTexCoord;
  Position = ModelViewMatrix * VertexPosition;
  gl_Position = MVP * VertexPosition;
}