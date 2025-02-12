#include "Spring.h"
#include "Gizmos.h"

Spring::Spring(RigidBody* body1, RigidBody* body2, float springCoefficient, float damping, float restLength, glm::vec2 contact1, glm::vec2 contact2, glm::vec4 colour)
	: m_body1(body1), m_body2(body2), m_restLength(restLength), m_springCoefficient(springCoefficient), m_damping(damping), m_contact1(contact1), m_contact2(contact2), m_colour(colour)
{
	m_shapeID = JOINT;
}

void Spring::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	// get the world coordinates of the ends of the springs
	glm::vec2 p1 = GetContact1();
	glm::vec2 p2 = GetContact2();
	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);
	// apply damping
	glm::vec2 relativeVelocity = m_body2->GetVelocity() - m_body1->GetVelocity();
	// F = -kX - bv
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;
	m_body1->ApplyForce(-force * timeStep, p1 - m_body1->GetPosition());
	m_body2->ApplyForce(force * timeStep, p2 - m_body2->GetPosition());
}

void Spring::Draw()
{
	aie::Gizmos::add2DLine(GetContact1(), GetContact2(), m_colour);
}