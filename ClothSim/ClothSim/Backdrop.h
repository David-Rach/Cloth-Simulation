#ifndef BACKDROP_H
#define BACKDROP_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <SOIL.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>

class CBackdrop
{
public:
	CBackdrop(std::string path);
	~CBackdrop();
	void AddTexture(std::string path);
	void Render(GLuint SHADER);

private:
	enum { POSITION,TEXTURE_COORDS, NUMOFBUFFERS };

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_UVs;

	GLuint m_texture;
};
#endif