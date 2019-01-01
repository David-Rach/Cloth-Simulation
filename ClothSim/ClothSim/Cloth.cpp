#include "Cloth.h"

CCloth::CCloth(int m_width, int m_height)
{
	this->m_height = m_height;
	this->m_width = m_width;

	m_NumOfParticles = m_height * m_width;
	m_vertices.resize(m_NumOfParticles);
	m_normals.resize(m_NumOfParticles);

	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			//float xpos = x - m_width / 2.f , ypos = y - m_height / 2.f ;
			m_ArrayOfParticles.push_back(new CParticle(glm::vec3(x - (m_width/2) , y - (m_height/2) , 0)));
		}
	}

	/*Setup Buffers*/
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);
}

CCloth::~CCloth()
{
	for (size_t i = 0; i < m_NumOfParticles; i++)
	{
		delete m_ArrayOfParticles[i];
	}
}

void CCloth::Update(float deltaTime)
{
	/*stream the vertices*/
	for (unsigned int i = 0; i < m_ArrayOfParticles.size(); i++)
	{
		m_vertices[i] = m_ArrayOfParticles[i]->GetPosition();
	}

	/*Stream the indices*/
	for (unsigned int x = 0; x < m_width - 1; x++)
	{
		for (unsigned int z = 0; z < m_height - 1; z++)
		{
			//First triangle
			m_indices.push_back(x * m_height + z);
			m_indices.push_back(x * m_height + z + 1);
			m_indices.push_back((x + 1) * m_height + z);

			//Second triangle
			m_indices.push_back((x + 1) * m_height + z);
			m_indices.push_back(x * m_height + z + 1);
			m_indices.push_back((x + 1) * m_height + z + 1);
		}
	}

	/*Set up GL calls*/
	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,BufferArrayObjects[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferArrayObjects[ELEMENT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void CCloth::Render(CCamera Camera, GLuint Shader)
{
	glUseProgram(Shader);

	//Lets make our models matrix first
	glm::mat4 m_positionMatrix = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 m_scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 m_modelMatrix = m_positionMatrix * m_scaleMatrix;	//No Rotations needed!

	glUniformMatrix4fv(glGetUniformLocation(Shader, "Model"), 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "View"), 1, GL_FALSE, &Camera.ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "Projection"), 1, GL_FALSE,&Camera.ProjectMatrix[0][0]);

	/*Gonna use an element buffer*/
	glBindVertexArray(VertexArrayBuffer);
	glPointSize(6.0f);
	glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}
