#include "Constraint.h"

CConstraint::CConstraint()
{

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
	/*Get distance from Paticle A to particle B*/
	glm::vec3 pA_to_Pb = m_particleA->GetPosition() - m_particleB->GetPosition();
	float distant = pA_to_Pb.length();

	/*Find the correction force*/
	glm::vec3 CorrectionVector = pA_to_Pb * (1 - (m_restingLength / distant));
	CorrectionVector *= .5f;

	/*Apply Correction Force*/
	m_particleA->AddForce(CorrectionVector);
	m_particleB->AddForce(-CorrectionVector);
}
