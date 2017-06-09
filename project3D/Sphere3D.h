#pragma once
#include "Rigidbody3D.h"
class Sphere3D :
	public Rigidbody3D
{
public:
	Sphere3D(glm::vec3 position, glm::vec3 velocity, float mass, float radius, float linearDrag, float angularDrag, bool isStatic, glm::vec4 colour);
	~Sphere3D();

	void MakeGizmo();

	float GetRadius();

private:
	float m_radius;
};

