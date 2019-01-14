#version 330 core
layout(location = 0) in vec3 Vertex_Position;
layout(location = 1) in vec3 Vertex_Normal;
layout(location = 2) in vec2 UV_Coords;
layout(location = 3) in vec3 Tangents;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec3 UV_Normal;
out vec3 FragPos;
out vec2 TexCoords;

out mat3 toTangentMat;


void main()
{
		//Make MVP
		mat4 MVP = Projection * View * Model;
		
		FragPos = vec3(Model * vec4(Vertex_Position,1.0));
		Normal = Vertex_Normal;
		TexCoords = UV_Coords;
		
		vec3 SurfaceNormal = (Model * vec4(Vertex_Normal,0.0)).xyz;
		
		vec3 norm = normalize(SurfaceNormal);
		vec3 tang = normalize((Model * vec4(Tangents,0.0)).xyz);
		vec3 biTangent = cross(norm,tang);
		
		toTangentMat = mat3(
			tang.x, biTangent.x, norm.x,
			tang.y, biTangent.y, norm.y,
			tang.z, biTangent.z, norm.z
		);
		
		gl_Position = MVP * vec4(Vertex_Position,1);
}