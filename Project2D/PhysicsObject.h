#pragma once

#include <vector>
#include <glm/glm.hpp>

enum ShapeType {
	Plane = 0,
	Sphere,
	Box
};

class PhysicsObject
{
protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};

protected:
	ShapeType m_shapeID;
};