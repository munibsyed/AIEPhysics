#include "Sphere.h"




Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, glm::vec4 colour, bool isStatic)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_radius = radius;
	m_colour = colour;

	m_rotation = 0.01f;
	m_angularVelocity = 0;
	m_angularDrag = 0.001f;
	m_linearDrag = 1;
	m_elasticity = elasticity;

	m_isStatic = isStatic;


	m_shapeID = SPHERE;
}

Sphere::~Sphere()
{
}

void Sphere::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, m_radius * 8, m_colour);
	aie::Gizmos::add2DLine(m_position, m_position + glm::vec2(cos(m_rotation), sin(m_rotation)) * m_radius, glm::vec4(0, 1, 0, 1));
}


float Sphere::GetRadius()
{
	return m_radius;
}

glm::vec4 Sphere::GetColour()
{
	return m_colour;
}
