#include "Ball.h"

CBall::CBall()
{
	/*Setup Buffers*/
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);

	float stacks = 30.f;
	float slices = 30.f;
	for (size_t i = 0; i < stacks; i++)
	{
		float V = i / stacks;
		float phi = V * glm::pi<float>();

		/*Loop Through Slices*/
		for (size_t j = 0; j < slices; j++)
		{
			float U = j / slices;
			float theta = U * (glm::pi<float>() * 2);

			/*Get Vertex Position*/
			float x = glm::cos(theta) * glm::sin(phi);
			float y = glm::cos(phi);
			float z = glm::sin(theta) * glm::sin(phi);

			m_vertices.push_back(glm::vec3(x, y, z));
		}
	}

	/*Do indices*/
	for (unsigned int i = 0; i < slices * stacks + slices; ++i) {

		m_indices.push_back(i);
		m_indices.push_back(i + slices + 1);
		m_indices.push_back(i + slices);

		m_indices.push_back(i + slices + 1);
		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	/*Do Buffers*/
	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferArrayObjects[ELEMENT]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

CBall::~CBall()
{
}

void CBall::SetClothObject(CCloth * Cloth)
{
	m_ClothObject = Cloth;
}

void CBall::Update(float deltaTime)
{
	for (size_t i = 0; i < m_ClothObject->m_ArrayOfParticles.size(); i++)
	{
		if (m_ClothObject->m_ArrayOfParticles[i]->m_moveable)
		{
			glm::vec3 Distance = m_ClothObject->m_ArrayOfParticles[i]->GetPosition() - m_position;
			float ScalarDistance = glm::length(Distance);
			if (ScalarDistance < m_radius)
			{
				Distance = glm::normalize(Distance);
				m_ClothObject->m_ArrayOfParticles[i]->SetPosition(m_position + (Distance * (m_radius)));
			}
		}
	}

	t += deltaTime;

	/*I move the ball along the Z axis by using a value from this sine wave equation*/
	m_position.z = m_InitialPosition.z + (6 * glm::sin(0.1f * t));
}

void CBall::Render(CCamera Camera, GLuint SHADER)
{
	glUseProgram(SHADER);

	//Lets make our models matrix first
	glm::mat4 m_positionMatrix = glm::translate(m_position);
	glm::mat4 m_scaleMatrix = glm::scale(glm::vec3(0.95f, 0.95f, 0.9f));
	glm::mat4 m_modelMatrix = m_positionMatrix * m_scaleMatrix;	//No Rotations needed!

	glUniformMatrix4fv(glGetUniformLocation(SHADER, "Model"), 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(SHADER, "View"), 1, GL_FALSE, &Camera.ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(SHADER, "Projection"), 1, GL_FALSE, &Camera.ProjectMatrix[0][0]);

	/*Pass in position for normal calculations*/
	glUniform3fv(glGetUniformLocation(SHADER, "bPosition"),1,&m_position[0]);

	/*Gonna use an element buffer*/
	glBindVertexArray(VertexArrayBuffer);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void CBall::SetPosition(glm::vec3 Position)
{
	m_position = Position;
}
