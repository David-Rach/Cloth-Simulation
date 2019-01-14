#ifndef BALL_H
#define BALL_H

#include "Cloth.h"

class CBall
{
public:
	CBall();
	~CBall();

	void SetClothObject(CCloth* Cloth);
	void Update(float deltaTime);
	void Render(CCamera Camera, GLuint SHADER);

	void SetPosition(glm::vec3 Position);

	float m_radius = 1.f;
private:
	enum { POSITION, ELEMENT, NUMOFBUFFERS };

	GLuint VertexArrayBuffer;
	GLuint BufferArrayObjects[NUMOFBUFFERS];

	CCloth* m_ClothObject;

	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned int> m_indices;

	glm::vec3 m_position = glm::vec3(0,0,10);
	glm::vec3 m_InitialPosition = glm::vec3(0, 0, 0);

	float t = 2;
};
#endif