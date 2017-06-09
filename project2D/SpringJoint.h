#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"
class SpringJoint :
	public PhysicsObject
{
public:
	SpringJoint(Rigidbody* connectionFirst, Rigidbody* connectionSecond, float springConstant, float friction);
	~SpringJoint();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();
	virtual void MakeGizmo();

private:

	Rigidbody** m_springJoints;

	float m_restDistance;
	float m_springConstant;
	float m_friction;
};

