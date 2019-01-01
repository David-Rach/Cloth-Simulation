#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Particle.h"

class CConstraint
{
public:
	CConstraint();
	~CConstraint();

	void LinkParticles(CParticle* pA, CParticle* pB);
	void Update(float deltaTime);

private:
	float m_restingLength = 1.0f;

	/*Particles to constrain*/
	CParticle* m_particleA;
	CParticle* m_particleB;
};
#endif