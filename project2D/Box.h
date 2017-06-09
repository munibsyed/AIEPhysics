#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box(glm::vec2 position, glm::vec2 extents, float mass, glm::vec4 colour);
	~Box();
	
	void Debug();

	void MakeGizmo();

	glm::vec2 GetExtents();

	glm::vec2* GetCorners();

	glm::vec2* GetEdges();

	void CalculateCorners();

	void CalculateEdges();

private:
	glm::vec2 m_extents;
	glm::vec4 m_colour;

	glm::vec2* m_corners;
	glm::vec2* m_edges;
};

