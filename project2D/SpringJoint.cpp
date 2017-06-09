#include "SpringJoint.h"



SpringJoint::SpringJoint(Rigidbody* connectionFirst, Rigidbody* connectionSecond, float springConstant, float friction)
{
	m_springJoints = new Rigidbody*[2];
	m_springJoints[0] = connectionFirst;
	m_springJoints[1] = connectionSecond;

	m_springConstant = springConstant;
	m_friction = friction;

	m_restDistance = glm::length(connectionFirst->GetPosition() - connectionSecond->GetPosition());


}


SpringJoint::~SpringJoint()
{
	//delete m_springJoints[0];
	//delete m_springJoints[1];
	delete m_springJoints;
}

void SpringJoint::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	glm::vec2 displacement = m_springJoints[0]->GetPosition() - m_springJoints[1]->GetPosition();
	glm::vec2 relativeVelocity = m_springJoints[0]->GetVelocity() - m_springJoints[1]->GetVelocity();
	glm::vec2 force = -m_springConstant * m_restDistance * displacement - (relativeVelocity * m_friction);

	m_springJoints[0]->ApplyForce(force);
	m_springJoints[1]->ApplyForce(-force);
}

void SpringJoint::Debug()
{
}

void SpringJoint::MakeGizmo()
{
	aie::Gizmos::add2DLine(m_springJoints[0]->GetPosition(), m_springJoints[1]->GetPosition(), glm::vec4(0, 1, 0, 1));
}
