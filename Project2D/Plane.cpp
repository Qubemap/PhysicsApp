#include "Plane.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Rigidbody.h"
#include <iostream>
#include "PhysicsScene.h"

Plane::Plane() : PhysicsObject(PLANE)
{

}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour, float elasticity) : PhysicsObject(PLANE, elasticity)
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

void Plane::ResolveCollision(RigidBody* actor2, glm::vec2 contact)
{
	// the position at which we'll apply the force relative to the object's COM
	glm::vec2 localContact = contact - actor2->GetPosition();
	// the plane isn't moving, so the relative velocity is just actor2's velocity
	//at the contact point
	glm::vec2 vRel = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = glm::dot(vRel, m_normal);
	// perfectly elasticity collisions for now
	float e = actor2->GetElasticity();
	// this is the perpendicular distance we apply the force at relative to the COM, so Torque = F * r
	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));
	// work out the "effective mass" - this is a combination of moment of
	// inertia and mass, and tells us how much the contact point velocity
	// will change with the force we're applying
	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2 -> GetMoment());
	float j = -(1 + e) * velocityIntoPlane * mass0;
	glm::vec2 force = m_normal * j;
	float kePre = actor2->GetKineticEnergy();
	actor2->ApplyForce(force, contact - actor2->GetPosition());
	float pen = glm::dot(contact, m_normal) - m_distanceToOrigin;
	PhysicsScene::ApplyContactForces(actor2, nullptr, m_normal, pen);
	float kePost = actor2->GetKineticEnergy();
	float deltaKE = kePost - kePre;
	if (deltaKE > kePost * 0.01f)
	{
		std::cout << "Kinetic Energy discrepancy greater than 1% detected!";
	}
	/*else
	{
		std::cout << "allg man" << std::endl;
	}*/
}

//void Plane::ResolveCollision(RigidBody* actor2, glm::vec2 contact)
//{
//	// position at which we'll apply the force relative to the center of mass
//	glm::vec2 localContact = contact - actor2->GetPosition();
//	// plane isn't moving, so rel velocity is just the other object's velocity
//	glm::vec2 relativeVelocity = actor2->GetVelocity() + actor2->GetAngularVelocity() * glm::vec2(-localContact.y, localContact.x);
//	float velocityIntoPlane = glm::dot(relativeVelocity, m_normal);
//
//	float e = 1; //actor2->GetElasticity();
//	float r = glm::dot(localContact, glm::vec2(m_normal.y, -m_normal.x));
//	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());
//
//	float j = -(1 + e) * velocityIntoPlane * mass0;
//	// glm::dot(-(1 + e) * (relativeVelocity), m_normal) / (1 / actor2->GetMass());
//
//	glm::vec2 force = m_normal * j;
//	
//	float kePre = actor2->GetKineticEnergy(); // kePre = Kinetic energy before collision
//
//	actor2->ApplyForce(force, contact - actor2->GetPosition());
//
//	float kePost = actor2->GetKineticEnergy();
//
//	float deltaKe = kePost - kePre;
//	if (deltaKe > kePost * 0.01f)
//	{
//		std::cout << "Kinetic Energy discrepancy greater than 1% detected!" << std::endl;
//	}
//	else
//	{
//		std::cout << "allg man" << std::endl;
//	}
//}

//void Plane::ResolveCollision(RigidBody* actor2)
//{
//	glm::vec2 normal = m_normal; // plane's surface (angle)
//	glm::vec2 relativeVelocity = actor2->GetVelocity();
//
//	// if the objects are already moving apart, we don't need to do anything
//	if (glm::dot(normal, relativeVelocity) >= 0) { return; }
//
//	float elasticity = (GetElasticity() + actor2->GetElasticity() / 2); // no kinetic energy lost at 1
//
//	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) / (1 / actor2->GetMass());
//	//float j = -(1 + elasticity) * glm::dot(relativeVelocity, normal);
//
//	glm::vec2 force = j * normal;
//
//	float kePre = actor2->GetKineticEnergy(); // kePre = Kinetic energy before collision
//
//	actor2->ApplyForce(force, { 0,0 });
//
//	float kePost = actor2->GetKineticEnergy();
//
//	float deltaKe = kePost - kePre;
//	if (deltaKe > kePost * 0.01f)
//	{
//		std::cout << "Kinetic Energy discrepancy greater than 1% detected!" << std::endl;
//	}
//	else
//	{
//		std::cout << "allg man" << std::endl;
//	}
//}
