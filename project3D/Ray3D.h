#pragma once
#include "PhysicsObject3D.h"

class Ray3D : public PhysicsObject3D
{
public:
	Ray3D(glm::vec3 startPos, glm::vec3 direction, float length, glm::vec4 colour);
	~Ray3D();

	void FixedUpdate(glm::vec3 gravity, float timeStep);

	void MakeGizmo();

	float GetLength();

	glm::vec3 GetStartPosition();

	glm::vec3 GetDirection();

	void SetLastHit(PhysicsObject3D* lastHit);

	PhysicsObject3D* GetLastHit();

private:
	glm::vec3 m_startPos;
	glm::vec3 m_direction;
	glm::vec4 m_colour;

	PhysicsObject3D* m_lastHit;

	float m_length;
};

