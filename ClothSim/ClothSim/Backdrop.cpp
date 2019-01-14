#include "Backdrop.h"

CBackdrop::CBackdrop(std::string path)
{
	/*Setup Buffers*/
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);

	/*Make a square for the background texture*/
	m_vertices.resize(6);

	m_vertices[0] = glm::vec3(-1,-1,0);
	m_vertices[1] = glm::vec3(-1, 1, 0);
	m_vertices[2] = glm::vec3(1, -1, 0);

	m_vertices[3] = glm::vec3(1, -1, 0);
	m_vertices[4] = glm::vec3(-1, 1, 0);
	m_vertices[5] = glm::vec3(1, 1, 0);

	/*Map coordinates*/
	m_UVs.resize(6);

	m_UVs[0] = glm::vec2(0, 1);
	m_UVs[1] = glm::vec2(0, 0);
	m_UVs[2] = glm::vec2(1, 1);

	m_UVs[3] = glm::vec2(0, 0);
	m_UVs[4] = glm::vec2(1, 1);
	m_UVs[5] = glm::vec2(0, 1);


	/*Set up GL calls*/
	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	AddTexture(path);
}

CBackdrop::~CBackdrop()
{

}

void CBackdrop::AddTexture(std::string path)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int Channels;
	unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &Channels, 0);

	if (data)
	{
		std::cout << "texture success" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		delete data;
	}
	else
	{
		std::cout << "texture failed" << std::endl;
	}

	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[TEXTURE_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, m_UVs.size() * sizeof(glm::vec2), &m_UVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void CBackdrop::Render(GLuint SHADER)
{
	glUseProgram(SHADER);

	glUniform1i(glGetUniformLocation(SHADER, "Texture01"), 0);

	/*Gonna use an element buffer*/
	glBindVertexArray(VertexArrayBuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glDrawArrays(GL_TRIANGLES,0,6);

	glBindVertexArray(0);
}
