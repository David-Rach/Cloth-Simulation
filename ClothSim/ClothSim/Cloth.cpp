#include "Cloth.h"

CCloth::CCloth(int m_width, int m_height)
{
	this->m_height = m_height;
	this->m_width = m_width;

	m_NumOfParticles = m_height * m_width;
	m_vertices.resize(m_NumOfParticles);
	m_normals.resize(m_NumOfParticles);

	/*Create Particles*/
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			CParticle* Particle;
			Particle = new CParticle(glm::vec3((x ) - (m_width / 2), (y) - (m_height / 2), 0));
			m_ArrayOfParticles.push_back(Particle);
		}
	}

	/*Shear Constraints*/
	for (int x = 0; x < m_width; x++)
	{
		for (int y = 0; y < m_height; y++)
		{
			CParticle* ParticleA = m_ArrayOfParticles[GetIndexFromGridCoord(x, y)];

			/*Stretch Constraints*/
			if (x + 1 < m_width)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x + 1, y)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB,STRETCH));
			}

			if (y + 1 < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x, y + 1)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB, STRETCH));
			}

			/*Shear Constraints*/
			if (x + 1 < m_width && y + 1 < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x + 1, y + 1)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB, SHEAR));
			}

			if (x - 1 >= 0 && y + 1 < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x - 1, y + 1)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB, SHEAR));
			}

			/*Bend constraints*/
			if (x + 2 < m_width && y + 2 < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x + 2, y + 2)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB, BEND));
			}

			if (x + 2 < m_width && y < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x + 2, y)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB,BEND));
			}

			if (x < m_width && y + 2 < m_height)
			{
				CParticle* ParticleB = m_ArrayOfParticles[GetIndexFromGridCoord(x, y + 2)];
				m_ArrayOfConstraints.push_back(new CConstraint(ParticleA, ParticleB, BEND));
			}
		}
	}

	for (unsigned int i = 0; i < 1; i++)
	{
		m_ArrayOfParticles[GetIndexFromGridCoord(i, m_height - 1)]->m_moveable = false;
		m_ArrayOfParticles[GetIndexFromGridCoord((m_width - 1) - i, m_height - 1)]->m_moveable = false;
	}

	int CentrePoint = (m_width - 1) / 2;
	CentrePoint = floor(CentrePoint);
	m_ArrayOfParticles[GetIndexFromGridCoord(CentrePoint, m_height - 1)]->m_moveable = false;



	/*Setup Buffers*/
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);

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
}

CCloth::~CCloth()
{
	for (size_t i = 0; i < m_NumOfParticles; i++)
	{
		delete m_ArrayOfParticles[i];
	}

	for (size_t i = 0; i < m_ArrayOfConstraints.size(); i++)
	{
		delete m_ArrayOfConstraints[i];
	}
}

void CCloth::Update(float deltaTime)
{
	for (size_t i = 0; i < m_ArrayOfParticles.size(); i++)
	{
		m_ArrayOfParticles[i]->AddForce(glm::vec3(0, -0.012f, 0));
		m_ArrayOfParticles[i]->Update(deltaTime);
	}

	for (size_t i = 0; i < m_ArrayOfConstraints.size(); i++)
	{
		m_ArrayOfConstraints[i]->Update(deltaTime);
		
	}
	

	/*stream the vertices*/
	for (unsigned int i = 0; i < m_ArrayOfParticles.size(); i++)
	{
		m_vertices[i] = m_ArrayOfParticles[i]->GetPosition();
	}


	/*Stream the normals*/
	int count = 0;
	for (int x = 0; x < m_width - 1; x++)
	{
		for (int y = 0; y < m_height - 1; y++)
		{
			glm::vec3 a = m_ArrayOfParticles[GetIndexFromGridCoord(x, y)]->GetPosition();
			glm::vec3 b = m_ArrayOfParticles[GetIndexFromGridCoord(x + 1, y)]->GetPosition();
			glm::vec3 c = m_ArrayOfParticles[GetIndexFromGridCoord(x, y + 1)]->GetPosition();

			m_normals[count] = ComputeNormal(a,b,c);
			count++;
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

	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
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
	glPointSize(2.0f);
	//glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
	glDrawElements(GL_POINTS, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
	glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
}

CParticle * CCloth::GetParticle(int Index)
{
	return m_ArrayOfParticles[Index];
}

glm::vec3 CCloth::ComputeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return glm::normalize(glm::cross(b - a, c - a));
}

int CCloth::GetIndexFromGridCoord(int x, int y)
{
	return (x * m_height + y);
}
