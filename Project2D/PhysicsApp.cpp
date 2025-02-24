#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Presets.h"

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	glm::vec2 gravity = { 0, -9.8f };
	m_physicsScene = new PhysicsScene(gravity);
	
	/*Platform* platform = new Platform({ 0, -50 }, 200, 20);
	m_physicsScene->AddActor(platform);*/

	Lino* ground = new Lino({ 0,1 }, -50);
	m_physicsScene->AddActor(ground);

	Rope(13);

	float X = 12;

	//Crate* lgWall = new Crate({ X, -50 }, 5, 30);
	//m_physicsScene->AddActor(lgWall);

	//Crate* rgWall = new Crate({ X + 30, -50 }, 5, 30);
	//m_physicsScene->AddActor(rgWall);

	Crate* rgWall = new Crate({ X + 15, -50 }, 5, 30);
	m_physicsScene->AddActor(rgWall);

	Crate* gFloor = new Crate({ X + 15, -20}, 40, 5);
	m_physicsScene->AddActor(gFloor);

	//rgWall->AddChild(gFloor);

	Crate* loneWall = new Crate({ X + 0, 0 }, 5, 30);
	m_physicsScene->AddActor(loneWall);

	Crate* roneWall = new Crate({ X + 30, 0 }, 5, 30);
	m_physicsScene->AddActor(roneWall);

	

	Crate* oneFloor = new Crate({ X + 15, 16 }, 35, 5);
	m_physicsScene->AddActor(oneFloor);
	
	return true;
}

/*Lino* line1 = new Lino({ -0.65 , 0.75 }, -30);
	Lino* line2 = new Lino({ 0.65, 0.75 }, -30);
	Lino* line3 = new Lino({ 0, 1 }, -25);
	Lino* line4 = new Lino({ 0, -1 }, -50);*/

	//m_physicsScene->AddActor(line1);
	//m_physicsScene->AddActor(line2);
	//m_physicsScene->AddActor(line3);  // flat plane
	//m_physicsScene->AddActor(line4);

//ROCKET
//m_physicsScene->setGravity(glm::vec2(0, -1));
//m_physicsScene->SetTimeStep(0.01f); // Decreasing the value of this timestep will increase the accuracy of our physics simulation at the expense of increased processing time 0.01f
//Sphere* rocket = new Sphere(glm::vec2(0, -50), glm::vec2(0, 0), 50, 4, glm::vec4(1, 0, 0, 1));
//m_physicsScene->AddActor(rocket);

void PhysicsApp::shutdown() {
	
	delete m_font;
	//delete m_texture;
	//delete m_shipTexture;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
	{
		if (m_ball)
		{
			m_ball->ApplyForce({ -4, 0 }, { 0,0 });
		}
	}
	else if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
	{
		if (m_ball)
		{
			m_ball->ApplyForce({ 4, 0 }, { 0,0 });
		}
	}
	
	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// TODO draw stuff here!
	static float aspectRatio = 16 / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f ));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 0);

	m_2dRenderer->drawText(m_font, "Left Arrow Key <-", 0, 650);

	m_2dRenderer->drawText(m_font, "Right Arrow Key ->", 900, 650);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::Rope(int num)
{
	//m_physicsScene->SetGravity(glm::vec2(0, -9.82f));
	Sphere* prev = nullptr;
	for (int i = 0; i <= num; i++)
	{
		float mass = 1;
		float radius = 0.5;
		if (i == num)
		{
			mass = 20;
			radius = 2;
		}
		
		Sphere* sphere = new Sphere(glm::vec2(-5, 50 + i * -4), glm::vec2(0), mass, radius, glm::vec4(1, 0, 0, 1), 0.5, 0.1, 0.1); //-20 + (i * -4), 50

		if (i == 0)
		{
			sphere->SetKinematic(true);
		}

		if (mass > 1)
		{
			m_ball = sphere;
		}

		m_physicsScene->AddActor(sphere);

		if (prev)
		{
			m_physicsScene->AddActor(new Spring(sphere, prev, 2000, 0, 4)); //springcoefficient, damping, restlength
		}

		prev = sphere;
	}
	// add a kinematic box at an angle for the rope to drape over
	//Platform* box = new Platform(glm::vec2(0, -20), 20, 20);
	//m_physicsScene->AddActor(box);

}
