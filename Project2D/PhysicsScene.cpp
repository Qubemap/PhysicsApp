#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Sphere.h"


PhysicsScene::PhysicsScene()
{
	m_timeStep = 0.01f;
	m_gravity = { 0, 0 };
}

void PhysicsScene::Update(float dt)
{
	// update physics at fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		// Check for collisions (ideally have scene mgmt)
		int actorCount = m_actors.size();
		for (int outer = 0; outer < actorCount - 1; outer++)
		{
			for (int inner = outer + 1; inner < actorCount; inner++)
			{
				PhysicsObject* object1 = m_actors[outer];
				PhysicsObject* object2 = m_actors[inner];

				// TEMP assuming shapes are spheres
				Sphere2Sphere(object1, object2);
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
			sphere1->SetVelocity(glm::vec2(0,0));
			sphere2->SetVelocity(glm::vec2(0,0));

			return true;
		}
	}

	return false;
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

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

