#version 430

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTexCoord;
layout(location = 3) in vec4 VertexTangent;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
uniform struct LightInfo {
  // Already in view space.
  vec4 Position;
  vec3 L;
  vec3 La;
} Light;

out vec3 LightDir;
out vec2 TexCoord;
out vec3 ViewDir;

void outputInTangentSpace(
  in vec3 viewPosition, in vec3 viewNormal, in vec3 viewTangent, in vec3 viewBinormal,
  out vec3 lightDir, out vec3 viewDir
) {
  mat3 tangentMatrix = mat3(
    viewTangent.x, viewBinormal.x, viewNormal.x,
    viewTangent.y, viewBinormal.y, viewNormal.y,
    viewTangent.z, viewBinormal.z, viewNormal.z
  );

  vec3 tangentSpacePosition = tangentMatrix * viewPosition;

  lightDir = normalize(tangentMatrix * (vec3(Light.Position) - viewPosition));
  viewDir = -normalize(tangentMatrix * viewPosition);
}

void outputInViewSpace(
  in vec3 viewPosition,
  out vec3 lightDir, out vec3 viewDir
) {
  lightDir = normalize(vec3(Light.Position) - viewPosition);
  viewDir = normalize(-viewPosition);
}

void main() {
  vec3 viewPosition = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));
  vec3 viewNormal = normalize(NormalMatrix * VertexNormal);
  vec3 viewTangent = normalize(NormalMatrix * vec3(VertexTangent));
  vec3 viewBinormal = normalize(cross(viewNormal, viewTangent)) * VertexTangent.w;

  outputInTangentSpace(viewPosition, viewNormal, viewTangent, viewBinormal, LightDir, ViewDir);
  //outputInViewSpace(viewPosition, LightDir, ViewDir);
  
  TexCoord = VertexTexCoord;

  gl_Position = MVP * vec4(VertexPosition, 1.0);
}