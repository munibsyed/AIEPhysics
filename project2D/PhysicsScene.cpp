#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

std::pair<float, float> GetProjection(glm::vec2 axis, glm::vec2 *vertices, int numVertices)
{
	float min, max;

	glm::vec2 currentAxis = axis;
	glm::vec2 normalizedAxis = glm::normalize(currentAxis);
	min = glm::dot(normalizedAxis, vertices[0]);
	max = min;
	for (int i = 1; i < numVertices; i++)
	{
		//normalize the axis
		float projection = glm::dot(normalizedAxis, vertices[i]);
		if (projection < min)
		{
			min = projection;
		}
		else if (projection > max)
		{
			max = projection;
		}

	}
	//aie::Gizmos::add2DLine(currentAxis - (normalizedAxis), currentAxis + (normalizedAxis), glm::vec4(0, 1, 0, 1));

	return std::pair<float, float>(min, max);
}

#ifndef isnan
inline bool isnan(double x) {
	return x != x;
}
#endif

bool LineIntersectsSphere(glm::vec2 startPoint, glm::vec2 endPoint, glm::vec2 circleCenter, float circleRadius)
{
	//line intersection formula
	//x0, y0 = centre point of circle
	//x1,y1 = start point
	//x2,y2 = end point

	/*float numerator = abs(((endPoint.x - startPoint.x) * circleCenter.x) + ((startPoint.y - endPoint.y) * circleCenter.y) +
		((startPoint.x - endPoint.x)*startPoint.y) + (startPoint.x*(endPoint.y - startPoint.y)));

	float denominator = sqrt(pow(endPoint.x - startPoint.x,2) + pow(startPoint.y - endPoint.y,2));
	float distance = numerator / denominator;
	std::cout << "Distance: " << distance << std::endl;
	return distance <= circleRadius;*/
	glm::vec2 dir = glm::normalize(endPoint - startPoint);
	float projection = glm::dot(circleCenter, dir);
	glm::vec2 projectedPoint = projection * dir;
	projectedPoint += startPoint;
	//std::cout << glm::length(projectedPoint) << std::endl;


	glm::vec2 L = circleCenter - startPoint;
	float lProjDir = glm::dot(L, dir);
	if (lProjDir < 0)
		return false;
	
	float d = sqrt(pow(glm::length(L), 2) - pow(glm::length(lProjDir*dir), 2));
	if (d < 0)
		return false;

	if (d > circleRadius)
	{
		return false;
	}

	float hc = sqrt(pow(circleRadius, 2) - pow(d, 2));
	float t0 = glm::length(lProjDir*dir) - hc;
	float t1 = glm::length(lProjDir * dir) + hc;

	if (t0 > glm::length(endPoint - startPoint) && t1 > glm::length(endPoint - startPoint))
	{
		return false;
	}

	glm::vec2 p1 = startPoint + t0 * dir;
	glm::vec2 p2 = startPoint + t1 * dir;

	return true;
}

static fn CollisionFunctionArray[] = 
{
	PhysicsScene::PlaneToPlane, PhysicsScene::PlaneToSphere, PhysicsScene::PlaneToBox, PhysicsScene::PlaneToTriangle,
	PhysicsScene::SphereToPlane, PhysicsScene::SphereToSphere, PhysicsScene::SphereToBox, PhysicsScene::SphereToTriangle,
	PhysicsScene::BoxToPlane, PhysicsScene::BoxToSphere, PhysicsScene::BoxToBox, PhysicsScene::BoxToTriangle,
	PhysicsScene::TriangleToPlane, PhysicsScene::TriangleToSphere, PhysicsScene::TriangleToBox, PhysicsScene::TriangleToTriangle
};

PhysicsScene::PhysicsScene()
{
	m_numActors = 0;
}
bool PhysicsScene::m_hasBroken = false;

PhysicsScene::~PhysicsScene()
{
	for (int i = 0; i < m_numActors; i++)
	{
		delete m_actors[i];
	}
}

void PhysicsScene::AddActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
	m_numActors++;
}

void PhysicsScene::RemoveActor(PhysicsObject * actor)
{
	auto item = std::find(m_actors.begin(), m_actors.end(), actor);
	if (item < m_actors.end())
	{
		m_actors.erase(item); 
	}
	m_numActors--;
}

void PhysicsScene::Update(float dt)
{
	//update physics at a fixed time steps
	static float timer = 0;
	timer += dt;
	if (timer >= m_timeStep)
	{
		for (int i = 0; i < m_numActors; i++)
		{
			m_actors[i]->FixedUpdate(m_gravity, timer);
		}
		timer = 0;
	}
}

void PhysicsScene::UpdateGizmos()
{
	for (int i = 0; i < m_numActors; i++)
	{
		m_actors[i]->MakeGizmo();
	}
}

void PhysicsScene::DebugScene()
{
	for (int i = 0; i < m_numActors; i++)
	{
		m_actors[i]->Debug();
	}
}

void PhysicsScene::CheckForCollision()
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		Rigidbody* rb = dynamic_cast<Rigidbody*>(m_actors[i]);
		if (rb != nullptr)
		{
			if (rb->GetPosition().y < -99)
				RemoveActor(m_actors[i]);
		}
	}

	//check for collisions against all objects except this one
	for (int outer = 0; outer < m_numActors - 1; outer++)
	{
		for (int inner = outer + 1; inner < m_numActors; inner++)
		{
			PhysicsObject* object1 = m_actors[inner];
			PhysicsObject* object2 = m_actors[outer];

			

			if (object1->GetShapeID() == TRIANGLE && object2->GetShapeID() == TRIANGLE)
			{
				int x = 0;
			}

			int shapeIDFirst = object1->GetShapeID();
			int shapeIDSecond = object2->GetShapeID();

			if (shapeIDFirst < 0 || shapeIDSecond < 0)
				continue;

			//function pointers
			int functionIdx = shapeIDFirst * SHAPE_COUNT + shapeIDSecond;
			fn collisionFunctionPtr = CollisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				if (collisionFunctionPtr(object1, object2) == true)
				{
					m_collisions++;
				}
			}
			m_iterations++;
			if (m_iterations == 100)
			{
				if (m_collisions > 0)
				{
					//std::cout << "Collisons in ten: " << m_collisions << std::endl;
					if (m_collisions > 1)
					{
						int x = 0;
					}
				}
				m_iterations = 0;
				m_collisions = 0;
			}
		}
	}
}

void PhysicsScene::SetGravity(const glm::vec2 gravity)
{
	m_gravity = gravity;
}

