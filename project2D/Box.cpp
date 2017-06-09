#include "Box.h"


Box::Box(glm::vec2 position, glm::vec2 extents, float mass, glm::vec4 colour)
{
	m_extents = extents;
	m_position = position;
	m_colour = colour;
	m_mass = mass;
	m_rotation = 0;
	m_rotation = 3.141592f / 4.0f;
	m_rotation = rand() % 3 - 3;

	m_shapeID = BOX;

	m_angularVelocity = 0;
	m_angularDrag = 4.0f;
	m_linearDrag = 0.99f;

	m_elasticity = 1;

	CalculateCorners();
	CalculateEdges();
}

Box::~Box()
{
	delete m_edges;
	delete m_corners;
}


void Box::Debug()
{
}

void Box::MakeGizmo()
{
	const glm::mat4* transform = new glm::mat4(cos(m_rotation), -sin(m_rotation), 0, m_position.x, sin(m_rotation), cos(m_rotation), 0, m_position.y, 0, 0, 1, 0, 0, 0, 0, 1);
	//std::cout << "MakeGizmo() ID: " << m_objectID << std::endl;
	//24

	aie::Gizmos::add2DAABB(m_position, m_extents, m_colour, transform);
}

glm::vec2 Box::GetExtents()
{
	return m_extents;
}

glm::vec2 * Box::GetCorners()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	aie::Gizmos::add2DCircle(m_corners[i], 1, 8, glm::vec4(1, 0, 0, 1));
	//}
	return m_corners;
}

glm::vec2 * Box::GetEdges()
{	
	return m_edges;
}

void Box::CalculateCorners()
{
	glm::vec2 *corners = new glm::vec2[4]{ glm::vec2(m_extents.x, m_extents.y), glm::vec2(-m_extents.x, m_extents.y),  glm::vec2(-m_extents.x, -m_extents.y), glm::vec2(m_extents.x, -m_extents.y) };
	glm::mat2 rotateZ(cos(-m_rotation), -sin(-m_rotation), sin(-m_rotation), cos(-m_rotation));
	for (int i = 0; i < 4; i++)
	{
		glm::vec2 rotatedCorner = corners[i] * rotateZ;
		rotatedCorner += m_position;
		corners[i] = rotatedCorner;
		//aie::Gizmos::add2DCircle(rotatedCorner, 1, 8, glm::vec4(1, 0, 0, 1));
	}

	//CHANGE THIS TO STORE CORNERS AND EDGES IN MEMBER VARIABLES, AND ONLY UPDATE THEM WHEN ROTATED

	m_corners = corners;
}

void Box::CalculateEdges()
{
	glm::vec2 *edges = new glm::vec2[4]{ glm::vec2(0, m_extents.y), glm::vec2(-m_extents.x, 0),  glm::vec2(0, -m_extents.y), glm::vec2(m_extents.x, 0) };
	glm::mat2 rotateZ(cos(-m_rotation), -sin(-m_rotation), sin(-m_rotation), cos(-m_rotation));

	for (int i = 0; i < 4; i++)
	{
		glm::vec2 rotatedEdge = edges[i] * rotateZ;
		rotatedEdge += m_position;
		edges[i] = rotatedEdge;
		aie::Gizmos::add2DLine(m_position, edges[i], glm::vec4(0, 1, 0, 1));
	}

	m_edges = edges;
}
