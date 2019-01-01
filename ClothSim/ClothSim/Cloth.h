#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>

#include "Particle.h"
#include "Constraint.h"
#include "Camera.h"

class CCloth
{
public:
	CCloth(int m_width, int m_height);
	~CCloth();

	void Update(float deltaTime);
	void Render(CCamera Camera, GLuint Shader);

	int GetIndexFromGridCoord(int x, int y);

private:
	enum{POSITION, ELEMENT, NORMAL, NUMOFBUFFERS};

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<unsigned int> m_indices;

	std::vector<CParticle*> m_ArrayOfParticles;
	std::vector<CConstraint*> m_ArrayOfConstraints;

	int m_width;
	int m_height;
	int m_NumOfParticles;
};
#endif