glm::vec2 PhysicsScene::GetGravity() const
{
	return m_gravity;
}

void PhysicsScene::SetTimeStep(const float timeStep)
{
	m_timeStep = timeStep;
}

float PhysicsScene::GetTimeStep() const
{
	return m_timeStep;
}

std::vector<PhysicsObject*> PhysicsScene::GetActors() const
{
	return m_actors;
}

int PhysicsScene::GetNumActors()
{
	return m_numActors;
}

bool PhysicsScene::PlaneToPlane(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::PlaneToSphere(PhysicsObject * first, PhysicsObject * second)
{
	Plane* plane = dynamic_cast<Plane*>(first);
	Sphere* sphere = dynamic_cast<Sphere*>(second);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		//distance from sphere to origin projected along normal	
		float sphereToPlaneDistance = glm::dot(sphere->GetPosition(), collisionNormal) + plane->GetDistanceToOrigin();

		//if we are behind the plane then flip the normal
		if (sphereToPlaneDistance < 0)
		{
			collisionNormal *= -1;
			sphereToPlaneDistance *= -1;
		}
		float intersection = sphere->GetRadius() - sphereToPlaneDistance;
		
		if (intersection > 0)
		{	
			glm::vec2 forceVector = -1 * sphere->GetMass() * collisionNormal * (glm::dot(collisionNormal, sphere->GetVelocity()));
			sphere->ApplyForce(forceVector * 2);
			sphere->SetPosition(sphere->GetPosition() + collisionNormal * intersection * 0.5f);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::SphereToPlane(PhysicsObject * first, PhysicsObject * second)
{
	return PlaneToSphere(second, first);
}

bool PhysicsScene::SphereToSphere(PhysicsObject * first, PhysicsObject * second)
{
	//perform cast
	Sphere* sphereFirst = dynamic_cast<Sphere*>(first);
	Sphere* sphereSecond = dynamic_cast<Sphere*>(second);

	if (sphereFirst != nullptr && sphereSecond != nullptr) 
	{
		float distance = glm::length(sphereFirst->GetPosition() - sphereSecond->GetPosition());
		if (distance < sphereFirst->GetRadius() + sphereSecond->GetRadius())
		{
			std::cout << "Circle collision!" << std::endl;	
			
			glm::vec2 delta = sphereSecond->GetPosition() - sphereFirst->GetPosition();
			glm::vec2 collisionNormal = glm::normalize(delta);
			glm::vec2 relativeVelocity = sphereFirst->GetVelocity() - sphereSecond->GetVelocity();
			glm::vec2 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);
			glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / sphereFirst->GetMass() + 1.0f / sphereSecond->GetMass());

			glm::vec2 torqueLever = glm::normalize(glm::vec2(delta.y, -delta.x)) * sphereFirst->GetRadius();
			
			float torque = glm::dot(torqueLever, relativeVelocity) * 1.0f / (1 / sphereFirst->GetMass() + 1 / sphereSecond->GetMass());

			sphereFirst->ApplyTorque(-torque);
			sphereSecond->ApplyTorque(torque);
			
			float combinedElasticity = (sphereFirst->GetElasticity() + sphereSecond->GetElasticity()) / 2.0f;

			//add force
			sphereFirst->ApplyForceToActor(sphereSecond, forceVector + (forceVector * combinedElasticity));
			//move spheres out of collision
			float intersection = sphereFirst->GetRadius() + sphereSecond->GetRadius() - distance;
			glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
			
			//if (sphereFirst->IsStatic() == false)
			//{
			//	if (sphereSecond->IsStatic() == true)
			//		sphereFirst->SetPosition(sphereFirst->GetPosition() - (separationVector*2));
			//	else
			//		sphereFirst->SetPosition(sphereFirst->GetPosition() - (separationVector));
			//
			//}
			//
			//if (sphereSecond->IsStatic() == false)
			//{
			//	if (sphereFirst->IsStatic() == true)
			//		sphereSecond->SetPosition(sphereFirst->GetPosition() + (separationVector * 2));
			//	else
			//		sphereSecond->SetPosition(sphereFirst->GetPosition() + (separationVector));
			//
			//}

			if (sphereFirst->IsStatic() == false)
				sphereFirst->SetPosition(sphereFirst->GetPosition() - (separationVector));
			if (sphereSecond->IsStatic() == false)
				sphereSecond->SetPosition(sphereSecond->GetPosition() + (separationVector));


			return true;
		}
	}

	return false;
}

bool PhysicsScene::PlaneToBox(PhysicsObject * first, PhysicsObject * second)
{
	Plane* plane = dynamic_cast<Plane*>(first);
	Box* box = dynamic_cast<Box*>(second);
	if (plane != nullptr && box != nullptr)
	{
		glm::vec2 collisionNormal = plane->GetNormal();
		glm::vec2 boxPos = box->GetPosition();
		glm::vec2 extents = box->GetExtents();
		box->CalculateCorners();
		glm::vec2* corners = box->GetCorners();
		float minDistance = INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			float cornerToPlane = glm::dot(corners[i] - collisionNormal, collisionNormal) + plane->GetDistanceToOrigin() + 1;
			if (cornerToPlane < minDistance)
			{
				minDistance = cornerToPlane;
			}
		}


		float boxToPlane = glm::dot(boxPos, collisionNormal) + plane->GetDistanceToOrigin();
		float closestEdgeToPlane = glm::dot(boxPos + (extents * (-collisionNormal)), collisionNormal) + plane->GetDistanceToOrigin();
		closestEdgeToPlane = minDistance;
		//std::cout << minDistance << std::endl;

		//aie::Gizmos::add2DCircle(boxPos + (extents * (-collisionNormal)), 1, 8, glm::vec4(1, 0, 0, 1));
		if (boxToPlane < 0)
		{
			collisionNormal *= -1;
		}

		if (closestEdgeToPlane <= 0)
		{
			//box->SetVelocityToZero();
			float intersection = sqrt(pow(extents.x, 2) + pow(extents.y, 2)) - boxToPlane;
			glm::vec2 forceVector = -1 * box->GetMass() * collisionNormal * (glm::dot(collisionNormal, box->GetVelocity()));
			//std::cout << "Applying force from PlaneToBox()" << std::endl;
			box->ApplyForce(forceVector * 2);
			box->SetPosition(box->GetPosition() + collisionNormal * intersection * 0.5f);
		}


	}
	return false;
}

