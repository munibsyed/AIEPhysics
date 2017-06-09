	#include "PhysicsObject3D.h"



PhysicsObject3D::PhysicsObject3D()
{
	m_objectID = m_numObjectsCreated;
	m_numObjectsCreated++;
}


PhysicsObject3D::~PhysicsObject3D()
{
}

ShapeType3D PhysicsObject3D::GetShapeID()
{
	return m_shapeID;
}

void PhysicsObject3D::SetColour(glm::vec4 colour)
{
	m_colour = colour;
}

int PhysicsObject3D::GetObjectID()
{
	return m_objectID;
}

int PhysicsObject3D::m_numObjectsCreated = 0;
