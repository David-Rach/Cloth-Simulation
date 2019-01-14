#version 330 core
layout(location = 0) in vec3 Vertex_Position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 bPosition;

out vec3 Normal;
out vec3 FragPos;

void main()
{
		//Make MVP
		mat4 MVP = Projection * View * Model;
		FragPos = vec3(Model * vec4(Vertex_Position,1.0));
		Normal = normalize(Vertex_Position - bPosition);
		gl_Position = MVP * vec4(Vertex_Position,1);
}