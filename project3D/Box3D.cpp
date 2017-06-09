#include "Box3D.h"


Box3D::Box3D(glm::vec3 position, glm::vec3 velocity, glm::vec3 extents, float mass, float linearDrag, float angularDrag, bool isFilled, bool isStatic, glm::vec4 colour)
{
	m_position = position;
	m_extents = extents;
	m_mass = mass;
	m_velocity = velocity;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;
	m_isStatic = isStatic;
	m_colour = colour;
	m_isFilled = isFilled;
	m_shapeID = BOX;

	//m_rotationMatrix = glm::mat4();
	//m_rotation = glm::vec3(0);
	m_rotation = glm::vec3(PI / 4.0f, 0.0f, PI / 4.0f);

	m_corners = new glm::vec3[8];
	m_faces = new glm::vec3[6];
}

Box3D::~Box3D()
{
	delete m_corners;
	delete m_faces;
}

void Box3D::MakeGizmo()
{
	m_rotationMatrix = glm::mat4();
	m_rotationMatrix = glm::rotate(m_rotation.x, glm::vec3(m_rotationMatrix[0]));
	m_rotationMatrix *= glm::rotate(m_rotation.y, glm::vec3(m_rotationMatrix[1]));
	m_rotationMatrix *= glm::rotate(m_rotation.z, glm::vec3(m_rotationMatrix[2]));

	if (m_isFilled)
	{
		aie::Gizmos::addAABBFilled(m_position, m_extents, m_colour, &m_rotationMatrix);
	}
	else
		aie::Gizmos::addAABB(m_position, m_extents, m_colour, &m_rotationMatrix);


	//m_rotationMatrix = glm::mat4();
	//m_rotationMatrix = glm::rotate(-m_rotation.x, glm::vec3(m_rotationMatrix[0]));
	//m_rotationMatrix *= glm::rotate(m_rotation.y, glm::vec3(m_rotationMatrix[1]));
	//m_rotationMatrix *= glm::rotate(m_rotation.z, glm::vec3(m_rotationMatrix[2]));

	//for (int i = 0; i < 8; i++)
	//{
	//	glm::vec4 cornerPos = glm::vec4(m_corners[i].x, m_corners[i].y, m_corners[i].z, 1) * m_rotationMatrix;
	//	glm::vec3 cornerVec3 = glm::vec3(-cornerPos.x, cornerPos.y, cornerPos.z);
	//	aie::Gizmos::addSphere(m_position + (cornerVec3), 0.1f, 8, 8, glm::vec4(1, 0, 0, 1));
	//}
}

glm::vec3 Box3D::GetExtents()
{
	return m_extents;
}

void Box3D::CalculateCorners()
{
	glm::vec4 extents = glm::vec4(m_extents.x, m_extents.y, m_extents.z, 1);

	//m_rotationMatrix = glm::mat4();
	//m_rotationMatrix = glm::rotate(-m_rotation.x, glm::vec3(m_rotationMatrix[0]));
	//m_rotationMatrix *= glm::rotate(m_rotation.y, glm::vec3(m_rotationMatrix[1]));
	//m_rotationMatrix *= glm::rotate(m_rotation.z, glm::vec3(m_rotationMatrix[2]));

	//extents = extents * m_rotationMatrix;

	m_corners[0] = glm::vec3(extents.x, extents.y, extents.z);
	m_corners[1] = glm::vec3(extents.x, extents.y, -extents.z);
	m_corners[2] = glm::vec3(extents.x, -extents.y, extents.z);
	m_corners[3] = glm::vec3(extents.x, -extents.y, -extents.z);
	m_corners[4] = glm::vec3(-extents.x, extents.y, extents.z);
	m_corners[5] = glm::vec3(-extents.x, extents.y, -extents.z);
	m_corners[6] = glm::vec3(-extents.x, -extents.y, extents.z);
	m_corners[7] = glm::vec3(-extents.x, -extents.y, -extents.z);


}

void Box3D::CalculateEdges()
{
}

glm::vec3 * Box3D::GetCorners()
{
	return nullptr;
}

glm::vec3 * Box3D::GetFaces()
{
	return nullptr;
}

void Box3D::AddRotationX(float x)
{
	m_rotation.x += x;
}
