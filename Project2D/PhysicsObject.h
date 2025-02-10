#pragma once

#include <vector>
#include <glm/glm.hpp>

enum ShapeType {
	PLANE = 0,
	SPHERE = 1,
	BOX = 2,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	PhysicsObject() {}
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Draw() = 0;
	virtual void ResetPosition() {};
	virtual float GetEnergy() { return 0; }

	ShapeType GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;
};