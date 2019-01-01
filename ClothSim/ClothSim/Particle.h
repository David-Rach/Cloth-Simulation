#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm.hpp>

class CParticle
{
public:
	CParticle(glm::vec3 m_position);
	~CParticle();

	void AddForce(glm::vec3 force);
	void Update(float deltaTime);

	/*Accessor functions*/
	glm::vec3 GetPosition();

private:
	glm::vec3 m_position;
	glm::vec3 m_oldPosition;
	glm::vec3 m_acceleration;

	float m_mass = 1.0f;
	bool m_moveable = true;
	
};
#endif