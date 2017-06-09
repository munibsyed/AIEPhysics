#include "Plane3D.h"


Plane3D::Plane3D(glm::vec3 normal, glm::vec3 distanceFromOrigin, glm::vec4 colour)
{
	m_normal = normal;
	m_distanceFromOrigin = distanceFromOrigin;
	m_colour = colour;
	m_shapeID = PLANE;
}

Plane3D::~Plane3D()
{
}

void Plane3D::MakeGizmo()
{
	glm::vec3 centrePoint = m_distanceFromOrigin - m_normal;
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	glm::vec3 parallel(m_normal.y, m_normal.z, m_normal.x);
	//glm::vec3 parallel2(m_normal.z, m_normal.x, m_normal.y);
	glm::vec3 parallel2 = glm::cross(m_normal, parallel);

	int range = 10;
	for (int i = -range; i < range; i++)
	{
		for (int ii = -range; ii < range; ii++)
		{
			glm::vec3 pos = (parallel * i) + (parallel2 * ii);
		//	std::cout << pos.x << ", " << pos.z << std::endl;
			aie::Gizmos::addLine(centrePoint + pos, centrePoint + (parallel * (i)) + (parallel2 * (ii + 1)), m_colour);
			aie::Gizmos::addLine(centrePoint + pos, centrePoint + (parallel * (i+1)) + (parallel2 * (ii)), m_colour);

			/*pos = parallel2 * ii;
*/
		}
	}

	


	//aie::Gizmos::addSphere(start, 0.5f, 8, 8, glm::vec4(1, 0, 0, 1));
	//aie::Gizmos::addSphere(end, 0.5f, 8, 8, glm::vec4(1, 0, 0, 1));


}

void Plane3D::FixedUpdate(glm::vec3 gravity, float timeStep)
{
}

glm::vec3 Plane3D::GetNormal()
{
	return m_normal;
}

glm::vec3 Plane3D::GetDistanceFromOrigin()
{
	return m_distanceFromOrigin;
}
