#include "Particle.h"

CParticle::CParticle(glm::vec3 m_position)
{
	this->m_position = m_position;
	m_oldPosition = m_position;
}

CParticle::~CParticle()
{

}

void CParticle::AddForce(glm::vec3 force)
{
	m_acceleration = force / m_mass;
}

void CParticle::Update(float deltaTime)
{
	if (m_moveable)
	{
		/*Verlet Integration*/
		m_position = m_position + (m_position - m_oldPosition) + m_acceleration;	//Probably gonna need damping in here
		m_acceleration = glm::vec3(0, 0, 0);
	}
}

glm::vec3 CParticle::GetPosition()
{
	return m_position;
}
