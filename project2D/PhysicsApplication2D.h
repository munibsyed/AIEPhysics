#pragma once
#include "Application.h"
#include <iostream>
#include <Renderer2D.h>
#include <Font.h>
#include <Input.h>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "PhysicsScene.h"
#include "SpringJoint.h"
#include <random>
#include  <imgui_glfw3.h>

class PhysicsApplication2D : public aie::Application
{
public:
	PhysicsApplication2D();
	virtual ~PhysicsApplication2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

private:
	PhysicsScene*		m_physicsScene;
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	glm::vec2			m_cameraPos;

	float				m_timeRunning;

	float* m_gravity;

	Sphere* m_sphereFirst;
	Sphere* m_sphereSecond;

	Triangle* tri;
	Triangle* tri2;

	Rigidbody* m_controlledRb1;
	Rigidbody* m_controlledRb2;

	float m_rot;

	SpringJoint* m_springJoint;

	Box*	m_boxFirst;
	Box*	m_boxSecond;

	Plane*	m_walls[4];

	bool m_leftClickDown;

	float m_speed;

};

