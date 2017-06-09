#define MIN_LINEAR_THRESHOLD 0.15f
#define MIN_ROTATION_THRESHOLD 0.01f

#pragma once
#include "PhysicsObject.h"


class Rigidbody : public PhysicsObject
{
public:
	Rigidbody();
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug();
	
	void ApplyForce(glm::vec2 force);
	void ApplyTorque(float torque);
	void ApplyForceToActor(Rigidbody *otherActor, glm::vec2 force);

	bool IsStatic();

	glm::vec2 GetPosition();
	glm::vec2 GetVelocity();

	void SetPosition(glm::vec2 position);

	void SetVelocityToZero();

	float GetMass();
	float GetRotation();

	float GetElasticity();

	void AddRotation(float amount);

	void SetRotation(float rot);

	//Remove this method later when no longer needed
	void ReduceMass(float amount);

	glm::vec2 CalculateProjectilePosition(float theta, float speed, float gravity, float time);

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;

	bool m_isStatic;

	float m_angularVelocity;
	float m_angularDrag;
	float m_linearDrag;

	float m_elasticity;

	float m_mass;
	float m_rotation;
};