bool PhysicsScene::SphereToBox(PhysicsObject * first, PhysicsObject * second)
{
	Sphere* sphere = dynamic_cast<Sphere*>(first);
	Box* box = dynamic_cast<Box*>(second);

	if (sphere != nullptr && box != nullptr)
	{
		bool useMethod1 = true;

		if (useMethod1 == true)
		{
			glm::vec2 extents = box->GetExtents();
			//get 'radius' of box at the direction to the circle
			glm::vec2 direction = glm::normalize(sphere->GetPosition() - box->GetPosition());
			glm::vec2 widthAxis = glm::vec2(1, 0);
			glm::vec2 heightAxis = glm::vec2(0, 1);

			float xProjection = glm::dot(direction, widthAxis);
			float yProjection = glm::dot(direction, heightAxis);
			float xDir = xProjection;
			float yDir = yProjection;
			xProjection *= extents.x;
			yProjection *= extents.y;
		
			float angle = glm::angle(glm::vec2(1, 0), direction);
		
			float ratio = tan(angle);
			angle = glm::degrees(angle);
			glm::vec2 width(extents.x, 0);
			width *= ratio;
			glm::vec2 height(0, extents.y);
			height *= ratio;
			//aie::Gizmos::add2DCircle(box->GetPosition() + width + height, 1, 8, glm::vec4(0, 1, 0, 1));
			//std::cout << ratio << std::endl;
			glm::vec2 vectorToPoint(-1, -1);
			//std::cout << angle << std::endl;
		
			if (abs(direction.x) > abs(direction.y)) //then x is max
			{
	
				vectorToPoint.x = extents.x;
				vectorToPoint.y = ratio * vectorToPoint.x;
				if (direction.x < 0)
				{
					vectorToPoint.x = -extents.x;
					vectorToPoint.y = ratio * vectorToPoint.x;

					if (direction.y < 0)
					{
						vectorToPoint.y *= -1;
					}
				}
			}

			else
			{
				vectorToPoint.y = extents.y;
				vectorToPoint.x = vectorToPoint.y / ratio;
			}

			if (vectorToPoint != glm::vec2(-1, -1))
			{
				//aie::Gizmos::add2DCircle(box->GetPosition() + vectorToPoint, 1, 8, glm::vec4(0, 1, 0, 1));
			}

			//aie::Gizmos::add2DLine(box->GetPosition(), box->GetPosition() + glm::vec2(xProjection, 0), glm::vec4(0, 1, 0, 1));
			//aie::Gizmos::add2DLine(box->GetPosition(), box->GetPosition() + glm::vec2(0, yProjection), glm::vec4(0, 1, 0, 1));

			float hyp = sqrt(pow(xProjection, 2) + pow(yProjection, 2));
			//aie::Gizmos::add2DCircle(box->GetPosition() + glm::vec2(xProjection, yProjection), 1, 8, glm::vec4(0, 1, 0, 1));
		
		//	float distance = glm::distance(box->GetPosition() + glm::vec2(xProjection, yProjection), box->GetPosition());
			float distance = glm::distance(box->GetPosition() + vectorToPoint, box->GetPosition());

			//leave this method in
			if (glm::distance(sphere->GetPosition(), box->GetPosition()) < distance + sphere->GetRadius())
			{
				std::cout << "Circle box collision!" << std::endl;
			
				sphere->SetVelocityToZero();
				box->SetVelocityToZero();

				//glm::vec2 delta = box->GetPosition() - sphere->GetPosition();
				//glm::vec2 collisionNormal = glm::normalize(delta);
				//glm::vec2 relativeVelocity = sphere->GetVelocity() - box->GetVelocity();
				//glm::vec2 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);
				//glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / box->GetMass() + 1.0f / sphere->GetMass());
				//sphere->ApplyForceToActor(box, forceVector * 2);
				//
				////move spheres out of collision
				//float intersection = glm::distance(sphere->GetPosition(), box->GetPosition()) - distance;
				//glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
				//
				//box->SetPosition(box->GetPosition() + separationVector);
				//sphere->SetPosition(sphere->GetPosition() - separationVector);

				return true;
			}
		}
		else
		{


			//check if centre of circle is inside rectangle
			glm::vec2 circlePosition = sphere->GetPosition();
			//top right, top left, bottom left, bottom right
			glm::vec2* corners = box->GetCorners();
			if (circlePosition.x > corners[1].x && circlePosition.x < corners[0].x && circlePosition.y > corners[2].y && circlePosition.y < corners[0].y)
			{
				sphere->SetVelocityToZero();
				return true;
			}
			else
			{

				//top
				if (LineIntersectsSphere(glm::vec2(corners[1].x, corners[1].y), glm::vec2(corners[0].x, corners[0].y), sphere->GetPosition(), sphere->GetRadius()))
				{
					sphere->SetVelocityToZero();
				}

				//left
				else if (LineIntersectsSphere(glm::vec2(corners[1].x, corners[1].y), glm::vec2(corners[2].x, corners[2].y), sphere->GetPosition(), sphere->GetRadius()))
				{
					sphere->SetVelocityToZero();
				}
				//right
				else if (LineIntersectsSphere(glm::vec2(corners[0].x, corners[0].y), glm::vec2(corners[3].x, corners[3].y), sphere->GetPosition(), sphere->GetRadius()))
				{
					sphere->SetVelocityToZero();
				}
				//bottom
				else if (LineIntersectsSphere(glm::vec2(corners[2].x, corners[2].y), glm::vec2(corners[3].x, corners[3].y), sphere->GetPosition(), sphere->GetRadius()))
				{
					sphere->SetVelocityToZero();
				}



			}

		}

	}

	return false;
}

