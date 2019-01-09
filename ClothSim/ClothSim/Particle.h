#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm.hpp>
#include <vector>


class CParticle
{
public:
	CParticle(glm::vec3 m_position);
	~CParticle();

	void AddForce(glm::vec3 force);
	void Update(float deltaTime);
	bool m_moveable = true;
	void SetPosition(glm::vec3 _Val);

	/*Accessor functions*/
	glm::vec3 GetPosition();
	std::vector<int> m_connectedFaces;

private:
	glm::vec3 m_position;
	glm::vec3 m_oldPosition;
	glm::vec3 m_acceleration = glm::vec3(0,0,0);

	float m_mass = 1.f;
	
	
};
#endif