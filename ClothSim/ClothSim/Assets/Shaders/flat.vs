#version 330 core
layout(location = 0) in vec3 Vertex_Position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main()
{
		gl_Position = Projection * View * Model * vec4(Vertex_Position,1);
}