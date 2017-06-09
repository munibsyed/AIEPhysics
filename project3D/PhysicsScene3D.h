#pragma once
#include "Sphere3D.h"
#include "Plane3D.h"
#include "Ray3D.h"
#include "Box3D.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

class PhysicsScene3D
{
public:
	PhysicsScene3D();
	PhysicsScene3D(glm::vec3 gravity, float timeStep);
	~PhysicsScene3D();

	void AddActor(PhysicsObject3D* actor);

	void RemoveActor(PhysicsObject3D* actor);

	void Update(float dt);

	void Draw();

	void CheckForCollisions();

	void SetGravity(glm::vec3 gravity);
	
	std::vector<PhysicsObject3D*> RaycastAll(Ray3D &ray);

	std::vector<PhysicsObject3D*> RaycastAll(glm::vec3 startPos, glm::vec3 direction, float distance, glm::vec4 colour);

	Rigidbody3D* RaycastToClosestRigidbody(Ray3D ray);

	PhysicsObject3D* RaycastFirst(glm::vec3 startPos, glm::vec3 direction, float distance, glm::vec4 colour);

	static bool PlaneToPlane(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool PlaneToBox(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool SphereToPlane(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool SphereToBox(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool PlaneToSphere(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool BoxToSphere(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool BoxToPlane(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool BoxToBox(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool BoxToRay(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool SphereToSphere(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool PlaneToRay(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool RayToPlane(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool SphereToRay(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool RayToSphere(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool RayToBox(PhysicsObject3D* first, PhysicsObject3D* second);
	static bool RayToRay(PhysicsObject3D* first, PhysicsObject3D* second);

	//is passing in a base class necessary for ray-to-x collisions?

private:
	std::vector<PhysicsObject3D*> m_actors;
	glm::vec3 m_gravity;

	float m_timeStep;
};

