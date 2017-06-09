#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "PhysicsScene3D.h"
#include "Plane3D.h"
#include "FlyCamera.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	Sphere3D* m_sphereFirst, *m_sphereSecond;
	Box3D* box;

	Rigidbody3D* m_controlledRbFirst;
	Rigidbody3D* m_controlledRbSecond;

	bool m_isLeftClickDown;

	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	float m_speed;
	float z = 10;

	PhysicsScene3D* m_physicsScene;
	FlyCamera* m_camera;

};