#include "PhysicsScene3D.h"

typedef bool(*fn) (PhysicsObject3D* first, PhysicsObject3D* second);

static fn CollisionFunctionArray[]
{
	PhysicsScene3D::PlaneToPlane, PhysicsScene3D::PlaneToSphere, PhysicsScene3D::PlaneToBox,
	PhysicsScene3D::SphereToPlane, PhysicsScene3D::SphereToSphere, PhysicsScene3D::SphereToPlane,
	PhysicsScene3D::BoxToPlane, PhysicsScene3D::SphereToBox, PhysicsScene3D::BoxToBox
};

PhysicsScene3D::PhysicsScene3D()
{
}

PhysicsScene3D::PhysicsScene3D(glm::vec3 gravity, float timeStep)
{
	m_gravity = gravity;
	m_timeStep = timeStep;
}


PhysicsScene3D::~PhysicsScene3D()
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		delete m_actors[i];
	}
}

void PhysicsScene3D::AddActor(PhysicsObject3D * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene3D::RemoveActor(PhysicsObject3D * actor)
{
	auto item = std::find(m_actors.begin(), m_actors.end(), actor);
	if (item != m_actors.end())
	{
		m_actors.erase(item);
	}
}

void PhysicsScene3D::Update(float dt)
{
	static float timer = 0;
	timer += dt;
	if (timer >= m_timeStep)
	{
		for (int i = 0; i < m_actors.size(); i++)
		{
			m_actors[i]->FixedUpdate(m_gravity, timer);
		}
		timer = 0;
	}

}

void PhysicsScene3D::Draw()
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		m_actors[i]->MakeGizmo();
	}
}

void PhysicsScene3D::CheckForCollisions()
{
	for (int i = 0; i < m_actors.size() - 1; i++)
	{
		for (int ii = i + 1; ii < m_actors.size(); ii++)
		{
			PhysicsObject3D* first = m_actors[i];
			PhysicsObject3D* second = m_actors[ii];

			int shapeIDFirst = first->GetShapeID();
			int shapeIDSecond = second->GetShapeID();

			int index = shapeIDFirst * SHAPE_COUNT + shapeIDSecond;

			fn collisionFunctonPtr = CollisionFunctionArray[index];
			if (collisionFunctonPtr != nullptr)
				collisionFunctonPtr(first, second);
		}
	}
}

void PhysicsScene3D::SetGravity(glm::vec3 gravity)
{
	m_gravity = gravity;
}

std::vector<PhysicsObject3D*> PhysicsScene3D::RaycastAll(Ray3D &ray)
{
	std::vector<PhysicsObject3D*> intersectedActors;
	Ray3D *pRay = &ray;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->GetShapeID() == SPHERE)
		{
			if (RayToSphere(pRay, m_actors[i]))
			{
				intersectedActors.push_back(m_actors[i]);
			}
		}

		else if (m_actors[i]->GetShapeID() == BOX)
		{
			if (BoxToRay(m_actors[i], pRay))
			{
				intersectedActors.push_back(m_actors[i]);
			}
		}
	}

	return intersectedActors;
}

std::vector<PhysicsObject3D*> PhysicsScene3D::RaycastAll(glm::vec3 startPos, glm::vec3 direction, float distance, glm::vec4 colour)
{
	std::vector<PhysicsObject3D*> intersectedActors;
	Ray3D ray = Ray3D(startPos, direction, distance, colour);
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->GetShapeID() == SPHERE)
		{
			if (RayToSphere(&ray, m_actors[i]))
			{
				intersectedActors.push_back(m_actors[i]);
			}
		}
	}

	return intersectedActors;
}

