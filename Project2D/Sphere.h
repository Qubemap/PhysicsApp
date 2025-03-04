#pragma once

#include "RigidBody.h"

class Sphere : public RigidBody
{
public:
	Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour, float elasticity, float linearDrag = 0.3f, float angularDrag = 0.3f);
	~Sphere() {}

	virtual void Draw();

	float GetRadius() { return m_radius; }
	glm::vec4 GetColour() { return m_colour; }

	void SetRadius (float radius) { m_radius = radius; }

protected:
	float m_radius;
	glm::vec4 m_colour;

};
