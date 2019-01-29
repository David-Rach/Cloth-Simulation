#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <glew.h>
#include <glfw3.h>

#include "Particle.h"
#include "Camera.h"

enum CONSTRAINT_TYPE
{
	STRETCH,
	SHEAR,
	BEND
};

class CConstraint
{
public:
	CConstraint();
	CConstraint(CParticle* pA, CParticle* pB, int Type);
	~CConstraint();

	void Update(float deltaTime);

	void DebugRender(CCamera Camera, GLuint Shader);
	float m_restingLength = 1.f;
	int Type;

private:
	enum { POSITION, NUMOFBUFFERS };
	 

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> m_vertices;

	/*Particles to constrain*/
	CParticle* m_particleA;
	CParticle* m_particleB;
};
#endif