//bool PhysicsScene::BoxToBox(PhysicsObject * first, PhysicsObject * second)
//{
//	Box* boxFirst = dynamic_cast<Box*>(first);
//	Box* boxSecond = dynamic_cast<Box*>(second);
//	
//	if (boxFirst != nullptr && boxSecond != nullptr)
//	{
//		bool useMinkowski = false;
//		if (boxFirst->GetRotation() == 0 && boxSecond->GetRotation() == 0)
//		{	
//			if (useMinkowski == false)
//			{
//				glm::vec2 extentsFirst = boxFirst->GetExtents(); //halve the extents
//				glm::vec2 extentsSecond = boxSecond->GetExtents(); //halve the extents
//
//				//extentsFirst += glm::vec2(2.0f, 2.0f);
//				//extentsSecond += glm::vec2(2.0f, 2.0f);
//
//				glm::vec2 boxFirstTopLeft = boxFirst->GetPosition() + glm::vec2(-extentsFirst.x, extentsFirst.y);
//				glm::vec2 boxFirstBottomRight = boxFirst->GetPosition() + glm::vec2(extentsFirst.x, -extentsFirst.y);
//				glm::vec2 boxSecondTopLeft = boxSecond->GetPosition() + glm::vec2(-extentsSecond.x, extentsSecond.y);
//				glm::vec2 boxSecondBottomRight = boxSecond->GetPosition() + glm::vec2(extentsSecond.x, -extentsSecond.y);
//				glm::vec2 boxFirstTopRight = boxFirst->GetPosition() + extentsFirst;
//				glm::vec2 boxSecondBottomLeft = boxSecond->GetPosition() - extentsSecond;
//				glm::vec2 boxSecondTopRight = boxSecond->GetPosition() + extentsSecond;
//				glm::vec2 boxFirstBottomLeft = boxFirst->GetPosition() - extentsFirst;
//
//				//aie::Gizmos::add2DCircle(boxFirstBottomLeft, 1, 8, glm::vec4(1, 0, 0, 1));
//				//aie::Gizmos::add2DCircle(boxFirstTopRight, 1, 8, glm::vec4(1, 0, 0, 1));
//				//aie::Gizmos::add2DCircle(boxFirstTopLeft, 1, 8, glm::vec4(1, 0, 0, 1));
//				//aie::Gizmos::add2DCircle(boxFirstBottomRight, 1, 8, glm::vec4(1, 0, 0, 1));
//
//				if (boxFirstBottomLeft.x <= boxSecondBottomRight.x && boxFirstBottomRight.x >= boxSecondBottomLeft.x)
//				{
//					if (boxFirstBottomLeft.y <= boxSecondTopLeft.y && boxFirstTopLeft.y >= boxSecondBottomRight.y)
//					{
//						glm::vec2 delta = boxSecond->GetPosition() - boxFirst->GetPosition();
//						glm::vec2 collisionNormal = glm::normalize(delta);
//						glm::vec2 relativeVelocity = boxFirst->GetVelocity() - boxSecond->GetVelocity();
//						glm::vec2 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);
//						glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / boxFirst->GetMass() + 1.0f / boxSecond->GetMass());
//						boxFirst->ApplyForceToActor(boxSecond, forceVector*2);
//						//float intersection = abs(glm::dot(boxFirst->GetExtents(), collisionNormal)) + abs(glm::dot(boxSecond->GetExtents(), collisionNormal)) - glm::length(delta);
//						glm::vec2 firstExtents = boxFirst->GetExtents();
//						glm::vec2 secondExtents = boxSecond->GetExtents();
//
//						//move extents vector so that it is in the general direction of the collision normal
//						if ((firstExtents.x < 0 && collisionNormal.x > 0) || (firstExtents.x > 0 && collisionNormal.x < 0))
//						{
//							firstExtents.x *= -1;
//						}
//
//						if ((firstExtents.y < 0 && collisionNormal.y > 0) || (firstExtents.y > 0 && collisionNormal.y < 0))
//						{
//							firstExtents.y *= -1;
//						}
//
//						if ((secondExtents.x < 0 && collisionNormal.x > 0) || (secondExtents.x > 0 && collisionNormal.x < 0))
//						{
//							secondExtents.x *= -1;
//						}
//
//						if ((secondExtents.y < 0 && collisionNormal.y > 0) || (secondExtents.y > 0 && collisionNormal.y < 0))
//						{
//							secondExtents.y *= -1;
//						}
//
//						//pseudo-radius
//						float dotFirst = abs(glm::dot((firstExtents), collisionNormal));
//						float dotSecond = abs(glm::dot((secondExtents), collisionNormal));
//						
//						//float intersection = glm::length(firstExtentsProjected) + glm::length(secondExtentsProjected) - glm::length(delta);
//						float intersection = dotFirst + dotSecond - glm::length(delta);
//						glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
//
//						//std::cout << separationVector.x << ", " << separationVector.y << std::endl;
//
//						boxFirst->SetPosition(boxFirst->GetPosition() - separationVector);
//						boxSecond->SetPosition(boxSecond->GetPosition() + separationVector);
//
//						return true;
//					}
//				}
//				return false;
//
//			}
//			else
//			{
//				glm::vec2 newExtents = boxFirst->GetExtents() + boxSecond->GetExtents();
//				glm::vec2 topLeft;
//				topLeft.x = (boxFirst->GetPosition().x - boxFirst->GetExtents().x) - (boxSecond->GetPosition().x + boxSecond->GetExtents().x); //leftmost point of first minus rightmost point of second
//				topLeft.y = (boxFirst->GetPosition().y + boxFirst->GetExtents().y) - (boxSecond->GetPosition().y - boxSecond->GetExtents().y);  //boxFirst top - boxSecond bottom
//				Box *newBox = new Box(topLeft + glm::vec2(newExtents.x, -newExtents.y), newExtents, 1, glm::vec4(1, 1, 1, 1));
//				if (newBox->GetPosition().x - newBox->GetExtents().x <= 0 &&
//					newBox->GetPosition().x + newBox->GetExtents().x >= 0 &&
//					newBox->GetPosition().y - newBox->GetExtents().y <= 0 &&
//					newBox->GetPosition().y + newBox->GetExtents().y >= 0)
//				{
//					std::cout << "Colliding" << std::endl;
//					boxFirst->SetVelocityToZero();
//					boxSecond->SetVelocityToZero();
//					return true;
//				}
//			}
//
//		}
//
//		else //non-AABB
//		{
//			if (useMinkowski == false)
//			{
//				boxFirst->CalculateCorners();
//				boxFirst->CalculateEdges();
//
//				boxSecond->CalculateCorners();
//				boxSecond->CalculateEdges();
//
//				glm::vec2 *firstAxes = boxFirst->GetEdges(); //axes to test are the normals of each edge on the shapes
//				glm::vec2 *secondAxes = boxSecond->GetEdges();
//				glm::vec2 *firstVertices = boxFirst->GetCorners();
//				glm::vec2 *secondVertices = boxSecond->GetCorners();
//
//
//				float rotationFirst = boxFirst->GetRotation();
//				float rotationSecond = boxSecond->GetRotation();
//
//				glm::vec2 firstExtents = boxFirst->GetExtents();
//				glm::vec2 secondExtents = boxSecond->GetExtents();
//
//				float scaleFirst = sqrt(pow(boxFirst->GetExtents().x, 2) + pow(boxFirst->GetExtents().y, 2));
//				float scaleSecond = sqrt(pow(boxSecond->GetExtents().x, 2) + pow(boxSecond->GetExtents().y, 2));
//
//				float rotFirst = rotationFirst;
//				float rotSecond = rotationSecond;
//				float vertexRotFirst = rotationFirst + (3.1415f / 4.0f);
//				float vertexRotSecond = rotationSecond + (3.1415f / 4.0f);
//			
//				for (int i = 0; i < 4; i++) 
//				{
//					glm::vec2 axis = firstAxes[i];
//					axis = axis - boxFirst->GetPosition();
//					std::pair<float, float> projection1 = GetProjection(axis, firstVertices, 4);
//					std::pair<float, float> projection2 = GetProjection(axis, secondVertices, 4);
//
//					//check for no overlap
//					if (projection1.second < projection2.first || projection2.second < projection1.first)
//					{
//						//std::cout << "No overlap" << std::endl;
//			
//						return false;
//					}
//
//					axis = secondAxes[i];
//					axis = axis - boxSecond->GetPosition();
//					projection1 = GetProjection(axis, firstVertices, 4);
//					projection2 = GetProjection(axis, secondVertices, 4);
//
//					//check for no overlap
//					if (projection1.second < projection2.first || projection2.second < projection1.first)
//					{
//						//std::cout << "No overlap" << std::endl;
//					
//						return false;
//					}
//					glm::vec2 delta = boxSecond->GetPosition() - boxFirst->GetPosition();
//					//std::cout << "Length: " << glm::length(delta) << std::endl;
//
//					if (isnan(delta.y))
//					{
//						__debugbreak();
//					}
//
//					boxFirst->SetVelocityToZero();
//					boxSecond->SetVelocityToZero();
//					return true;
//
//					glm::vec2 collisionNormal = glm::normalize(delta);
//					glm::vec2 relativeVelocity = boxFirst->GetVelocity() - boxSecond->GetVelocity();
//					glm::vec2 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
//					glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / boxFirst->GetMass() + 1.0f / boxSecond->GetMass());
//					//std::cout << "Applying force from BoxToBox(): " << glm::length(forceVector*2) << std::endl;
//					boxFirst->ApplyForceToActor(boxSecond, forceVector * 2);
//					float angle = glm::angle(glm::vec2(1, 0), collisionNormal);
//					float ratio = tan(angle);
//					angle = glm::degrees(angle);
//					glm::vec2 width(boxFirst->GetExtents().x, 0);
//					width = width * ratio;
//					glm::vec2 height(0, boxFirst->GetExtents().y);
//					height = height * ratio;
//					//aie::Gizmos::add2DCircle(box->GetPosition() + width + height, 1, 8, glm::vec4(0, 1, 0, 1));
//					//std::cout << ratio << std::endl;
//					glm::vec2 boxExtentAtPoint(-1, -1);
//					glm::vec2 box2ExtentAtPoint(-1, -1);
//					//std::cout << angle << std::endl;
//
//					if (abs(collisionNormal.x) > abs(collisionNormal.y)) //then x is max
//					{
//						boxExtentAtPoint.x = boxFirst->GetExtents().x;
//						boxExtentAtPoint.y = ratio * boxExtentAtPoint.x;
//						if (collisionNormal.x < 0)
//						{
//							boxExtentAtPoint.x = -boxFirst->GetExtents().x;
//							boxExtentAtPoint.y = ratio * boxExtentAtPoint.x;
//
//							if (collisionNormal.y < 0)
//							{
//								boxExtentAtPoint.y *= -1;
//							}
//						}
//					}
//					else
//					{
//						boxExtentAtPoint.y = boxFirst->GetExtents().y;
//						boxExtentAtPoint.x = boxExtentAtPoint.y / ratio;
//					}
//					delta = boxFirst->GetPosition() - boxSecond->GetPosition();
//					collisionNormal = glm::normalize(delta);
//					angle = glm::angle(glm::vec2(1, 0), collisionNormal);
//
//					ratio = tan(angle);
//					angle = glm::degrees(angle);
//					width = glm::vec2(boxSecond->GetExtents().x, 0);
//					width *= ratio;
//					height = glm::vec2(0, boxSecond->GetExtents().y);
//					height *= ratio;
//					//aie::Gizmos::add2DCircle(box->GetPosition() + width + height, 1, 8, glm::vec4(0, 1, 0, 1));
//					//std::cout << ratio << std::endl;
//					//std::cout << angle << std::endl;
//
//					if (abs(collisionNormal.x) > abs(collisionNormal.y)) //then x is max
//					{
//						box2ExtentAtPoint.x = boxSecond->GetExtents().x;
//						box2ExtentAtPoint.y = ratio * box2ExtentAtPoint.x;
//						if (collisionNormal.x < 0)
//						{
//							box2ExtentAtPoint.x = -boxSecond->GetExtents().x;
//							box2ExtentAtPoint.y = ratio * box2ExtentAtPoint.x;
//
//							if (collisionNormal.y < 0)
//							{
//								box2ExtentAtPoint.y *= -1;
//							}
//						}
//					}
//					else
//					{
//						box2ExtentAtPoint.y = boxSecond->GetExtents().y;
//						box2ExtentAtPoint.x = box2ExtentAtPoint.y / ratio;
//					}
//			
//					//Pythagoras method
//					//float intersection = sqrt(pow(boxFirst->GetExtents().x, 2) + pow(boxFirst->GetExtents().y, 2)) + sqrt(pow(boxSecond->GetExtents().x,2) + pow(boxSecond->GetExtents().y, 2)) - glm::length(delta);
//					//glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
//					////	glm::vec2 separationVector = collisionNormal * (glm::distance(boxFirst->GetPosition(), boxFirst->GetPosition() + boxExtentAtPoint) + glm::distance(boxSecond->GetPosition(), boxSecond->GetPosition() + box2ExtentAtPoint) - delta) * 0.5f;
//
//					//boxFirst->SetPosition(boxFirst->GetPosition() + separationVector);
//					//boxSecond->SetPosition(boxSecond->GetPosition() - separationVector);
//
//					glm::vec2 firstExtents = boxFirst->GetExtents();
//					glm::vec2 secondExtents = boxSecond->GetExtents();
//
//					//move extents vector so that it is in the general direction of the collision normal
//					if ((firstExtents.x < 0 && collisionNormal.x > 0) || (firstExtents.x > 0 && collisionNormal.x < 0))
//					{
//						firstExtents.x *= -1;
//					}
//
//					if ((firstExtents.y < 0 && collisionNormal.y > 0) || (firstExtents.y > 0 && collisionNormal.y < 0))
//					{
//						firstExtents.y *= -1;
//					}
//
//					if ((secondExtents.x < 0 && collisionNormal.x > 0) || (secondExtents.x > 0 && collisionNormal.x < 0))
//					{
//						secondExtents.x *= -1;
//					}
//
//					if ((secondExtents.y < 0 && collisionNormal.y > 0) || (secondExtents.y > 0 && collisionNormal.y < 0))
//					{
//						secondExtents.y *= -1;
//					}
//
//					//pseudo-radius
//					float dotFirst = abs(glm::dot((firstExtents), collisionNormal));
//					float dotSecond = abs(glm::dot((secondExtents), collisionNormal));
//
//					//float intersection = glm::length(firstExtentsProjected) + glm::length(secondExtentsProjected) - glm::length(delta);
//					float intersection = dotFirst + dotSecond - glm::length(delta);
//					glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
//		
//					return true;
//				}
//			}
//			else //use minkowski
//			{
//				//glm::vec2 newExtents = boxFirst->GetExtents() + boxSecond->GetExtents();
//				//glm::vec2 topLeft;
//
//				//glm::vec2* boxFirstCorners = boxFirst->GetCorners(); //top right, top left, bottom left, bottom right
//				//glm::vec2* boxSecondCorners = boxSecond->GetCorners();
//
//				//glm::vec2 boxFirstLeftmost = boxFirstCorners[1].x < boxFirstCorners[2].x ? boxFirstCorners[1] : boxFirstCorners[2];
//				//glm::vec2 boxSecondRightmost = boxSecondCorners[0].x > boxSecondCorners[3].x ? boxSecondCorners[0] : boxSecondCorners[3];
//				//glm::vec2 boxFirstTopMost = 
//
//
//				//topLeft.x = boxFirstLeftmost.x - boxSecondRightmost.x; //leftmost point of first minus rightmost point of second
//				//topLeft.y = (boxFirst->GetPosition().y + boxFirst->GetExtents().y) - (boxSecond->GetPosition().y - boxSecond->GetExtents().y);  //boxFirst top - boxSecond bottom
//				//Box *newBox = new Box(topLeft + glm::vec2(newExtents.x, -newExtents.y), newExtents, 1, glm::vec4(1, 1, 1, 1));
//				//if (newBox->GetPosition().x - newBox->GetExtents().x <= 0 && //min x
//				//	newBox->GetPosition().x + newBox->GetExtents().x >= 0 && //max x
//				//	newBox->GetPosition().y - newBox->GetExtents().y <= 0 && //min y
//				//	newBox->GetPosition().y + newBox->GetExtents().y >= 0) //max y	
//				//{
//				//	std::cout << "Colliding" << std::endl;
//				//	boxFirst->SetVelocityToZero();
//				//	boxSecond->SetVelocityToZero();
//				//	return true;
//				//}
//			}
//
//		}
//
//	}
//}
bool PhysicsScene::BoxToBox(PhysicsObject * first, PhysicsObject * second)
{
Box* boxFirst = dynamic_cast<Box*>(first);
Box* boxSecond = dynamic_cast<Box*>(second);

if (boxFirst != nullptr && boxSecond != nullptr)
{
	if (boxFirst->GetRotation() == 0 && boxSecond->GetRotation() == 0)
	{
		glm::vec2 extentsFirst = boxFirst->GetExtents(); //halve the extents
		glm::vec2 extentsSecond = boxSecond->GetExtents(); //halve the extents

														   //extentsFirst += glm::vec2(2.0f, 2.0f);
														   //extentsSecond += glm::vec2(2.0f, 2.0f);

		glm::vec2 boxFirstTopLeft = boxFirst->GetPosition() + glm::vec2(-extentsFirst.x, extentsFirst.y);
		glm::vec2 boxFirstBottomRight = boxFirst->GetPosition() + glm::vec2(extentsFirst.x, -extentsFirst.y);
		glm::vec2 boxSecondTopLeft = boxSecond->GetPosition() + glm::vec2(-extentsSecond.x, extentsSecond.y);
		glm::vec2 boxSecondBottomRight = boxSecond->GetPosition() + glm::vec2(extentsSecond.x, -extentsSecond.y);
		glm::vec2 boxFirstTopRight = boxFirst->GetPosition() + extentsFirst;
		glm::vec2 boxSecondBottomLeft = boxSecond->GetPosition() - extentsSecond;
		glm::vec2 boxSecondTopRight = boxSecond->GetPosition() + extentsSecond;
		glm::vec2 boxFirstBottomLeft = boxFirst->GetPosition() - extentsFirst;

		//aie::Gizmos::add2DCircle(boxFirstBottomLeft, 1, 8, glm::vec4(1, 0, 0, 1));
		//aie::Gizmos::add2DCircle(boxFirstTopRight, 1, 8, glm::vec4(1, 0, 0, 1));
		//aie::Gizmos::add2DCircle(boxFirstTopLeft, 1, 8, glm::vec4(1, 0, 0, 1));
		//aie::Gizmos::add2DCircle(boxFirstBottomRight, 1, 8, glm::vec4(1, 0, 0, 1));

		if (boxFirstBottomLeft.x <= boxSecondBottomRight.x && boxFirstBottomRight.x >= boxSecondBottomLeft.x)
		{
			if (boxFirstBottomLeft.y <= boxSecondTopLeft.y && boxFirstTopLeft.y >= boxSecondBottomRight.y)
			{
				glm::vec2 delta = boxSecond->GetPosition() - boxFirst->GetPosition();
				glm::vec2 collisionNormal = glm::normalize(delta);
				glm::vec2 relativeVelocity = boxFirst->GetVelocity() - boxSecond->GetVelocity();
				glm::vec2 collisionVector = collisionNormal * glm::dot(collisionNormal, relativeVelocity);
				glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / boxFirst->GetMass() + 1.0f / boxSecond->GetMass());
				boxFirst->ApplyForceToActor(boxSecond, forceVector*2);
				//float intersection = abs(glm::dot(boxFirst->GetExtents(), collisionNormal)) + abs(glm::dot(boxSecond->GetExtents(), collisionNormal)) - glm::length(delta);
				glm::vec2 firstExtents = boxFirst->GetExtents();
				glm::vec2 secondExtents = boxSecond->GetExtents();

				//move extents vector so that it is in the general direction of the collision normal
				if ((firstExtents.x < 0 && collisionNormal.x > 0) || (firstExtents.x > 0 && collisionNormal.x < 0))
				{
					firstExtents.x *= -1;
				}
				
				if ((firstExtents.y < 0 && collisionNormal.y > 0) || (firstExtents.y > 0 && collisionNormal.y < 0))
				{
					firstExtents.y *= -1;
				}
				
				if ((secondExtents.x < 0 && collisionNormal.x > 0) || (secondExtents.x > 0 && collisionNormal.x < 0))
				{
					secondExtents.x *= -1;
				}
				
				if ((secondExtents.y < 0 && collisionNormal.y > 0) || (secondExtents.y > 0 && collisionNormal.y < 0))
				{
					secondExtents.y *= -1;
				}
				
				//pseudo-radius
				float dotFirst = abs(glm::dot((firstExtents), collisionNormal));
				float dotSecond = abs(glm::dot((secondExtents), collisionNormal));
				
				//float intersection = glm::length(firstExtentsProjected) + glm::length(secondExtentsProjected) - glm::length(delta);
				float intersection = dotFirst + dotSecond - glm::length(delta);
				glm::vec2 separationVector = collisionNormal * intersection * 0.5f;
				
				//std::cout << separationVector.x << ", " << separationVector.y << std::endl;
				
				boxFirst->SetPosition(boxFirst->GetPosition() - separationVector);
				boxSecond->SetPosition(boxSecond->GetPosition() + separationVector);
				return true;
			}
		}
		return false;
	}

	else
	{

		boxFirst->CalculateCorners();
		boxFirst->CalculateEdges();

		boxSecond->CalculateCorners();
		boxSecond->CalculateEdges();

		glm::vec2 *firstAxes = boxFirst->GetEdges(); //axes to test are the normals of each edge on the shapes
		glm::vec2 *secondAxes = boxSecond->GetEdges();
		glm::vec2 *firstVertices = boxFirst->GetCorners();
		glm::vec2 *secondVertices = boxSecond->GetCorners();

		float rotationFirst = boxFirst->GetRotation();
		float rotationSecond = boxSecond->GetRotation();

		glm::vec2 firstExtents = boxFirst->GetExtents();
		glm::vec2 secondExtents = boxSecond->GetExtents();

		float scaleFirst = sqrt(pow(boxFirst->GetExtents().x, 2) + pow(boxFirst->GetExtents().y, 2));
		float scaleSecond = sqrt(pow(boxSecond->GetExtents().x, 2) + pow(boxSecond->GetExtents().y, 2));

		float rotFirst = rotationFirst;
		float rotSecond = rotationSecond;
		float vertexRotFirst = rotationFirst + (3.1415f / 4.0f);
		float vertexRotSecond = rotationSecond + (3.1415f / 4.0f);
		/*
		for (int i = 0; i < 4; i++)
		{
		aie::Gizmos::add2DLine(boxFirst->GetPosition(), firstAxes[i], glm::vec4(0, 1, 0, 1));
		}

		for (int i = 0; i < 4; i++)
		{
		aie::Gizmos::add2DLine(boxSecond->GetPosition(), secondAxes[i], glm::vec4(0, 1, 0, 1));
		}*/

		//THIS WORKS
		//for (int i = 0; i < 4; i++)
		//{
		//	glm::vec2 someEdge = boxFirst->GetPosition() + scaleFirst * glm::vec2(cos(-rotFirst), sin(-rotFirst));
		//	aie::Gizmos::add2DLine(boxFirst->GetPosition(), someEdge, glm::vec4(0, 1, 0, 1));
		//	someEdge = boxFirst->GetPosition() - someEdge;
		//	firstAxes[i] = someEdge;
		//	glm::vec2 someVertex = boxFirst->GetPosition() + scaleFirst * glm::vec2(cos(-vertexRotFirst), sin(-vertexRotFirst));

		//	firstVertices[i] = someVertex;
		//	someEdge = boxSecond->GetPosition() + scaleSecond * glm::vec2(cos(-rotSecond), sin(-rotSecond));
		//	aie::Gizmos::add2DLine(boxSecond->GetPosition(), someEdge, glm::vec4(0, 1, 0, 1));
		//	someEdge = boxSecond->GetPosition() - someEdge; //because some edge wasn't perpendicular to the surface. Right spot but wrong direction.

		//	someVertex = boxSecond->GetPosition() + scaleSecond * glm::vec2(cos(-vertexRotSecond), sin(-vertexRotSecond));
		//	secondVertices[i] = someVertex;
		//	secondAxes[i] = someEdge;

		//	rotFirst += 3.1415f / 2.0f; //add 90 degrees
		//	rotSecond += 3.1415f / 2.0f;
		//	vertexRotFirst += 3.1415f / 2.0f;
		//	vertexRotSecond += 3.1415f / 2.0f;
		//}
		for (int i = 0; i < 4; i++)
		{
			glm::vec2 axis = firstAxes[i];
			axis = axis - boxFirst->GetPosition();
			std::pair<float, float> projection1 = GetProjection(axis, firstVertices, 4);
			std::pair<float, float> projection2 = GetProjection(axis, secondVertices, 4);

			//check for no overlap
			if (projection1.second < projection2.first || projection2.second < projection1.first)
			{
				//std::cout << "No overlap" << std::endl;

				return false;
			}

			axis = secondAxes[i];
			axis = axis - boxSecond->GetPosition();
			projection1 = GetProjection(axis, firstVertices, 4);
			projection2 = GetProjection(axis, secondVertices, 4);

			//check for no overlap
			if (projection1.second < projection2.first || projection2.second < projection1.first)
			{
				//std::cout << "No overlap" << std::endl;

				return false;
			}
		}

		glm::vec2 delta = boxSecond->GetPosition() - boxFirst->GetPosition();
		//std::cout << "Length: " << glm::length(delta) << std::endl;
		
		if (isnan(delta.y))
		{
			__debugbreak();
		}

		glm::vec2 collisionNormal = glm::normalize(delta);
		glm::vec2 relativeVelocity = boxFirst->GetVelocity() - boxSecond->GetVelocity();
		glm::vec2 collisionVector = collisionNormal * (glm::dot(relativeVelocity, collisionNormal));
		glm::vec2 forceVector = collisionVector * 1.0f / (1.0f / boxFirst->GetMass() + 1.0f / boxSecond->GetMass());
		//std::cout << "Applying force from BoxToBox(): " << glm::length(forceVector*2) << std::endl;


		boxFirst->ApplyForceToActor(boxSecond, forceVector * 2);


		delta = boxSecond->GetPosition() - boxFirst->GetPosition();
		collisionNormal = glm::normalize(delta);
		//float intersection = abs(glm::dot(boxFirst->GetExtents(), collisionNormal)) + abs(glm::dot(boxSecond->GetExtents(), collisionNormal)) - glm::length(delta);
		firstExtents = boxFirst->GetExtents();
		secondExtents = boxSecond->GetExtents();

		glm::mat2 rotateZFirst(cos(-rotationFirst), -sin(-rotationFirst), sin(-rotationFirst), cos(-rotationFirst));
		glm::mat2 rotateZSecond(cos(-rotationSecond), -sin(-rotationSecond), sin(-rotationSecond), cos(-rotationSecond));

		//firstExtents = firstExtents * rotateZFirst;
		//secondExtents = secondExtents * rotateZSecond;

		int highestDot = -INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			float currentDot = glm::dot(firstVertices[i], collisionNormal);
			if (currentDot > highestDot)
			{
				highestDot = currentDot;
				firstExtents = firstVertices[i];
			}
		}
		highestDot = -INT_MAX;
		for (int i = 0; i < 4; i++)
		{
			float currentDot = glm::dot(secondVertices[i], collisionNormal);
			if (currentDot > highestDot)
			{
				highestDot = currentDot;
				secondExtents = secondVertices[i];
			}
		}

		firstExtents -= boxFirst->GetPosition();
		secondExtents -= boxSecond->GetPosition();
		//firstExtents = firstVertices[0] - boxFirst->GetPosition();
		//secondExtents = secondVertices[0] - boxSecond->GetPosition();

		//move extents vector so that it is in the general direction of the collision normal
	/*	if ((firstExtents.x < 0 && collisionNormal.x > 0) || (firstExtents.x > 0 && collisionNormal.x < 0))
		{
			firstExtents.x *= -1;
		}

		if ((firstExtents.y < 0 && collisionNormal.y > 0) || (firstExtents.y > 0 && collisionNormal.y < 0))
		{
			firstExtents.y *= -1;
		}

		if ((secondExtents.x < 0 && collisionNormal.x > 0) || (secondExtents.x > 0 && collisionNormal.x < 0))
		{
			secondExtents.x *= -1;
		}

		if ((secondExtents.y < 0 && collisionNormal.y > 0) || (secondExtents.y > 0 && collisionNormal.y < 0))
		{
			secondExtents.y *= -1;
		}*/
	
		//pseudo-radius
		float dotFirst = (glm::dot((firstExtents), collisionNormal));
		float dotSecond = (glm::dot((secondExtents), collisionNormal));

		//apply torque before separation
		glm::vec2 torqueLever = glm::normalize(glm::vec2(delta.y, -delta.x)) * dotFirst;
		//float dot = glm::dot(glm::normalize(forceVector), glm::normalize(torqueLever));
		//float angle = acos(dot);
		//float magnitude = sin(angle) * glm::length(forceVector); //trig to find opposite side, or the perpendicular component
		//magnitude *= dotFirst; //multiply by distance to get final magnitude
		float torque = glm::dot(torqueLever, relativeVelocity) * 1.0f / (1 / boxFirst->GetMass() + 1 / boxSecond->GetMass());
		torque *= 0.1f;
		boxFirst->ApplyTorque(-torque);
		boxSecond->ApplyTorque(torque);

		/*if (dotFirst < 0.001f)
		{
			firstExtents = glm::vec2(firstExtents.y, firstExtents.x);
			dotFirst = abs(glm::dot((firstExtents), collisionNormal));
		}

		if (dotSecond < 0.001f)
		{
			secondExtents = glm::vec2(secondExtents.y, secondExtents.x);
			dotSecond = abs(glm::dot((secondExtents), collisionNormal));
		}
*/
		//float intersection = glm::length(firstExtentsProjected) + glm::length(secondExtentsProjected) - glm::length(delta);
		float intersection = dotFirst + dotSecond - glm::length(delta);
		
		std::cout << "Intersection: " << intersection << std::endl;
		glm::vec2 separationVector = collisionNormal * intersection * 0.5f;

		//std::cout << separationVector.x << ", " << separationVector.y << std::endl;

		boxFirst->SetPosition(boxFirst->GetPosition() - separationVector);
		boxSecond->SetPosition(boxSecond->GetPosition() + separationVector);


		return true;

	}
}
}