Rigidbody3D * PhysicsScene3D::RaycastToClosestRigidbody(Ray3D ray)
{
	Rigidbody3D* closestRb = nullptr;
	float closestDistance = INT_MAX;
	Ray3D *pRay = &ray;
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->GetShapeID() == SPHERE)
		{
			if (RayToSphere(pRay, m_actors[i]))
			{
				if (closestRb == nullptr)
				{
					closestRb = dynamic_cast<Rigidbody3D*>(m_actors[i]);
					closestDistance = glm::distance(ray.GetStartPosition(), closestRb->GetPosition());
				}

				else
				{
					Rigidbody3D* rb = dynamic_cast<Rigidbody3D*>(m_actors[i]);
					float distance = glm::distance(ray.GetStartPosition(), rb->GetPosition());
					if (distance < closestDistance)
					{
						closestRb = rb;
						closestDistance = distance;
					}
				}
			}
		}
		else if (m_actors[i]->GetShapeID() == BOX)
		{
			if (BoxToRay(m_actors[i], pRay))
			{
				if (closestRb == nullptr)
				{
					closestRb = dynamic_cast<Rigidbody3D*>(m_actors[i]);
					closestDistance = glm::distance(ray.GetStartPosition(), closestRb->GetPosition());
				}


				else
				{
					Rigidbody3D* rb = dynamic_cast<Rigidbody3D*>(m_actors[i]);
					float distance = glm::distance(ray.GetStartPosition(), rb->GetPosition());
					if (distance < closestDistance)
					{
						closestRb = rb;
						closestDistance = distance;
					}
				}
			}
		}
	}

	return closestRb;
}

PhysicsObject3D * PhysicsScene3D::RaycastFirst(glm::vec3 startPos, glm::vec3 direction, float distance, glm::vec4 colour)
{
	return nullptr;
}

