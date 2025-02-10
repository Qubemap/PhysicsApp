#include "RigidBody.h"
#include <iostream>
#include "PhysicsScene.h"

float MIN_LINEAR_THRESHOLD = 0.1;
float MIN_ANGULAR_THRESHOLD = 0.01;

RigidBody::RigidBody()
{
	m_shapeID = PLANE;
	m_position = { 0, 0 };
	m_velocity = { 0, 0 };
	m_orientation = 0.0f;
	m_mass = 0.0f;
	m_angularVelocity = 0;
	m_elasticity = 1;
	m_angularDrag = 0.3f;
	m_linearDrag = 0.3f;
	m_moment = 1;
}

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float elasticity)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
	m_elasticity = elasticity;
	m_angularVelocity = 0;
	m_angularDrag = 0.9f;
	m_linearDrag = 0.3f;
	m_moment = 1;

	std::cout << "shape: " << shapeID << std::endl;

}

void RigidBody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	// using getMass() and getMoment() here in case we ever get it to do something more than just return mass
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x - force.x * pos.y) / GetMoment();
}

//void RigidBody::ApplyForceToActor(RigidBody* actor2, glm::vec2 force)
//{
//	actor2->ApplyForce(force);
//	ApplyForce(force * glm::vec2(-1, -1));
//}

void RigidBody::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal)
{
	// find vector between centres, or use provided directionof force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);

	// vector perpendicular (90 deg) to collision normal
	glm::vec2 perp(normal.y, -normal.x);

	// determine total velocity of the contact points for the two objects, linear and rotational

	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2)
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.

		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = 1;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		// apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}
}

float RigidBody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}

float RigidBody::GetKineticEnergy()
{
	return 0.5f * (m_mass * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

float RigidBody::GetEnergy()
{
	return GetKineticEnergy() + GetPotentialEnergy();
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep, {0,0});

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	m_orientation += m_angularVelocity * timeStep;
	
	if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}


}