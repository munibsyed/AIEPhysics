#pragma once
#include <Gizmos.h>
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, glm::vec4 colour, bool isStatic);
	~Sphere();

	virtual void MakeGizmo();

	float GetRadius();
	glm::vec4 GetColour();

protected:
	float m_radius;
	glm::vec4 m_colour;
};

