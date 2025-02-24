#pragma once

#include "RigidBody.h"

class Box : public RigidBody
{
public:
	Box(glm::vec2 position, glm::vec2 velocity, float mass, float height, float width, float orientation, glm::vec4 colour, float elasticity, float linearDrag, float angularDrag);

	virtual void Draw();

	bool CheckBoxCorners(const Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);
	
	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }

	glm::vec2 GetExtents() const { return m_extents; }
	
protected:
	glm::vec2 m_extents;
	glm::vec4 m_colour;
};