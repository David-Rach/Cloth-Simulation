#ifndef CLOTH_H
#define CLOTH_H

#include <vector>
#include <glm\gtx\transform.hpp>
#include <glm\matrix.hpp>
#include <SOIL.h>

#include "Particle.h"
#include "Constraint.h"
#include "Camera.h"

struct ClothFace
{
public:
	ClothFace(){};
	CParticle* p1,* p2,* p3,* p4;
	glm::vec2 uv1, uv2, uv3, uv4;

	glm::vec3 normal;
	glm::vec3 tangent;
};

class CCloth
{
public:
	CCloth(int m_width, int m_height);
	~CCloth();

	void Update(float deltaTime);
	void Render(CCamera Camera, GLuint Shader);
	void AddWindForces(CParticle* p1, CParticle* p2, CParticle* p3, glm::vec3 direction);
	void AddWindForces(ClothFace Face, glm::vec3 direction);
	void Wind(glm::vec3 Direction);
	CParticle* GetParticle(int Index);
	void AddTexture(std::string path);
	void AddNormalMap(std::string path);

	glm::vec3 ComputeNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	glm::vec3 ComputeNormalNormalized(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	glm::vec3 ComputeNormalNormalized(CParticle* a, CParticle* b, CParticle* c);
	int GetIndexFromGridCoord(int x, int y);

	int m_width;
	int m_height;

	bool bUseOldShading = false;
	bool bNormalMapping = true;
	int iDrawType = 0;

	float m_WindChangeTimer = 0.0f;
	glm::vec3 m_WindDirection;
	std::vector<CParticle*> m_ArrayOfParticles;
private:
	enum{POSITION, ELEMENT, NORMAL, TEXTURE_COORDS, TANGENTS ,NUMOFBUFFERS};

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec3> m_tangents;
	std::vector<glm::vec2> m_UVs;
	std::vector<ClothFace> m_Faces;
	std::vector<unsigned int> m_indices;
	GLuint m_texture;
	GLuint m_normalMap;

	
	std::vector<CConstraint*> m_ArrayOfConstraints;
	int m_NumOfParticles;
};
#endif