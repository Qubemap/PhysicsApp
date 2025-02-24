#pragma once

#include <glm/glm.hpp>
#include <vector>


class PhysicsObject;
class RigidBody;

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
	static void ApplyContactForces(RigidBody* body1, RigidBody* body2, glm::vec2 norm, float pen);

	static bool Plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Plane2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Plane(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2);
	static bool Box2Box(PhysicsObject* obj1, PhysicsObject* obj2);


	//void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	static glm::vec2 GetGravity() { return m_gravity; }
	static void SetGravity(glm::vec2 gravity) { m_gravity = gravity; }

	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }

	void SetBall(RigidBody* ball) { m_ball = ball; }


protected:
	static glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;
	RigidBody* m_ball;
	

};