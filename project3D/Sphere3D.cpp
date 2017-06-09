#include "Sphere3D.h"



Sphere3D::Sphere3D(glm::vec3 position, glm::vec3 velocity, float mass, float radius, float linearDrag, float angularDrag, bool isStatic, glm::vec4 colour)
{
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_radius = radius;
	m_isStatic = isStatic;
	m_colour = colour;
	m_shapeID = SPHERE;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	//m_rotation = glm::mat4(cos(PI/4.0f), -sin(PI/4.0f), 0, 0,
	//						sin(PI/4.0f), cos(PI/4.0f), 0, 0,
	//						0, 0, 1, 0,
	//						0,0,0,1);

	m_rotationMatrix = glm::mat4();
}

Sphere3D::~Sphere3D()
{
}

void Sphere3D::MakeGizmo()
{
	m_rotationMatrix = glm::mat4();
	m_rotationMatrix = glm::rotate(m_rotation.x, glm::vec3(m_rotationMatrix[0]));
	m_rotationMatrix *= glm::rotate(m_rotation.y, glm::vec3(m_rotationMatrix[1]));
	m_rotationMatrix *= glm::rotate(m_rotation.z, glm::vec3(m_rotationMatrix[2]));

	const glm::mat4 rotation = m_rotationMatrix;
	aie::Gizmos::addSphere(m_position, m_radius, m_radius*16, m_radius*16, m_colour, &rotation);
}

float Sphere3D::GetRadius()
{
	return m_radius;
}
