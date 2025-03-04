#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	virtual ~PhysicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	virtual void Rope(int num);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	//aie::Texture*		m_shipTexture;
	aie::Font*			m_font;

	RigidBody* m_ball;

	float m_timer;
	PhysicsScene* m_physicsScene;
};