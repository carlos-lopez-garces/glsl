#version 430

layout (location = 0) in vec2 UVCoord;
layout (location = 1) in vec3 WorldNormal;

uniform sampler2D colorTex;
uniform samplerCube diffCube;
uniform struct MaterialInfo {
  vec3 Color;
  float Kd;
} Material;

layout (location = 0) out vec4 FragColor;

void main() {
  vec4 Irradiance = texture(diffCube, WorldNormal);
  vec4 BaseColor = texture(colorTex, UVCoord);
  FragColor = mix(
    BaseColor,
    BaseColor * Irradiance,
    Material.Kd
  );
  FragColor = BaseColor;
}