#include "RigidBody.h"
#include <iostream>
#include "PhysicsScene.h"

float MIN_LINEAR_THRESHOLD = 0.2;
float MIN_ANGULAR_THRESHOLD = 0.01;

RigidBody::RigidBody()
{
	m_shapeID = PLANE;
	m_position = { 0, 0 };
	m_velocity = { 0, 0 };
	m_orientation = 0.0f;
	m_mass = 0.0f;
	m_angularVelocity = 0;
	m_elasticity = 0;
	m_angularDrag = 0.3f;
	m_linearDrag = 0.3f;
	m_moment = 1;
	m_isKinematic = false;
}

RigidBody::RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float elasticity, float linearDrag, float angularDrag) : PhysicsObject(shapeID, elasticity)
{
	//m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_orientation = orientation;
	m_mass = mass;
	//m_elasticity = elasticity;
	m_angularVelocity = 0;
	m_angularDrag = angularDrag;
	m_linearDrag = linearDrag;
	m_moment = 1;
	m_isKinematic = false;

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

void RigidBody::ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen)
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

		float mass1 = 1.0f / (1.0f / GetMass() + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->GetMass() + (r2 * r2) / actor2->m_moment);

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		// apply equal and opposite forces
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);

		//std::cout << "rigidbody collision" << std::endl;
	}

	if (pen > 0)
	{
		PhysicsScene::ApplyContactForces(this, actor2, normal, pen);
	}
}

float RigidBody::GetPotentialEnergy()
{
	return -GetMass() * glm::dot(PhysicsScene::GetGravity(), GetPosition());
}

float RigidBody::GetKineticEnergy()
{
	return 0.5f * (GetMass() * glm::dot(m_velocity, m_velocity) + m_moment * m_angularVelocity * m_angularVelocity);
}

float RigidBody::GetEnergy()
{
	return GetKineticEnergy() + GetPotentialEnergy();
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}

	std::cout << m_velocity[0] << " : " << m_velocity[1] << std::endl;

	m_position += m_velocity * timeStep;

	ApplyForce(gravity * GetMass() * timeStep, {0,0});

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	m_orientation += m_angularVelocity * timeStep;
	
	
	if (abs(m_velocity.x) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, m_velocity.y);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
	{
		m_angularVelocity = 0;
	}
	

	
}