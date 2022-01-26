#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

uniform struct MaterialInfo {
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float Shininess;
} Material;

uniform struct LightInfo {
  vec3 La;
  vec3 Ld;
  vec3 Ls;
  vec4 Position;
} Light;

uniform sampler2D BrickTex;
uniform sampler2D MossTex;

vec4 phongReflectionModel(vec4 baseColor) {
  vec4 directionToLight = normalize(Light.Position - Position);
  vec4 directionToCamera = normalize(-Position);
  vec4 directionOfReflection = normalize(reflect(-directionToLight, vec4(Normal, 1.0)));

  vec4 ambientColor = vec4(Light.La * Material.Ka, 0.0);

  vec4 diffuseColor = vec4(Light.Ld * Material.Kd * vec3(clamp(dot(directionToLight, vec4(Normal, 1.0)), 0.0, 1.0)), 0.0);

  vec4 specularColor = vec4(Light.Ls * Material.Ks * pow(dot(directionOfReflection, directionToCamera), Material.Shininess), 0.0);

  return baseColor * (ambientColor + diffuseColor) + specularColor;
}

void main() {
  vec4 brickColor = texture(BrickTex, TexCoord);
  vec4 mossColor = texture(MossTex, TexCoord);
  vec4 baseColor = mix(brickColor, mossColor, mossColor.a);
  FragColor = phongReflectionModel(baseColor);
}