#include "Constraint.h"

CConstraint::CConstraint()
{

}

CConstraint::CConstraint(CParticle * pA, CParticle * pB, int Type)
{
	m_particleA = pA;
	m_particleB = pB;
	m_restingLength = glm::length(pB->GetPosition() - pA->GetPosition());
	this->Type = Type;

	/*Setup Buffers*/
	glGenVertexArrays(1, &VertexArrayBuffer);
	glGenBuffers(NUMOFBUFFERS, BufferArrayObjects);
	
}

CConstraint::~CConstraint()
{

}

void CConstraint::LinkParticles(CParticle * pA, CParticle * pB)
{
	m_particleA = pA;
	m_particleB = pB;
}

void CConstraint::Update(float deltaTime)
{
	for (unsigned int i = 0; i < 15; i++)
	{
		/*Get distance from Paticle A to particle B*/
		glm::vec3 pA_to_Pb = m_particleB->GetPosition() - m_particleA->GetPosition();
		float distant = glm::length(pA_to_Pb);
		//float distant = pA_to_Pb.length();

		/*Find the correction force*/
		glm::vec3 CorrectionVector = pA_to_Pb * (1 - m_restingLength / distant);
		CorrectionVector *= .5f;

		if (Type == STRETCH)
		{
			m_particleA->AddForce(CorrectionVector);
			m_particleB->AddForce(-CorrectionVector);
		}
		else if (Type == SHEAR)
		{
			m_particleA->AddForce(CorrectionVector);
			m_particleB->AddForce(-CorrectionVector);
		}
		else if (Type == BEND)
		{
			m_particleA->AddForce(CorrectionVector);
			m_particleB->AddForce(-CorrectionVector);
		}
	}

}

void CConstraint::DebugRender(CCamera Camera, GLuint Shader)
{
	/*Set up GL calls*/
	m_vertices.resize(2);
	m_vertices[0] = m_particleA->GetPosition();
	m_vertices[1] = m_particleB->GetPosition();

	glBindVertexArray(VertexArrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, BufferArrayObjects[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glUseProgram(Shader);

	//Lets make our models matrix first
	glm::mat4 m_positionMatrix = glm::translate(glm::vec3(0, 0, 0));
	glm::mat4 m_scaleMatrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 m_modelMatrix = m_positionMatrix * m_scaleMatrix;	//No Rotations needed!

	glUniformMatrix4fv(glGetUniformLocation(Shader, "Model"), 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "View"), 1, GL_FALSE, &Camera.ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(Shader, "Projection"), 1, GL_FALSE, &Camera.ProjectMatrix[0][0]);

	glBindVertexArray(VertexArrayBuffer);
	glDrawArrays(GL_LINES, 0, m_vertices.size());
	glBindVertexArray(0);
}
