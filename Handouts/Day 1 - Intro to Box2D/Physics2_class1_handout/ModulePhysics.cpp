#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include "Box2D/Box2D/Box2D.h"
#include "Box2D/Box2D.h"
#include "ModuleInput.h"

// TODO 1: Include Box 2 header and library
#ifdef _DEBUG
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")
#endif //_DEBUG


#ifdef _RELEASE
#pragma comment(lib, "Box2D/libx86/Release/Box2D.lib")
#endif //_RELEASE

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it
	b2Vec2 gravity(0.0f, 0.3f);
	world = new b2World(gravity);

	// TODO 4: Create a a big static circle as "ground"
	int x = 500, y = 450;
	float radius = 270.0f;
	b2BodyDef body_def;
	body_def.type = b2_staticBody; // or b2_dynamicBody
	body_def.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	b2Body* body = world->CreateBody(&body_def);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	body->CreateFixture(&fixture);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	float32 timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	for (int32 i = 0; i < 60; ++i)
	{
		world->Step(timeStep, velocityIterations, positionIterations);
		//b2Vec2 position = body->GetPosition();
		//float32 angle = body->GetAngle();
		//printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		//float radius = 20.0f; 
		float radius = rand() % 30 + 5; //Random ball size
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body_def.position.Set(PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()));
		b2Body* body = world->CreateBody(&body_def);

		b2CircleShape dynshape;
		dynshape.m_radius = PIXEL_TO_METERS(radius);

		b2FixtureDef fixture;
		fixture.shape = &dynshape;
		body->CreateFixture(&fixture);

	}

	if (!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete  world;

	return true;
}
