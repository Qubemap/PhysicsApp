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


void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);
}