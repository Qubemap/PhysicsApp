#include "RigidBody.h"

class Spring : public RigidBody
{
public:
	Spring(glm::vec4 colour = {1, 0, 0, 1}) { m_colour = colour; }
	Spring(RigidBody* body1, RigidBody* body2, float springCoefficient, float damping, float restLength, glm::vec2 contact1 = { 0,0 }, glm::vec2 contact2 = { 0,0 }, glm::vec4 colour = { 1, 0, 0, 1 });
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) override;
	virtual void Draw();

	glm::vec2 GetContact1() { return m_body1 ? m_body1->ToWorld(m_contact1) : m_contact1; }
	glm::vec2 GetContact2() { return m_body2 ? m_body2->ToWorld(m_contact2) : m_contact2; }

private:
	RigidBody* m_body1;
	RigidBody* m_body2;
	glm::vec2 m_contact1;
	glm::vec2 m_contact2;
	float m_damping;
	float m_restLength;
	float m_springCoefficient; // the restoring force;
	glm::vec4 m_colour;
};