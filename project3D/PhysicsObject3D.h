#pragma once
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <Gizmos.h>
#include <iostream>

#define PI 3.141592f

enum ShapeType3D
{
	RAY = -1,
	PLANE = 0,
	SPHERE = 1,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject3D
{
public:
	PhysicsObject3D();
	~PhysicsObject3D();

	virtual void FixedUpdate(glm::vec3 gravity, float timeStep) = 0;
	virtual void MakeGizmo() = 0;
	
	ShapeType3D GetShapeID();

	void SetColour(glm::vec4 colour);

	int GetObjectID();

protected:
	ShapeType3D m_shapeID;
	glm::vec4 m_colour;
	int m_objectID;
	static int m_numObjectsCreated;
};

