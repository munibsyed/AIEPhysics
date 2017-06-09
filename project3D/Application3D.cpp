#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(100000, 100000, 100000, 100000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);
	
	m_physicsScene = new PhysicsScene3D(glm::vec3(0,0,0), 0.01f);

	m_camera = new FlyCamera(m_window, 10, 1);
	m_camera->SetLookAt(vec3(0,0,-10), vec3(0), vec3(0, 1, 0));
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
	
	m_isLeftClickDown = false;
	
	Plane3D* bottomPlane = new Plane3D(glm::vec3(0, 1, 0), glm::vec3(0, -10, 0), glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(bottomPlane);

	//Plane3D* topPlane = new Plane3D(glm::vec3(0, -1, 0), glm::vec3(0, 10, 0), glm::vec4(0, 1, 0, 1));
	//m_physicsScene->AddActor(topPlane);

	Plane3D* rightPlane = new Plane3D(glm::vec3(-1, 0, 0), glm::vec3(10, 0, 0), glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(rightPlane);

	Plane3D* leftPlane = new Plane3D(glm::vec3(1, 0, 0), glm::vec3(-10, 0, 0), glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(leftPlane);

	Plane3D* frontPlane = new Plane3D(glm::vec3(0, 0, -1), glm::vec3(0, 0, 10), glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(frontPlane);

	Plane3D* backPlane = new Plane3D(glm::vec3(0, 0, 1), glm::vec3(0, 0, -10), glm::vec4(0, 1, 0, 1));
	m_physicsScene->AddActor(backPlane);

	m_sphereFirst = new Sphere3D(glm::vec3(0, -9, -3), glm::vec3(0), 1.0f, 1.0f, 0.99f, 0.8f, false, glm::vec4(1, 0, 0, 1));
	m_sphereSecond = new Sphere3D(glm::vec3(0, -9, 3), glm::vec3(0), 1, 1, 0.99f, 0.8f, false, glm::vec4(1, 0, 0, 1));

	m_physicsScene->AddActor(m_sphereFirst);
	m_physicsScene->AddActor(m_sphereSecond);

	m_speed = 0.1f;

	int n = 1;
	float scale = 1.0f;
	for (int i = 0; i < n; i++)
	{
		for (int ii = 0; ii < n; ii++)
		{
			for (int iii = 0; iii < n; iii++)
			{
				vec3 pos = vec3(i*(scale * 2), ii*(scale * 2), iii*(scale * 2));
				box = new Box3D(pos, vec3(0), vec3(scale-0.01f), 1, 1.0f, 0, true, false, vec4(1, 0, 1, 1));
				m_physicsScene->AddActor(box);
			}
		}
	}
	

	/*Box3D* box = new Box3D(vec3(0, -9.5, 0), vec3(0), vec3(1, 1, 1), 1, 1, 0, true, false, vec4(1, 0, 1, 1));
	m_physicsScene->AddActor(box);

*/
	box->CalculateCorners();

	m_controlledRbFirst = box;
	m_controlledRbSecond = box;

	return true;
}

void Application3D::shutdown() {

	delete m_physicsScene;
	Gizmos::destroy();
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	z -= deltaTime * 2;
	m_viewMatrix = glm::lookAt(vec3(10, 10, 10),
							   vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();
	
	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_physicsScene->Update(deltaTime);
	m_physicsScene->CheckForCollisions();
	m_camera->Update(m_window, deltaTime);
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_PAGE_UP))
	{
		box->AddRotationX(deltaTime);
	}

	if (input->isKeyDown(aie::INPUT_KEY_PAGE_DOWN))
	{
		box->AddRotationX(-deltaTime);
	}

	box->CalculateCorners();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		m_controlledRbFirst->ApplyForce(vec3(0, 0, -m_speed));
	}

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
	{
		m_controlledRbFirst->ApplyForce(vec3(0, 0, m_speed));
	}

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		m_controlledRbFirst->ApplyForce(vec3(m_speed,0,0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		m_controlledRbFirst->ApplyForce(vec3(-m_speed, 0, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_I))
	{
		m_controlledRbFirst->ApplyForce(vec3(0, m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_P))
	{
		m_controlledRbFirst->ApplyForce(vec3(0, -m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_J))
	{
		m_controlledRbSecond->ApplyForce(vec3(0, m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_L))
	{
		m_controlledRbSecond->ApplyForce(vec3(0, -m_speed, 0));
	}

	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		m_physicsScene->SetGravity(vec3(0, -9.81f, 0));
	}

	if (input->isMouseButtonDown(0) && m_isLeftClickDown == false)
	{
		m_isLeftClickDown = true;
		float x = input->getMouseX() / (getWindowWidth()  * 0.5f) - 1.0f;
		float y = input->getMouseY() / (getWindowHeight() * 0.5f) - 1.0f; //y gets bigger from bottom to top

		glm::mat4 cameraTransform = m_camera->GetWorldTransform();
		glm::vec3 cameraPos(cameraTransform[3][0], cameraTransform[3][1], cameraTransform[3][2]);
		
		glm::mat4 invPV = glm::inverse(m_camera->GetProjectionView());
		glm::vec4 screenPos = glm::vec4(x, y, 1.0f, 1.0f);
		glm::vec4 worldPos = invPV * screenPos;
		
		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

		
		glm::vec3 start = cameraPos;
		Ray3D ray(start, dir, 50, vec4(0, 1, 0, 1));
		Rigidbody3D* closestHit = m_physicsScene->RaycastToClosestRigidbody(ray);
		if (closestHit != nullptr)
		{
			glm::vec3 forceVector = dir * 10;
			closestHit->ApplyForce(forceVector);
		}

	//	glm::vec3 direction = glm::normalize(vec3(worldSpacePoint.x, worldSpacePoint.y, worldSpacePoint.z) - vec3(m_camera->GetWorldTransform()[3][0], m_camera->GetWorldTransform()[3][1], m_camera->GetWorldTransform()[3][2]));

	}

	if (input->isMouseButtonDown(0) == false)
		m_isLeftClickDown = false;

	
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();
	m_physicsScene->Draw();
	
	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	Gizmos::draw(m_camera->GetProjectionView());
}