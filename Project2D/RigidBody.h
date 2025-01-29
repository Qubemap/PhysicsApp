#pragma once

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass);
	~RigidBody() {}

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force);
	void ApplyForceToActor(RigidBody* actor2, glm::vec2 force);

	glm::vec2 GetPosition() { return m_position; }
	float GetOrientation() { return m_orientation; }
	glm::vec2 GetVelocity() { return m_velocity; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	float GetMass() { return m_mass; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D, only a single float needed
};