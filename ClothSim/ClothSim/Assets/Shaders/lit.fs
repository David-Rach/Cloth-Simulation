#version 330 core

out vec4 fragment;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D Texture01;

void main(){
	vec3 LightColor = vec3(1,1,1);
	vec3 ObjectColor = vec3(1,1,1);

	float ambientStrength = 0.25;
	vec3 ambient = ambientStrength * LightColor;

	vec3 norm = normalize(Normal);
	vec3 LightDirection = normalize(vec3(0.5,0.5,0.5));

	float diff = max(dot(norm, LightDirection), 0.0);
	vec3 diffuse = diff * LightColor;

//	fragment = vec4(.9,.9,.9,1.0f);
	vec3 result = (ambient + diffuse) * ObjectColor;
	fragment = texture(Texture01,TexCoords) * vec4(result,1.0);
}