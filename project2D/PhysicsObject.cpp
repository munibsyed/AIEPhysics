#include "PhysicsObject.h"



ShapeType PhysicsObject::GetShapeID() const
{
	return m_shapeID;
}

int PhysicsObject::GetObjectID()
{
	return m_objectID;
}

bool PhysicsObject::operator==(const PhysicsObject & other)
{
	return m_objectID == other.m_objectID;
}

PhysicsObject::PhysicsObject()
{
	m_objectIDCounter++;
	m_objectID = m_objectIDCounter;
}

PhysicsObject::PhysicsObject(ShapeType shapeType)
{
	m_shapeID = shapeType;
}


PhysicsObject::~PhysicsObject()
{
}

int PhysicsObject::m_objectIDCounter = 0;
	