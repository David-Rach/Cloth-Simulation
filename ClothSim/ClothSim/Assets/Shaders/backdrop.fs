#version 330 core

out vec4 fragment;

in vec3 FragPos;
in vec2 TexCoords;


uniform sampler2D Texture01;

void main(){
	fragment = texture(Texture01,TexCoords);
	//fragment = vec4(1.0,0.0,0.0,1.0);
}