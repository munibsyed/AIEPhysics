#include "FlyCamera.h"




FlyCamera::FlyCamera(GLFWwindow *window, float speed, float rotateSpeed)
{
	this->speed = speed;
	this->rotateSpeed = rotateSpeed;
	
	m_oldMousePos = ImGui::GetMousePos();
}


FlyCamera::~FlyCamera()
{
}



void FlyCamera::Update(GLFWwindow *window, float deltaTime)
{
	mat4 currentTransform = GetWorldTransform();
	currentTransform[3] = vec4(0, 0, 0, 1);

	vec3 currentPos(worldTransform[3][0], worldTransform[3][1], worldTransform[3][2]);
	vec3 forward(worldTransform[2][0], worldTransform[2][1], worldTransform[2][2]);
	vec3 right = glm::cross(vec3(0, 1, 0), forward);
	//vec3 right(worldTransform[0][0], worldTransform[0][1], worldTransform[0][2]);
		//get global right of camera 
	
	vec3 up(worldTransform[1][0], worldTransform[1][1], worldTransform[1][2]);
	aie::Input *input = aie::Input::getInstance();

	//movement with arrow keys
	if (input->isKeyDown(aie::INPUT_KEY_W))
	{
		currentPos -= forward * speed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_S))
	{
		currentPos += forward * speed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_Q))
	{
		currentPos += up * speed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_E))
	{
		currentPos -= up * speed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_A))
	{
		currentPos -= right * speed * deltaTime;
	}

	if (input->isKeyDown(aie::INPUT_KEY_D))
	{
		currentPos += right * speed * deltaTime;
	}

	double cursorX;
	double cursorY;

	ImVec2 m_newMousePos = ImGui::GetMousePos();
	
	double deltaX = m_oldMousePos.x - m_newMousePos.x;
	double deltaY = m_oldMousePos.y - m_newMousePos.y;

	deltaX = glm::clamp((float)deltaX, -1.0f, 1.0f);
	deltaY = glm::clamp((float)deltaY, -1.0f, 1.0f);
	
	m_oldMousePos = m_newMousePos;

	float angleX = (float)deltaX * rotateSpeed * deltaTime;
	float angleY = (float)deltaY * rotateSpeed * deltaTime;


	mat4 rotation = glm::rotate(angleX, vec3(0, 1, 0)); //rotate on y-axis
	rotation *= glm::rotate(angleY, vec3(worldTransform[0])); //rotate on x-axis

	worldTransform = rotation * currentTransform;

	SetPosition(currentPos);
		
}



void FlyCamera::SetSpeed(float speed)
{
	this->speed = speed;
}
