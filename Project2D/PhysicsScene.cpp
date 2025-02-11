#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>
#include "RigidBody.h"

glm::vec2 PhysicsScene::m_gravity = { 0, 0 };


PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	
}

PhysicsScene::PhysicsScene(glm::vec2 gravity)
{
	m_timeStep = 1.f / 60.f;
	m_gravity = gravity;
	
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	// make sure you consider the case where your client code asks to remove an actor that isn’t present in the scene.
	m_actors.erase(std::find(m_actors.begin(), m_actors.end(), actor));
}

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);
static fn collisionFunctionArray[] = 
{ 
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere, PhysicsScene::Plane2Box,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere, PhysicsScene::Sphere2Box,
	PhysicsScene::Box2Plane, PhysicsScene::Box2Sphere, PhysicsScene::Box2Box
};

void PhysicsScene::Update(float dt)
{
	// update physics at fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		//std::cout << GetTotalEnergy() << std::endl;
		for (auto pActor : m_actors) { pActor->FixedUpdate(m_gravity, m_timeStep); }

		accumulatedTime -= m_timeStep;
		int actorCount = m_actors.size();

		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = m_actors[outer];
				PhysicsObject* object2 = m_actors[inner];
				int shapeID1 = object1->GetShapeID();
				int shapeID2 = object2->GetShapeID();

				// using fn pointers
				int functionID = (shapeID1 * SHAPE_COUNT) + shapeID2;
				fn collisionFunctionPtr = collisionFunctionArray[functionID];
				if (collisionFunctionPtr != nullptr)
				{
					collisionFunctionPtr(object1, object2);
				}
			}
		}
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw();
	}
}

float PhysicsScene::GetTotalEnergy()
{
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++)
	{
		PhysicsObject* obj = *it;
		total += obj->GetEnergy();
	}
	return total;
}

void PhysicsScene::ApplyContactForces(RigidBody* body1, RigidBody* body2, glm::vec2 norm, float pen)
{
	float body2Mass = body2 ? body2->GetMass() : INT_MAX;

	float body1Factor = body2Mass / (body1->GetMass() + body2Mass);

	body1->SetPosition(body1->GetPosition() - body1Factor * norm * pen);

	if (body2)
	{
		body2->SetPosition(body2->GetPosition() + (1 - body1Factor) * norm * pen);
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
}

bool PhysicsScene::Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);
	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		// get a representative point on the plane
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		// check all four corners to see if we've hit the plane
		for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
		{
			for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
			{
				// get the position of the corner in world space
				glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
				// this is the total velocity of the point in world space
				glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
				glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
				// and this is the component of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->getNormal());
				// and moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				}
			}
		}
		// we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			plane->ResolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// D = (C . N) - D - R

	//D1 is the distance of the sphere surface to the plane surface
	//	C is the centre of the sphere
	//	N is the normal to the plane
	//	D is the distance of the plane from the origin
	//	R is the radius of the sphere

	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->GetPosition(), plane->getNormal()) - plane->getDistance();

		float intersection = sphere->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->GetVelocity(), plane->getNormal());
		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			glm::vec2 contact = sphere->GetPosition() + (collisionNormal * -sphere->GetRadius());
			plane->ResolveCollision(sphere, contact);
			//sphere->ApplyForce(-sphere->GetVelocity() * sphere->GetMass());

			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float penetration = sphere1->GetRadius() + sphere2->GetRadius() - glm::distance(sphere1->GetPosition(), sphere2->GetPosition());
		if (penetration > 0)
		{
			glm::vec2 contact = 0.5f * (sphere1->GetPosition() + sphere2->GetPosition());
			sphere1->ResolveCollision(sphere2, contact, nullptr, penetration);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Box2Sphere(obj2, obj1);
}

bool PhysicsScene::Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Plane2Box(obj2, obj1);
}

bool PhysicsScene::Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
	if (box != nullptr && sphere != nullptr)
	{
		// transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = sphere->GetPosition() - box->GetPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box -> GetLocalX()), glm::dot(circlePosWorld, box->GetLocalY()));
			// find the closest point to the circle centre on the box by clamping the coordinates in box - space to the box's extents
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->GetExtents();
		if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = - extents.x;
		if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = - extents.y;
		if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;
		// and convert back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
		glm::vec2 circleToBox = sphere->GetPosition() - closestPointOnBoxWorld;
		float pen = sphere->GetRadius() - glm::length(circleToBox);
		if (pen > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->ResolveCollision(sphere, contact, &direction);
		}
	}
	return false;
}

bool PhysicsScene::Box2Box(PhysicsObject * obj1, PhysicsObject * obj2) {
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr) {
		glm::vec2 boxPos = box2->GetPosition() - box1->GetPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->CheckBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, norm)) 
		{
			norm = -norm;
		}
		if (pen > 0) 
		{
			box1->ResolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	return false;
}
