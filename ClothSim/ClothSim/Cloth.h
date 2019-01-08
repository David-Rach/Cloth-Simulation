#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <SOIL.h>

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
	void AddWindForces(CParticle* p1, CParticle* p2, CParticle* p3, glm::vec3 direction);
	void Wind(glm::vec3 Direction);
	CParticle* GetParticle(int Index);
	void AddTexture(std::string path);

	glm::vec3 ComputeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	glm::vec3 ComputeNormalNormalized(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	int GetIndexFromGridCoord(int x, int y);

	int m_width;
	int m_height;

private:
	enum{POSITION, ELEMENT, NORMAL,TEXTURE_COORDS ,NUMOFBUFFERS};

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_UVs;
	std::vector<unsigned int> m_indices;
	GLuint m_texture;

	std::vector<CParticle*> m_ArrayOfParticles;
	std::vector<CConstraint*> m_ArrayOfConstraints;
	int m_NumOfParticles;
};
#endif