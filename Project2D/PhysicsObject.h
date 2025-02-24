#pragma once

#include <vector>
#include <glm/glm.hpp>

class RigidBody;

enum ShapeType {
	JOINT = -1,
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT
};

class RigidBody;

class PhysicsObject
{
protected:
	PhysicsObject() { m_elasticity = 0; }
	PhysicsObject(ShapeType a_shapeID, float elasticity = 0) : m_shapeID(a_shapeID), m_elasticity(elasticity) {}
	

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual float GetEnergy() { return 0; }

	ShapeType GetShapeID() { return m_shapeID; }
	float GetElasticity() const { return m_elasticity; }
	RigidBody* GetParent() { return m_parent; }

	void SetElasticity(float elasticity) { m_elasticity = elasticity; }

protected:
	ShapeType m_shapeID;

	float m_elasticity;
	RigidBody* m_parent;
};