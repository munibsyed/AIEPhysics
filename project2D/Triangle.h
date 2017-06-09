#pragma once
#include "Rigidbody.h"
class Triangle :
	public Rigidbody
{
public:
	Triangle(glm::vec2 position, glm::vec2 velocity, glm::vec2 firstCorner, glm::vec2 secondCorner, glm::vec2 thirdCorner, float mass, glm::vec4 colour);
	~Triangle();

	void MakeGizmo();
	
	void CalculateCorners();

	glm::vec2* GetCorners();

	glm::vec2* GetAxes();


private:
	glm::vec2 m_corners[3];
	glm::vec4 m_colour;
};

