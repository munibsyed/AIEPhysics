#include "Plane.h"



Plane::Plane()
{
}

Plane::Plane(glm::vec2 normal, float distanceToOrigin)
{
	m_normal = normal;
	m_distanceToOrigin = distanceToOrigin;
	m_shapeID = PLANE;
}


Plane::~Plane()
{
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Debug()
{
}

void Plane::MakeGizmo()
{
	float lineSegmentLength = 300;
	glm::vec2 centrePoint = m_normal - m_distanceToOrigin;
	//aie::Gizmos::add2DCircle(centrePoint, 4, 8, glm::vec4(0, 1, 0, 1));
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colour(1, 1, 1, 1);
	glm::vec2 start = centrePoint + (parallel * lineSegmentLength) - m_normal;
	glm::vec2 end = centrePoint - (parallel * lineSegmentLength) - m_normal;
	aie::Gizmos::add2DLine(start, end, colour);
}

glm::vec2 Plane::GetNormal()
{
	return m_normal;
}

float Plane::GetDistanceToOrigin()
{
	return m_distanceToOrigin;
}

void Plane::SetNormal(glm::vec2 normal)
{
	m_normal = normal;
}
