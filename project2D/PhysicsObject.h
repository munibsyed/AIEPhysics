#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <Gizmos.h>
#include <iostream>

enum ShapeType
{
	JOINT = -1,
	PLANE,
	SPHERE,
	BOX,
	TRIANGLE,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	~PhysicsObject();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;

	ShapeType GetShapeID() const;

	int GetObjectID();

protected:
	PhysicsObject();
	PhysicsObject(ShapeType shapeType);
	ShapeType m_shapeID;
	

	bool operator == (const PhysicsObject& other);

	int m_objectID;
	static int m_objectIDCounter;
};

