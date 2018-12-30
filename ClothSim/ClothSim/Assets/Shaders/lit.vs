#version 330 core
layout(location = 0) in vec3 Vertex_Position;
layout(location = 1) in vec3 Vertex_Normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
		gl_Position = Projection * View * Model * vec4(Vertex_Position,1);
		FragPos = vec3(Model * vec4(Vertex_Position,1.0));
		Normal = Vertex_Normal;
}