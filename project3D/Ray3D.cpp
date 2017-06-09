#include "Ray3D.h"



Ray3D::Ray3D(glm::vec3 position, glm::vec3 direction, float length, glm::vec4 colour)
{
	m_startPos = position;
	m_direction = direction;
	m_colour = colour;
	m_length = length;
	m_shapeID = RAY;

	m_lastHit = nullptr;
}


Ray3D::~Ray3D()
{
}

void Ray3D::FixedUpdate(glm::vec3 gravity, float timeStep)
{
}

void Ray3D::MakeGizmo()
{
	aie::Gizmos::addLine(m_startPos, m_startPos + (m_direction*m_length), m_colour);
}

float Ray3D::GetLength()
{
	return m_length;
}

glm::vec3 Ray3D::GetStartPosition()
{
	return m_startPos;
}

glm::vec3 Ray3D::GetDirection()
{
	return m_direction;
}

void Ray3D::SetLastHit(PhysicsObject3D * lastHit)
{
	m_lastHit = lastHit;
}

PhysicsObject3D * Ray3D::GetLastHit()
{
	return m_lastHit;
}
