#include "PhysicsApplication2D.h"


PhysicsApplication2D::PhysicsApplication2D()
{
}


PhysicsApplication2D::~PhysicsApplication2D()
{
}

bool PhysicsApplication2D::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_gravity = new float;
	*m_gravity = 0;
	m_timeRunning = 0;
	m_font = new aie::Font("font/consolas.tff", 32);
	m_2dRenderer = new aie::Renderer2D;
	m_leftClickDown = false;

	m_rot = 0;
	m_speed = 0.5f;
	//initialize the physics scene
	m_physicsScene = new PhysicsScene;
	m_physicsScene->SetGravity(glm::vec2(0, *m_gravity));
	m_physicsScene->SetTimeStep(0.00001f);

	//m_projectile = new Sphere(glm::vec2(0, 0), glm::vec2(cos(3.14f / 4.0f), sin(3.14f / 4.0f))*15.0f, 1, 1.0f, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->AddActor(m_projectile);
	
	//m_physicsScene->AddActor(m_plane);

	m_walls[0] = new Plane(glm::vec2(1, 0), 99);
	m_physicsScene->AddActor(m_walls[0]);
	m_walls[1] = new Plane(glm::vec2(-1, 0), 99);
	m_physicsScene->AddActor(m_walls[1]);
	m_walls[2] = new Plane(glm::vec2(0, -1), 56.25f);
	m_physicsScene->AddActor(m_walls[2]);
	m_walls[3] = new Plane(glm::vec2(0, 1), 56.25f);
	m_physicsScene->AddActor(m_walls[3]);

	//m_boxFirst = new Box(glm::vec2(0, -10), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1));
	//m_boxSecond = new Box(glm::vec2(20, -25), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1));
	
	//m_physicsScene->AddActor(m_boxFirst);
	//m_physicsScene->AddActor(m_boxSecond);

	int startY = 50;
	float mass = 1;
	float radius = 2;

	//m_sphereFirst = new Sphere(glm::vec2(-50, 50), glm::vec2(0, 0), mass, radius, 0.1f, glm::vec4(1, 0, 0, 1), true);
	//m_physicsScene->AddActor(m_sphereFirst);

	//spring joint code
	/*int numBalls = 5;
	for (int i = 1; i <= numBalls; i++)
	{
		m_sphereSecond = new Sphere(glm::vec2(-50, startY - i*6.0f), glm::vec2(0, 0), mass, radius, 0.1f, glm::vec4(1, 0, 0, 1), false);
		m_physicsScene->AddActor(m_sphereSecond);

		m_physicsScene->AddActor(new SpringJoint(m_sphereFirst, m_sphereSecond, 0.0005f, 0.0005f));

		m_sphereFirst = m_sphereSecond;

	}*/

	/*int numBoxes = 10;
	for (int i = 0; i < numBoxes; i++)
	{
		m_boxFirst = new Box(glm::vec2(i*2, 5.5f+(i*2)), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1));
		m_physicsScene->AddActor(m_boxFirst);

	}*/

	m_boxFirst = new Box(glm::vec2(0, 8.5f), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1));
	m_boxSecond = new Box(glm::vec2(-5, 0), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(m_boxFirst);
	m_physicsScene->AddActor(m_boxSecond);

	//which actors to control via keyboard
	m_controlledRb1 = m_boxFirst;
	m_controlledRb2 = m_boxSecond;

	return true;
}

void PhysicsApplication2D::shutdown()
{
	delete m_physicsScene;
	delete m_font;
	delete m_2dRenderer;
	delete m_gravity;
	aie::Gizmos::destroy();

}

void PhysicsApplication2D::update(float deltaTime)
{
	ImGui::Begin("2D Physics Scene");

	ImGui::InputFloat("Gravity", m_gravity);

	ImGui::End();



	m_physicsScene->SetGravity(glm::vec2(0, -*m_gravity));

	aie::Input* input = aie::Input::getInstance();
	aie::Gizmos::clear();
	m_timeRunning += deltaTime;

	//m_plane->SetNormal(glm::vec2(cos(m_rot), sin(m_rot)));

	//update physics
	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();

	m_physicsScene->CheckForCollision();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_controlledRb1->ApplyForce(glm::vec2(-m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		m_controlledRb1->ApplyForce(glm::vec2(m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		m_controlledRb1->ApplyForce(glm::vec2(0, -m_speed));
	}

	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		m_controlledRb1->ApplyForce(glm::vec2(0, m_speed));
	}

	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		m_controlledRb2->ApplyForce(glm::vec2(-m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		m_controlledRb2->ApplyForce(glm::vec2(m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		m_controlledRb2->ApplyForce(glm::vec2(0, -m_speed));
	}

	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		m_controlledRb2->ApplyForce(glm::vec2(0, m_speed));
	}
	
	if (input->isKeyDown(aie::INPUT_KEY_E))
	{
		m_rot += 0.01f;
		m_controlledRb1->SetRotation(m_rot);
	}

	if (input->isKeyDown(aie::INPUT_KEY_Q))
	{
		m_rot -= 0.01f;
		m_controlledRb1->SetRotation(m_rot);
	}

	if (input->isMouseButtonDown(0) && m_leftClickDown == false)
	{
		int x;
		int y;
		input->getMouseXY(&x, &y);
		x -= 640;
		y -= 360;

		x /= 12.8f;
		y /= 12.8f;

		x *= 2;
		y *= 2;

		int randMassMin = 1;
		int randMassMax = 10;

		int randMass = randMassMin + (rand() % static_cast<int>(randMassMax - randMassMin + 1));

		int randRadiusMin = 1;
		int randRadiusMax = 5;

		int randRadius = randRadiusMin + (rand() % static_cast<int>(randRadiusMax - randMassMin + 1));

		float randR, randG, randB;
		randR = ((double)rand() / (RAND_MAX));
		randG = ((double)rand() / (RAND_MAX));
		randB = ((double)rand() / (RAND_MAX));

		m_physicsScene->AddActor(new Box(glm::vec2(x, y), glm::vec2(2.5f, 2.5f), 1, glm::vec4(1, 1, 1, 1)));
		//Sphere* sphere = new Sphere(glm::vec2(x, y), glm::vec2(0, 0), randMass, randRadius, 1, glm::vec4(randR, randG, randB, 1), false);
		//m_physicsScene->AddActor(sphere);
		m_leftClickDown = true;
	}

	if (input->isMouseButtonUp(0))
	{
		m_leftClickDown = false;
	}

	//m_calculatedProjectilePos = m_projectile->CalculateProjectilePosition(3.14f / 4.0f, 15, 9.8f, m_timeRunning);
	
}

void PhysicsApplication2D::draw()
{
	clearScreen();
	// set the camera position before we begin rendering
	//m_2dRenderer->setCameraPos(0, 0);

	// begin drawing sprites
	m_2dRenderer->begin();

	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
	
	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);

	//aie::Gizmos::add2DCircle(m_calculatedProjectilePos, m_projectile->GetRadius(), 8, glm::vec4(0,0,1,1));
	m_2dRenderer->end();
}
