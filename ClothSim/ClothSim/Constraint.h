#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <glew.h>
#include <glfw3.h>

#include "Particle.h"
#include "Camera.h"

class CConstraint
{
public:
	CConstraint(CParticle* pA, CParticle* pB);
	~CConstraint();

	void Update(float deltaTime);

	float m_restingLength = 1.f;

private:
	CParticle* m_particleA;
	CParticle* m_particleB;
};
#endif