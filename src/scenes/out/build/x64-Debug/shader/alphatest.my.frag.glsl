#version 430

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (location = 0) out vec3 FragColor;

uniform struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
} CementMaterial;

uniform struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
} ChinaMaterial;

uniform struct LightInfo {
	vec4 Position;
} Light;

uniform sampler2D CementTex;
uniform sampler2D MossTex;

vec3 phongReflectionModel(vec3 baseColor, MaterialInfo material) {
	vec3 lightVector = normalize(vec3(Light.Position - Position));
	vec3 reflectedLightVector = reflect(-lightVector, Normal);
	vec3 viewCameraDirection = normalize(vec3(-Position));

	vec3 ambientComponent
		= material.La
		* material.Ka;

	vec3 diffuseComponent
		= material.Ld
		* material.Kd
		* clamp(dot(Normal, lightVector), 0.0, 1.0);

	vec3 specularComponent
		= material.Ls
		* material.Ks
		* pow(clamp(dot(viewCameraDirection, reflectedLightVector), 0.0, 1.0), material.Shininess);

	return baseColor * (ambientComponent + diffuseComponent) + specularComponent;
}

void main() {
  vec4 cementColor = texture(CementTex, TexCoord);
  vec4 mossColor = texture(MossTex, TexCoord);

  if (mossColor.a >= 0.5) {
	  FragColor = phongReflectionModel(vec3(252.0/256.0, 16 / 256.0, 127 / 256.0), ChinaMaterial);
	  // Or discard.
  }
  else {
	FragColor = phongReflectionModel(vec3(cementColor), CementMaterial);
  }
}