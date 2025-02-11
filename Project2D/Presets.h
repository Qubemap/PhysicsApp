#pragma once

#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

class Basketball : public Sphere
{
public:
	Basketball(glm::vec2 position, float radius) : Sphere(position, {0, 10}, 4, radius, {1, 0.5, 0.1, 1}, 0.9f, 0.1, 0.3)
	{
		
	}
};

class BowlingBall : public Sphere
{
public:
	BowlingBall(glm::vec2 position, float radius) : Sphere(position, { 0, 10 }, 50, radius, { 0, 0.5, 1, 1 }, 0.1f, 0.1, 0.3)
	{

	}
};

class Crate : public Box
{
public:
	Crate(glm::vec2 position, float height, float width) : Box(position, {0, 0}, (height + width) * 8, height, width, 0, {1, 0.5, 0.5, 1}, 0.1, 0.3f, 0.3f)
	{

	}
};

class Lino : public Plane
{
public:
	Lino(glm::vec2 normal, float distance) : Plane(normal, distance, { 0.5, 0.5, 1, 1 }, 0)
	{

	}
};

class Platform : public Box
{
public:
	Platform(glm::vec2 position, float height, float width) : Box(position, { 0, 0 }, 100000000, height, width, 0, { 0.5, 0.5, 1, 1 }, 0.1, 0.3f, 0.3f)
	{
		m_isKinematic = true;
		//m_mass = 100000000;
	}
};