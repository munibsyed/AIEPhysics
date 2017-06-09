#pragma once
#include "PhysicsObject.h"

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distanceToOrigin);
	~Plane();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();
	virtual void MakeGizmo();

	glm::vec2 GetNormal();

	float GetDistanceToOrigin();

	void SetNormal(glm::vec2 normal);

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;

};

