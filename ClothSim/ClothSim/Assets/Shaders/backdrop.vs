#version 330 core
layout(location = 0) in vec3 Vertex_Position;
layout(location = 1) in vec2 UV_Coords;


out vec3 FragPos;
out vec2 TexCoords;



void main()
{
		FragPos = Vertex_Position;
		TexCoords = UV_Coords;
		
		gl_Position = vec4(Vertex_Position,1);
}