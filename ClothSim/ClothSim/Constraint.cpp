#include "Constraint.h"

CConstraint::CConstraint(CParticle * pA, CParticle * pB)
{
	m_particleA = pA;
	m_particleB = pB;
	m_restingLength = glm::length(pB->GetPosition() - pA->GetPosition());
}

CConstraint::~CConstraint()
{

}


void CConstraint::Update(float deltaTime)
{
	for (unsigned int i = 0; i < 15; i++)
	{
		/*Get distance from Paticle A to particle B*/
		glm::vec3 pA_to_Pb = m_particleB->GetPosition() - m_particleA->GetPosition();
		float distant = glm::length(pA_to_Pb);

		/*Find the correction force*/
		glm::vec3 CorrectionVector = pA_to_Pb * (1 - m_restingLength / distant);
		CorrectionVector *= .5f;

		/*Notice the inversion of the force on particle B*/
		m_particleA->AddForce(CorrectionVector);
		m_particleB->AddForce(-CorrectionVector);
	}
}
