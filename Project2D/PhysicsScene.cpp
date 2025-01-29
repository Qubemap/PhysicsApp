#include "PhysicsScene.h"


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
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->draw();
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

