#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class Basketball : public Sphere
{
public:
	Basketball(glm::vec2 position, float radius) : Sphere(position, {0, 10},10, radius, {1, 1.5, 0.1, 1}, 0.8)
	{
		m_position = position;
	}
};