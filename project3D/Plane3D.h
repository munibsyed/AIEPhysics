#pragma once
#include "PhysicsObject3D.h"
class Plane3D :
	public PhysicsObject3D
{
public:
	Plane3D(glm::vec3 normal, glm::vec3 distanceFromOrigin, glm::vec4 colour);
	~Plane3D();

	void MakeGizmo();
	
	void FixedUpdate(glm::vec3 gravity, float timeStep);

	glm::vec3 GetNormal();

	glm::vec3 GetDistanceFromOrigin();

private:
	glm::vec3 m_distanceFromOrigin;
	glm::vec3 m_normal;
};

