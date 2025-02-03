#include "Plane.h"
#include <glm\ext.hpp>
#include <Gizmos.h>


//Plane(glm::vec2 normal, float distance) : m_normal(normal), m_distanceToOrigin(distance), m_colour({ 1, 0, 0, 1 }) {}
//Plane(glm::vec2 normal, float distance, glm::vec4 colour) : m_normal(normal), m_distanceToOrigin(distance), m_colour(colour) {}


Plane::Plane() : PhysicsObject(PLANE)
{
	m_normal = { 0, 1 };
	m_distanceToOrigin = 0;
	m_colour = { 1, 0, 0, 1 };
}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) : PhysicsObject(PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
}

void Plane::FixedUpdate(glm::vec2 gravity, float timeStep)
{
}

void Plane::Draw()
{
	float lineSegmentLength = 300;
	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec4 colourFade = m_colour;
	colourFade.a = 0;
	glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
	glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
	//aie::Gizmos::add2DLine(start, end, colour);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour, colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_colour, colourFade, colourFade);
}

void Plane::ResetPosition()
{

}
