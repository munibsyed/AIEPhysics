#include "Rigidbody3D.h"



Rigidbody3D::Rigidbody3D()
{
}


Rigidbody3D::~Rigidbody3D()
{
}

void Rigidbody3D::ApplyForce(glm::vec3 force)
{
	if (m_isStatic)
		return;

	glm::vec3 acceleration = force / m_mass;
	m_velocity += acceleration;
}

void Rigidbody3D::ApplyForceToActor(Rigidbody3D * rb, glm::vec3 force)
{
	ApplyForce(-force);
	rb->ApplyForce(force);
}

void Rigidbody3D::ApplyTorque(glm::vec3 torque)
{
	if (m_isStatic)
		return;
	m_angularVelocity += torque;
}

void Rigidbody3D::SetPosition(glm::vec3 position)
{
	m_position = position;
}

glm::vec3 Rigidbody3D::GetPosition()
{
	return m_position;
}

glm::vec3 Rigidbody3D::GetVelocity()
{
	return m_velocity;
}

float Rigidbody3D::GetMass()
{
	return m_mass;
}

void Rigidbody3D::SetVelocityToZero()
{
	m_velocity = glm::vec3(0);
}

void Rigidbody3D::FixedUpdate(glm::vec3 gravity, float timeStep)
{
	ApplyForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	m_velocity *= m_linearDrag;

	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec3(0);
	}

	/*if (m_angularVelocity.x < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity.x = 0;
	}

	if (m_angularVelocity.y < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity.y = 0;
	}
	
	if (m_angularVelocity.z < MIN_ROTATION_THRESHOLD)
	{
		m_angularVelocity.z = 0;
	}*/

}