bool PhysicsScene::BoxToSphere(PhysicsObject * first, PhysicsObject * second)
{
	return SphereToBox(second, first);
}

bool PhysicsScene::BoxToPlane(PhysicsObject * first, PhysicsObject * second)
{
	return PlaneToBox(second, first);
}

bool PhysicsScene::PlaneToTriangle(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::SphereToTriangle(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::BoxToTriangle(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::TriangleToPlane(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::TriangleToSphere(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::TriangleToBox(PhysicsObject * first, PhysicsObject * second)
{
	return false;
}

bool PhysicsScene::TriangleToTriangle(PhysicsObject * first, PhysicsObject * second)
{
	Triangle* firstTri = dynamic_cast<Triangle*>(first);
	Triangle* secondTri = dynamic_cast<Triangle*>(second);
	if (firstTri != nullptr && secondTri != nullptr)
	{
		//get corners and axes of each
		glm::vec2 *firstCorners = firstTri->GetCorners();
		glm::vec2 *secondCorners = secondTri->GetCorners();

		glm::vec2 *firstAxes = firstTri->GetAxes();
		glm::vec2 *secondAxes = secondTri->GetAxes();

		for (int i = 0; i < 3; i++)
		{
			glm::vec2 axis = firstAxes[i];
			std::pair<float, float> projection1 = GetProjection(axis, firstCorners, 3);
			std::pair<float, float> projection2 = GetProjection(axis, secondCorners, 3);

			//check for no overlap
			if (projection1.second < projection2.first || projection2.second < projection1.first)
			{
				delete firstCorners, delete secondCorners;
				return false;
			}
			
			axis = secondAxes[i];
			projection1 = GetProjection(axis, firstCorners, 3);
			projection2 = GetProjection(axis, secondCorners, 3);

			//check for no overlap
			if (projection1.second < projection2.first || projection2.second < projection1.first)
			{
				delete firstCorners, delete secondCorners;
				return false;
			}

		}
		delete firstCorners, delete secondCorners;
		firstTri->SetVelocityToZero();
		secondTri->SetVelocityToZero();
		return true;
	}

	return false;
}
