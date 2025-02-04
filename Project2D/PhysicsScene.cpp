#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"
#include "Plane.h"

PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = { 0, 0 };
}

PhysicsScene::PhysicsScene(glm::vec2 gravity)
{
	m_timeStep = 0.01f;
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
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere,
};

void PhysicsScene::Update(float dt)
{
	// update physics at fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
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

bool PhysicsScene::Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return Sphere2Plane(obj2, obj1);
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
			plane->ResolveCollision(sphere);
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
		if ((sphere1->GetRadius() + sphere2->GetRadius()) > glm::distance(sphere1->GetPosition(), sphere2->GetPosition())) // if overlap
		{
			sphere1->ResolveCollision(sphere2);

			return true;
		}
	}

	return false;
}
