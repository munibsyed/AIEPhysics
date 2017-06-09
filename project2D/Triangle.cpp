#include "Triangle.h"



Triangle::Triangle(glm::vec2 position, glm::vec2 velocity, glm::vec2 firstCorner, glm::vec2 secondCorner, glm::vec2 thirdCorner, float mass, glm::vec4 colour)
{
	m_position = position;
	m_velocity = velocity;
	m_corners[0] = firstCorner;
	m_corners[1] = secondCorner;
	m_corners[2] = thirdCorner;
	m_mass = mass;
	m_colour = colour;
	m_shapeID = TRIANGLE;
	m_linearDrag = 1;
}

Triangle::~Triangle()
{
}

void Triangle::MakeGizmo()
{	
	aie::Gizmos::add2DLine(m_position + m_corners[0], m_position + m_corners[1], m_colour);
	aie::Gizmos::add2DLine(m_position + m_corners[1], m_position + m_corners[2], m_colour);
	aie::Gizmos::add2DLine(m_position + m_corners[2], m_position + m_corners[0], m_colour);
}

glm::vec2 * Triangle::GetCorners()
{
	return new glm::vec2[3]{m_position + m_corners[0], m_position + m_corners[1], m_position + m_corners[2]};
}

glm::vec2 * Triangle::GetAxes()
{
	glm::vec2 axes[3];
	glm::vec2 *corners = GetCorners();
	float pi = 3.141592f;
	glm::mat2 rotationZ(cos(pi / 2.0f), -sin(pi / 2.0f), sin(pi / 2.0f), cos(pi / 2.0f));

	axes[0] = (corners[0] + corners[1]) / 2.0f; 
	axes[1] = (corners[1] + corners[2]) / 2.0f; 
	axes[2] = (corners[2] + corners[0]) / 2.0f;
	
	axes[0]	= glm::normalize(axes[0] - m_position) * glm::length(axes[0] - m_position);
	axes[1] = glm::normalize(axes[1] - m_position) * glm::length(axes[1] - m_position);
	axes[2] = glm::normalize(axes[2] - m_position) * glm::length(axes[2] - m_position);

	//aie::Gizmos::add2DCircle(corners[0], 1, 8, glm::vec4(1, 0, 0, 1));
	//aie::Gizmos::add2DCircle(corners[1], 1, 8, glm::vec4(1, 0, 0, 1));
	//aie::Gizmos::add2DCircle(corners[2], 1, 8, glm::vec4(1, 0, 0, 1));

	aie::Gizmos::add2DLine(axes[0], axes[0] * 2, glm::vec4(1, 1, 1, 1));
	aie::Gizmos::add2DLine(axes[1], axes[1] * 2, glm::vec4(1, 1, 1, 1));
	aie::Gizmos::add2DLine(axes[2], axes[2] * 2, glm::vec4(1, 1, 1, 1));

	delete corners;

	return axes;
}
