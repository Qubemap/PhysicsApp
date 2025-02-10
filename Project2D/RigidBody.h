#pragma once

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float elasticity);
	~RigidBody() {}

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	//void ApplyForceToActor(RigidBody* actor2, glm::vec2 force);
	void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr);
	
	glm::vec2 GetPosition() const { return m_position; }
	float GetOrientation() const { return m_orientation; }
	glm::vec2 GetVelocity() const { return m_velocity; }
	float GetMass() const { return m_mass; }
	float GetKineticEnergy();
	float GetEnergy() override;
	float GetPotentialEnergy();
	float GetAngularVelocity() const { return m_angularVelocity; }
	float GetMoment() const { return m_moment; }
	float GetLinearDrag() const { return m_linearDrag; }
	float GetAngularDrag() const { return m_angularDrag; }

	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetMass(float mass) { m_mass = mass; }
	void SetLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	void SetAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D, only a single float needed
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag;
	float m_angularDrag;
};