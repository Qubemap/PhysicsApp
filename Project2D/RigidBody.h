#pragma once

#include "PhysicsObject.h"

class RigidBody : public PhysicsObject
{
public:
	RigidBody();
	RigidBody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float orientation, float mass, float elasticity, float linearDrag = 0.3f, float angularDrag = 0.3f);
	~RigidBody() {}

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	void ApplyForce(glm::vec2 force, glm::vec2 pos);
	void ResolveCollision(RigidBody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr, float pen = 0);
	glm::vec2 ToWorld(glm::vec2 localPos);
	void AddChild(RigidBody* child);
	bool IsKinematic() const { return m_isKinematic; }

	float GetKineticEnergy();
	float GetEnergy() override;
	float GetPotentialEnergy();
	
	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() const { return m_parent->m_velocity; }
	float GetMass() const { return m_parent->m_isKinematic ? 10000000 : m_parent->m_mass; }
	float GetOrientation() const { return m_orientation; }
	float GetAngularVelocity() const { return m_parent->m_angularVelocity; }
	float GetMoment() const { return m_parent->m_isKinematic ? 10000000 : m_parent->m_moment; }
	float GetLinearDrag() const { return m_linearDrag; }
	float GetAngularDrag() const { return m_angularDrag; }
	glm::vec2 GetLocalX() const { return m_localX; }
	glm::vec2 GetLocalY() const { return m_localY; }

	void SetPosition(glm::vec2 position) { m_position = position; }
	void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void SetMass(float mass) { m_mass = mass; }
	void SetAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }
	void SetOrientation(float orientation) { m_orientation = orientation; }
	void SetLinearDrag(float linearDrag) { m_linearDrag = linearDrag; }
	void SetAngularDrag(float angularDrag) { m_angularDrag = angularDrag; }
	void SetKinematic(bool isKinematic) { m_isKinematic = isKinematic; }
	void SetLocalPosition(glm::vec2 position) { m_localPosition = position; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_orientation; // 2D, only a single float needed
	float m_angularVelocity;
	float m_moment; 
	float m_linearDrag; 
	float m_angularDrag; // rotational drag
	bool m_isKinematic; // if true, object won't move
	glm::vec2 m_localX; // local x axis
	glm::vec2 m_localY; // local y axis
	std::vector<RigidBody*> m_children;
	glm::vec2 m_localPosition;

};