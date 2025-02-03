#include "RigidBody.h"

RigidBody::RigidBody()
{
	m_shapeID = PLANE;
	m_position = { 0, 0 };
	m_velocity = { 0, 0 };
	m_orientation = 0.0f;
	m_mass = 0.0f;
}

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
}

void RigidBody::ApplyForce(glm::vec2 force)
{
	m_velocity = m_velocity + (force / m_mass);
}

void RigidBody::ApplyForceToActor(RigidBody* actor2, glm::vec2 force)
{
	actor2->ApplyForce(force);
	ApplyForce(force * glm::vec2(-1, -1));
}

void RigidBody::ResolveCollision(RigidBody* actor2)
{
	glm::vec2 normal = glm::normalize(actor2->GetPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;

	// if the objects are already moving apart, we don't need to do anything
	if (glm::dot(normal, relativeVelocity) >= 0) { return; }

	float elasticity = 1;
	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / ((1 / m_mass) + (1 / actor2->GetMass()));

	glm::vec2 force = normal * j;

	ApplyForceToActor(actor2, force);
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);
}