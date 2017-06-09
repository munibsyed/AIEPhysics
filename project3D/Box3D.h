#pragma once
#include "Rigidbody3D.h"

class Box3D : public Rigidbody3D
{
public:
	Box3D(glm::vec3 position, glm::vec3 velocity, glm::vec3 extents, float mass, float linearDrag, float angularDrag, bool isFilled, bool isStatic, glm::vec4 colour);
	~Box3D();

	void MakeGizmo();

	glm::vec3 GetExtents();

	void CalculateCorners();

	void CalculateEdges();

	glm::vec3* GetCorners();

	glm::vec3* GetFaces();

	void AddRotationX(float x);

private:
	bool m_isFilled;
	glm::vec3 m_extents;

	glm::vec3* m_corners;
	glm::vec3* m_faces;
};

