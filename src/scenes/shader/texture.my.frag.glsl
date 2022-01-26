#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoord;

layout (location = 0) out vec3 fragColor;

uniform struct LightInfo {
	vec3 La;
	vec3 Ld;
	vec3 Ls;
	vec4 Position;
} Light;

uniform struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

uniform sampler2D Tex1;

vec3 phongReflectionModel() {
	vec3 lightVector = normalize(vec3(Light.Position) - Position);
	vec3 reflectedLightVector = reflect(-lightVector, Normal);
	vec3 viewCameraDirection = normalize(vec3(-Position));

	vec3 baseColor = vec3(texture(Tex1, TexCoord));

	vec3 ambientComponent 
		= Light.La 
		  * Material.Ka;
		  
	vec3 diffuseComponent 
		= Light.Ld
		   * Material.Kd
		   * clamp(dot(Normal, lightVector), 0.0, 1.0);

	vec3 specularComponent 
		= Light.Ls
		  * Material.Ks 
		  * pow(clamp(dot(viewCameraDirection, reflectedLightVector), 0.0, 1.0), Material.Shininess);

	return baseColor*(ambientComponent + diffuseComponent) + specularComponent;
}

vec3 phongReflectionModelBaseColorAsReflectivityCoefficients() {
	vec3 lightVector = normalize(vec3(Light.Position) - Position);
	vec3 reflectedLightVector = reflect(-lightVector, Normal);
	vec3 viewCameraDirection = normalize(vec3(-Position));

	vec3 baseColor = vec3(texture(Tex1, TexCoord));

	vec3 Ka = baseColor;
	vec3 ambientComponent
		= Light.La
		* Ka;
	
	vec3 Kd = baseColor;
	vec3 diffuseComponent
		= Light.Ld
		* Ka
		* clamp(dot(Normal, lightVector), 0.0, 1.0);

	vec3 Ks = baseColor;
	vec3 specularComponent
		= Light.Ls
		* Ks
		* pow(clamp(dot(viewCameraDirection, reflectedLightVector), 0.0, 1.0), Material.Shininess);

	return ambientComponent + diffuseComponent + specularComponent;
}

void main() {
	fragColor = phongReflectionModel();
}