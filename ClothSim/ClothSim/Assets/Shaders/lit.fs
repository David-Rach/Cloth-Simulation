#version 330 core

out vec4 fragment;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in mat3 toTangentMat;

uniform sampler2D Texture01;
uniform sampler2D Texture02;

void main(){
	//Normal maps
	float scale = 1;
	vec3 NewNormal = texture(Texture02, TexCoords * scale).rgb;
	NewNormal = normalize(NewNormal * 2.0 - 1.0);
	NewNormal = normalize(NewNormal * toTangentMat) ;

	vec3 LightColor = vec3(1,1,1);
	vec3 ObjectColor = vec3(1,1,1);

	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * LightColor;

	vec3 norm = normalize(NewNormal);
	vec3 LightDirection = normalize(vec3(0.5,0.5,0));

	float diff = max(dot(norm, LightDirection), 0.0);
	vec3 diffuse = diff * LightColor;

	//fragment = vec4(NewNormal,1.0f);
	vec3 result = (ambient + diffuse) * ObjectColor;
	fragment = texture(Texture01,TexCoords * scale) * vec4(result,1.0);
}