#pragma once

#include <glm/glm.hpp>
#include <vector>


class PhysicsObject;

class PhysicsScene
{


public:
	PhysicsScene();
	PhysicsScene(glm::vec2 gravity);
	~PhysicsScene();

	void AddActor(PhysicsObject* actor);
	void RemoveActor(PhysicsObject* actor);
	void Update(float dt);
	void Draw();
	float GetTotalEnergy();

	static bool Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);

	//void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	static glm::vec2 GetGravity() { return m_gravity; }
	static void SetGravity(glm::vec2 gravity) { m_gravity = gravity; }

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

protected:
	static glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	

};