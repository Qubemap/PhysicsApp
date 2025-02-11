#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class Basketball : public Sphere
{
public:
	Basketball(glm::vec2 position, float radius) : Sphere(position, {0, 10},10, radius, {1, 0.5, 0.1, 1}, 0.8f, 0.3, 0.3)
	{
		
	}
};

class Crate : public Box
{
public:
	Crate(glm::vec2 position, float height, float width) : Box(position, {0, 0}, (height + width) * 8, height, width, 0, {1, 0.5, 0.5, 1}, 0.1, 0.3, 0.3)
	{

	}
};

class Lino : public Plane
{
public:
	Lino(glm::vec2 normal, float distance) : Plane(normal, distance, { 0.5, 0.5, 1, 1 }, 0.5)
	{

	}
};