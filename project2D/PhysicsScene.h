#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Triangle.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <intrin.h>
#include <cmath>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void Update(float dt);
	void UpdateGizmos();
	void DebugScene();

	void CheckForCollision();

	void SetGravity(const glm::vec2 gravity);
	glm::vec2 GetGravity() const;

	void SetTimeStep(const float timeStep);
	float GetTimeStep() const;

	std::vector<PhysicsObject*> GetActors() const;

	int GetNumActors();

	static bool PlaneToPlane(PhysicsObject*, PhysicsObject*); 
	static bool PlaneToSphere(PhysicsObject* first, PhysicsObject* second); 
	static bool SphereToPlane(PhysicsObject* first, PhysicsObject* second); 
	static bool SphereToSphere(PhysicsObject* first, PhysicsObject* second);
	static bool PlaneToBox(PhysicsObject* first, PhysicsObject* second);
	static bool SphereToBox(PhysicsObject* first, PhysicsObject* second);
	static bool BoxToBox(PhysicsObject* first, PhysicsObject* second);
	static bool BoxToSphere(PhysicsObject* first, PhysicsObject* second);
	static bool BoxToPlane(PhysicsObject* first, PhysicsObject* second);
	static bool PlaneToTriangle(PhysicsObject* first, PhysicsObject* second);
	static bool SphereToTriangle(PhysicsObject* first, PhysicsObject* second);
	static bool BoxToTriangle(PhysicsObject* first, PhysicsObject* second);
	static bool TriangleToPlane(PhysicsObject* first, PhysicsObject* second);
	static bool TriangleToSphere(PhysicsObject* first, PhysicsObject* second);
	static bool TriangleToBox(PhysicsObject* first, PhysicsObject* second);
	static bool TriangleToTriangle(PhysicsObject* first, PhysicsObject* second);


protected:
	static bool m_hasBroken;
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	int m_numActors;
	int m_collisions = 0;
	int m_iterations = 0;
};