bool PhysicsScene3D::PlaneToPlane(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::PlaneToBox(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Plane3D* plane = dynamic_cast<Plane3D*>(first);
	Box3D* box = dynamic_cast<Box3D*>(second);

	if (plane != nullptr && box != nullptr)
	{
		glm::vec3 collisionNormal = plane->GetNormal();
		glm::vec3 boxPos = box->GetPosition();
		glm::vec3 extents = box->GetExtents();
		float boxToPlaneDistanceFirst = glm::dot(boxPos + extents, collisionNormal) + glm::length(plane->GetDistanceFromOrigin()) + 1;
		float boxToPlaneDistanceSecond = glm::dot(boxPos - extents, collisionNormal) + glm::length(plane->GetDistanceFromOrigin()) + 1;
		if ((boxToPlaneDistanceFirst < 0 && boxToPlaneDistanceSecond > 0) || (boxToPlaneDistanceFirst > 0 && boxToPlaneDistanceSecond < 0))
		{
			float distance = glm::dot(extents, collisionNormal);
			boxToPlaneDistanceFirst = abs(boxToPlaneDistanceFirst);
			boxToPlaneDistanceSecond = abs(boxToPlaneDistanceSecond);
			float intersection = boxToPlaneDistanceFirst < boxToPlaneDistanceSecond ? boxToPlaneDistanceFirst : boxToPlaneDistanceSecond;
			//std::cout << intersection << std::endl;
			glm::vec3 forceVector = -1 * box->GetMass() * collisionNormal * (glm::dot(collisionNormal, box->GetVelocity()));
			box->ApplyForce(forceVector * 2);
			box->SetPosition(box->GetPosition() + collisionNormal * intersection * 0.5f);
		//	box->SetVelocityToZero();
			return true;
		}
	}

	return false;

}

bool PhysicsScene3D::SphereToPlane(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return PlaneToSphere(second, first);
}

bool PhysicsScene3D::SphereToBox(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::PlaneToSphere(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Plane3D* plane = dynamic_cast<Plane3D*>(first);
	Sphere3D* sphere = dynamic_cast<Sphere3D*>(second);

	if (plane != nullptr && sphere != nullptr)
	{
		glm::vec3 collisionNormal = plane->GetNormal();
		aie::Gizmos::addLine(plane->GetDistanceFromOrigin() + collisionNormal, plane->GetDistanceFromOrigin() + collisionNormal * 4, glm::vec4(1, 0, 0, 1));
		float sphereToPlaneDistance = glm::dot(sphere->GetPosition(), collisionNormal) + glm::length(plane->GetDistanceFromOrigin()) + 1;

		if (sphereToPlaneDistance < 0)
		{
			sphereToPlaneDistance *= -1;
			collisionNormal *= -1;
		}

		float intersection = sphere->GetRadius() - sphereToPlaneDistance;
		if (intersection > 0)
		{
			glm::vec3 force = -1 * sphere->GetMass() * collisionNormal * glm::dot(sphere->GetVelocity(), collisionNormal);
			sphere->ApplyForce(force * 2);
			glm::vec3 separationVector = collisionNormal * intersection;
			sphere->SetPosition(sphere->GetPosition() + collisionNormal * intersection * 0.5f);

			return true;
		}
	}

	return false;
}

bool PhysicsScene3D::BoxToSphere(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::BoxToPlane(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return PlaneToBox(second, first);
}

bool PhysicsScene3D::BoxToBox(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Box3D* boxFirst = dynamic_cast<Box3D*>(first);
	Box3D* boxSecond = dynamic_cast<Box3D*>(second);

	if (boxFirst != nullptr && boxSecond != nullptr)
	{
		glm::vec3 boxFirstPos = boxFirst->GetPosition();
		glm::vec3 boxSecondPos = boxSecond->GetPosition();
		glm::vec3 extentsFirst = boxFirst->GetExtents();
		glm::vec3 extentsSecond = boxSecond->GetExtents();

		glm::vec3 boxFirstTopLeft = boxFirstPos + glm::vec3(-extentsFirst.x, extentsFirst.y, extentsFirst.z);
		glm::vec3 boxFirstBottomRight = boxFirstPos + glm::vec3(extentsFirst.x, -extentsFirst.y, extentsFirst.z);
		glm::vec3 boxSecondTopLeft = boxSecondPos + glm::vec3(-extentsSecond.x, extentsSecond.y, extentsSecond.z);
		glm::vec3 boxSecondBottomRight = boxSecondPos + glm::vec3(extentsSecond.x, -extentsSecond.y, extentsSecond.z);
		glm::vec3 boxFirstTopRight = boxFirstPos + extentsFirst;
		glm::vec3 boxSecondBottomLeft = boxSecondPos - extentsSecond;
		glm::vec3 boxSecondTopRight = boxSecondPos + extentsSecond;
		glm::vec3 boxFirstBottomLeft = boxFirstPos - extentsFirst;

		if (boxFirstBottomLeft.x <= boxSecondBottomRight.x && boxFirstBottomRight.x >= boxSecondBottomLeft.x)
		{
			if (boxFirstBottomLeft.y <= boxSecondTopLeft.y && boxFirstTopLeft.y >= boxSecondBottomRight.y)
			{
				//minZ < maxZ
				//maxZ > minZ
				if (boxFirstPos.z - extentsFirst.z <= boxSecondPos.z + extentsSecond.z && boxFirstPos.z + extentsFirst.z >= boxSecondPos.z - extentsSecond.z)
				{
					//boxFirst->SetVelocityToZero();
					//boxSecond->SetVelocityToZero();

					glm::vec3 delta = boxSecondPos - boxFirstPos;
					glm::vec3 collisionNormal = glm::normalize(delta);
					glm::vec3 relativeVelocity = boxFirst->GetVelocity() - boxSecond->GetVelocity();
					glm::vec3 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);

					glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / boxFirst->GetMass() + 1.0f / boxSecond->GetMass());
					boxFirst->ApplyForceToActor(boxSecond, forceVector*2);
					
					//calculate separation vector
					glm::vec3 firstExtents = boxFirst->GetExtents();
					glm::vec3 secondExtents = boxSecond->GetExtents();

					//move extents vector so that it is in the general direction of the collision normal
					if ((firstExtents.x < 0 && collisionNormal.x > 0) || (firstExtents.x > 0 && collisionNormal.x < 0))
					{
						firstExtents.x *= -1;
					}

					if ((firstExtents.y < 0 && collisionNormal.y > 0) || (firstExtents.y > 0 && collisionNormal.y < 0))
					{
						firstExtents.y *= -1;
					}

					if ((firstExtents.z < 0 && collisionNormal.z > 0) || (firstExtents.z > 0 && collisionNormal.z < 0))
					{
						firstExtents.z *= -1;
					}

					if ((secondExtents.x < 0 && collisionNormal.x > 0) || (secondExtents.x > 0 && collisionNormal.x < 0))
					{
						secondExtents.x *= -1;
					}

					if ((secondExtents.y < 0 && collisionNormal.y > 0) || (secondExtents.y > 0 && collisionNormal.y < 0))
					{
						secondExtents.y *= -1;
					}

					if ((secondExtents.z < 0 && collisionNormal.z > 0) || (secondExtents.z > 0 && collisionNormal.z < 0))
					{
						secondExtents.z *= -1;
					}

					float intersection = abs(glm::dot(firstExtents, collisionNormal)) + abs(glm::dot(secondExtents, collisionNormal)) - glm::length(delta);
					glm::vec3 separationVector = collisionNormal * intersection * 0.2f;
					
					boxFirst->SetPosition(boxFirstPos - separationVector);
					boxSecond->SetPosition(boxSecondPos + separationVector);

					//glm::vec3 torqueLever = glm::normalize(glm::vec3(delta.y, delta.z, delta.x)) * abs(glm::dot(firstExtents, collisionNormal));
					//float dot = glm::dot(glm::normalize(forceVector), glm::normalize(torqueLever));

					//float angle = acos(dot);
					////angle = acos(dot);
					//float magnitude = sin(angle) * glm::length(forceVector); //trig to find opposite side, or the perpendicular component
					//magnitude *= abs(glm::dot(firstExtents, collisionNormal)); //multiply by distance to get final magnitude
					//glm::vec3 direction = glm::cross(forceVector, torqueLever); //this will be the axis of rotation
					//glm::vec3 torque = direction * magnitude;

					////float torque = glm::dot(torqueLever, relativeVelocity) * 1.0f / (1 / sphereFirst->GetMass() + 1 / sphereSecond->GetMass());
					//boxFirst->ApplyTorque(-torque);
					//boxSecond->ApplyTorque(torque);


					return true;
				}
			}
		}
	}

	return false;
}

bool PhysicsScene3D::BoxToRay(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Box3D* box = dynamic_cast<Box3D*>(first);
	Ray3D* ray = dynamic_cast<Ray3D*>(second);

	if (box != nullptr && ray != nullptr)
	{
		glm::vec3 displacement = box->GetPosition() - ray->GetStartPosition();
		float displacementDot = glm::dot(ray->GetDirection(), glm::normalize(displacement));
		glm::vec3 displacementProjected = ray->GetStartPosition() + ray->GetDirection() * displacementDot * glm::length(displacement);
		float distance = glm::length(displacementProjected - box->GetPosition());
	
		glm::vec3 dir = glm::normalize(displacementProjected - box->GetPosition());
		glm::vec3 extents = box->GetExtents();

		if ((extents.x < 0 && dir.x > 0) || (extents.x > 0 && dir.x < 0))
		{
			extents.x *= -1;
		}

		if ((extents.y < 0 && dir.y > 0) || (extents.y > 0 && dir.y < 0))
		{
			extents.y *= -1;
		}

		if ((extents.z < 0 && dir.z > 0) || (extents.z > 0 && dir.z < 0))
		{
			extents.z *= -1;
		}
		//pseduo-radius
		float dot = abs(glm::dot(dir, extents));

		if (distance < dot)
		{
			return true;
		}
		
	}

	return false;
}

bool PhysicsScene3D::SphereToSphere(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Sphere3D* sphereFirst = dynamic_cast<Sphere3D*>(first);
	Sphere3D* sphereSecond = dynamic_cast<Sphere3D*>(second);
	
	if (sphereFirst != nullptr && sphereSecond != nullptr)
	{
		float distance = glm::distance(sphereFirst->GetPosition(), sphereSecond->GetPosition());
		float sumRadii = sphereFirst->GetRadius() + sphereSecond->GetRadius();
		if (distance <= sumRadii)
		{
			glm::vec3 delta = sphereSecond->GetPosition() - sphereFirst->GetPosition();
			glm::vec3 collisionNormal = glm::normalize(delta);
			glm::vec3 relativeVelocity = sphereFirst->GetVelocity() - sphereSecond->GetVelocity();
			glm::vec3 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);

			glm::vec3 forceVector = collisionVector * 1.0f / (1.0f / sphereFirst->GetMass() + 1.0f / sphereSecond->GetMass());
		
			//apply torque

			//lever arm = perpendicular distance from axis to a line along the force. Basically a perpendicular line from axis to force or a line extending the force.

			//get some lever
			//get force vector
			//get angle, use trig to calculate perpendicular component
			//perpendicular component x distance from lever equal magnitude of torque
			//get cross product of force and distance to get direction of torque
		
			glm::vec3 torqueLever = glm::normalize(glm::vec3(delta.y, delta.z, delta.x)) * sphereFirst->GetRadius();

			//try calculating real torque level at some point; perpendicular distance from pivot point to point where force is applied

		//	torqueLever = glm::normalize(forceVector - delta) * sphereFirst->GetRadius();
			float dot = glm::dot(glm::normalize(forceVector), glm::normalize(torqueLever));
			

			float angle = acos(dot);
			//angle = acos(dot);
			float magnitude = sin(angle) * glm::length(forceVector); //trig to find opposite side, or the perpendicular component
			magnitude *= sphereFirst->GetRadius(); //multiply by distance to get final magnitude
			glm::vec3 direction = glm::cross(forceVector, torqueLever); //this will be the axis of rotation
			glm::vec3 torque = direction * magnitude;

			//float torque = glm::dot(torqueLever, relativeVelocity) * 1.0f / (1 / sphereFirst->GetMass() + 1 / sphereSecond->GetMass());
			sphereFirst->ApplyTorque(-torque);
			sphereSecond->ApplyTorque(torque);

			sphereFirst->ApplyForceToActor(sphereSecond, forceVector * 2);
			float intersection = sphereFirst->GetRadius() + sphereSecond->GetRadius() - distance;
			glm::vec3 separationVector = collisionNormal * intersection * 0.5f;
			
			sphereFirst->SetPosition(sphereFirst->GetPosition() - separationVector);
			sphereSecond->SetPosition(sphereSecond->GetPosition() + separationVector);

			return true;
		}
	}

	return false;
}

bool PhysicsScene3D::PlaneToRay(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::RayToPlane(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::SphereToRay(PhysicsObject3D * first, PhysicsObject3D * second)
{
	Sphere3D* sphere = dynamic_cast<Sphere3D*>(first);
	Ray3D* ray = dynamic_cast<Ray3D*>(second);
	if (sphere != nullptr && ray != nullptr)
	{
		glm::vec3 displacement = sphere->GetPosition() - ray->GetStartPosition();
		glm::vec3 displacementProjected = ray->GetDirection() * glm::dot(ray->GetDirection(), displacement);
		float distance = glm::length(displacementProjected - displacement);
		glm::vec3 collisionPoint = displacementProjected - displacement;
		if (distance < sphere->GetRadius())
		{
			ray->SetLastHit(sphere);
			
			if (glm::distance(collisionPoint, ray->GetStartPosition()) < ray->GetLength())
				return true;
		}
	}

	return false;
}

bool PhysicsScene3D::RayToSphere(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return SphereToRay(second, first);
}

bool PhysicsScene3D::RayToBox(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}

bool PhysicsScene3D::RayToRay(PhysicsObject3D * first, PhysicsObject3D * second)
{
	return false;
}
