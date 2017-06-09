#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
}

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_isStatic = false;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isStatic == true)
		return;

	ApplyForce(gravity * m_mass * timeStep);
	//apply drag
	m_velocity *= m_linearDrag;
	m_position += m_velocity * timeStep;

	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
	m_rotation += m_angularVelocity * timeStep;
	

	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
		m_velocity = glm::vec2(0, 0);
	if (abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
		m_angularVelocity = 0;
}

void Rigidbody::Debug()
{
}

void Rigidbody::ApplyForce(glm::vec2 force)
{
	if (m_isStatic == true)
		return;

	glm::vec2 acceleration = force / m_mass;
	m_velocity += acceleration;

}

void Rigidbody::ApplyTorque(float torque)
{
	m_angularVelocity += torque;
}

void Rigidbody::ApplyForceToActor(Rigidbody * otherActor, glm::vec2 force)
{
	ApplyForce(-force);
	otherActor->ApplyForce(force);
}

bool Rigidbody::IsStatic()
{
	return m_isStatic;
}

glm::vec2 Rigidbody::GetPosition()
{
	return m_position;
}

glm::vec2 Rigidbody::GetVelocity()
{
	return m_velocity;
}

void Rigidbody::SetPosition(glm::vec2 position)
{
	m_position = position;
}

void Rigidbody::SetVelocityToZero()
{
	m_velocity = glm::vec2(0, 0);
}

float Rigidbody::GetMass()
{
	return m_mass;
}

float Rigidbody::GetRotation()
{
	return m_rotation;
}

float Rigidbody::GetElasticity()
{
	return m_elasticity;
}

void Rigidbody::AddRotation(float amount)
{
	m_rotation = amount;
}

void Rigidbody::SetRotation(float rot)
{
	m_rotation = rot;
}

void Rigidbody::ReduceMass(float amount)
{
	m_mass -= amount;
	if (m_mass < 0)
	{
		m_mass = 0;
	}
}

glm::vec2 Rigidbody::CalculateProjectilePosition(float theta, float speed, float gravity, float time)
{
	float x = m_position.x + speed*time*cos(theta);
	float y = m_position.y + speed*time*sin(theta) - (0.5f * gravity * (time*time));
	return glm::vec2(x, y);
}
