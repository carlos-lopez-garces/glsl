#version 410

in vec4 VertexPosition;
in vec3 VertexNormal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

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

out vec3 outgoingRadiance;

void getViewSpace(out vec3 viewVertexPosition, out vec3 viewVertexNormal) {
	viewVertexPosition = vec3(ModelViewMatrix * VertexPosition);
	viewVertexNormal = normalize(NormalMatrix * VertexNormal);
};

vec3 phongModel(vec3 viewVertexPosition, vec3 viewVertexNormal) {
	vec3 ambientComponent = Material.Ka * Light.La;

	vec3 viewLightDirection = normalize(vec3(Light.Position)-viewVertexPosition);
	float lightDirDotNormal = dot(viewLightDirection, viewVertexNormal);
	vec3 viewCameraDirection = normalize(-viewVertexPosition);
	float cameraDotNormal = dot(viewCameraDirection, viewVertexNormal);
	if (cameraDotNormal < 0.0) {
		lightDirDotNormal = -lightDirDotNormal;
	}
	vec3 diffuseComponent = Material.Kd * Light.Ld * lightDirDotNormal;

	int viewVertexNormalSign = 1;
	if (cameraDotNormal < 0.0) {
		viewVertexNormalSign = -1;
	}
	vec3 reflectionDirection = reflect(-viewLightDirection, viewVertexNormalSign*viewVertexNormal);
	vec3 specularComponent 
		= Material.Ks 
		  * Light.Ls 
		  * pow(clamp(dot(reflectionDirection, viewCameraDirection), 0.0, 1.0), Material.Shininess);

	return ambientComponent + diffuseComponent + specularComponent;
}

void main() {
	vec3 viewVertexPosition;
	vec3 viewVertexNormal;
	getViewSpace(viewVertexPosition, viewVertexNormal);
	outgoingRadiance = phongModel(viewVertexPosition, viewVertexNormal);
	gl_Position = MVP * VertexPosition;
};