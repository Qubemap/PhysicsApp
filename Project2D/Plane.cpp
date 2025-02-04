#include "Plane.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Rigidbody.h"
#include <iostream>

Plane::Plane() : PhysicsObject(PLANE)
{

}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{

}

void Plane::Draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_colour, colourFade, colourFade);
}

void Plane::ResolveCollision(RigidBody* actor2)
{
	glm::vec2 normal = m_normal; // plane's surface (angle)
	glm::vec2 relativeVelocity = actor2->GetVelocity();

	// if the objects are already moving apart, we don't need to do anything
	if (glm::dot(normal, relativeVelocity) >= 0) { return; }

	float elasticity = 1; // no kinetic energy lost at 1

	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (1 / actor2->GetMass());
	//float j = -(1 + elasticity) * glm::dot(relativeVelocity, normal);

	glm::vec2 force = j * normal;

	float kePre = actor2->GetKineticEnergy(); // kePre = Kinetic energy before collision

	actor2->ApplyForce(force);

	float kePost = actor2->GetKineticEnergy();

	float deltaKe = kePost - kePre;
	if (deltaKe > kePost * 0.01f)
	{
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!" << std::endl;
	}
	else
	{
		std::cout << "allg man" << std::endl;
	}
}

void Plane::ResolveCollision(RigidBody* actor2, glm::vec2 contact)
{
	glm::vec2 relativeVelocity = actor2->GetVelocity();
	float e = actor2->GetElasticity();
	float j = glm::dot(-(1 + e) * (relativeVelocity), m_normal) / (1 / actor2->GetMass());

	glm::vec2 force = m_normal * j;
	actor2->ApplyForce(force, contact - actor2->GetPosition());
}
