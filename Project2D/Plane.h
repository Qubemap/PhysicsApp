#pragma once

#include "PhysicsObject.h"

class RigidBody;

class Plane : public PhysicsObject
{
public:
	Plane();
	Plane(glm::vec2 normal, float distance, glm::vec4 colour = { 1, 0, 0, 1 }, float elasticity = 0.5);
	~Plane() {}

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Draw();
	//virtual void ResolveCollision(RigidBody* actor2);
	virtual void ResolveCollision(RigidBody* actor2, glm::vec2 contact);

	glm::vec2 getNormal() { return m_normal; }
	float getDistance() { return m_distanceToOrigin; }

protected:
	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_colour;
};