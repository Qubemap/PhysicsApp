#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"

PhysicsApp::PhysicsApp() {

}

PhysicsApp::~PhysicsApp() {

}

bool PhysicsApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);
	m_physicsScene->setGravity({ 0, -10 });

	Plane* line1 = new Plane({ 0, 1 }, -30);
	Plane* line2 = new Plane({ 0, 2 }, -10);
	Sphere* ball1 = new Sphere({ 0, 0 }, glm::vec2(0), 8, 8, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere({ 30, 2 }, glm::vec2(0), 4, 4, glm::vec4(0, 1, 0, 1));

	ball1->ApplyForce({100, 0});
	
	m_physicsScene->AddActor(line1);
	m_physicsScene->AddActor(line2);
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	return true;
}

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

	// done drawing sprites
	m_2dRenderer->end();
}