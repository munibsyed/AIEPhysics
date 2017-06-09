#pragma once
#define MIN_ROTATION_THRESHOLD 0.001f
#define MIN_LINEAR_THRESHOLD 0.01f
#include "PhysicsObject3D.h"

class Rigidbody3D :
	public PhysicsObject3D
{
public:
	Rigidbody3D();
	~Rigidbody3D();

	void ApplyForce(glm::vec3 force);

	void ApplyForceToActor(Rigidbody3D *rb, glm::vec3 force);

	void ApplyTorque(glm::vec3 torque);

	void SetPosition(glm::vec3 position);

	glm::vec3 GetPosition();

	glm::vec3 GetVelocity();

	float GetMass();

	void SetVelocityToZero();

	void FixedUpdate(glm::vec3 gravity, float timeStep);


protected:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_angularVelocity;

	glm::vec3 m_rotation;

	glm::mat4 m_rotationMatrix;

	float m_linearDrag;
	float m_angularDrag;
	float m_mass;

	bool m_isStatic;
};

